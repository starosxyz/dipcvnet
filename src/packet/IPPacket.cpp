/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/IPPacket.h>
#define IP_MAX_SIZE 1440


IPPacket::IPPacket()
    :CEthernet(0x0800)
{
    m_version= 4;
    m_headerLength= 0;
    m_diffServ= 0;
    m_totalLength= 0;
    m_identification= 0;
    m_flags= 0;
    m_fragmentOffset= 0;
    m_ttl= 0;
    m_protocol= 0;
    m_checksum= 0;
    m_sourceAddress= 0;
    m_destinationAddress= 0;
    m_optionsLength = 0;
    m_options = NULL;
    m_isTruncated=false;
}
IPPacket::IPPacket(uint8_t protocol)
    :CEthernet(0x0800)
{
    m_version= 4;
    m_headerLength= 0;
    m_diffServ= 0;
    m_totalLength= 0;
    m_identification= 0;
    m_flags= 0;
    m_fragmentOffset= 0;
    m_ttl= 0;
    m_protocol= protocol;
    m_checksum= 0;
    m_sourceAddress= 0;
    m_destinationAddress= 0;
    m_optionsLength = 0;
    m_options = NULL;
    m_isTruncated=false;
}

IPPacket::IPPacket(const IPPacket &right)
    :CEthernet(right)
{
    m_version=right.m_version;
    m_headerLength = right.m_headerLength;
    m_diffServ = right.m_diffServ;
    m_totalLength = right.m_totalLength;
    m_identification = right.m_identification;
    m_flags = right.m_flags;
    m_fragmentOffset = right.m_fragmentOffset;
    m_ttl = right.m_ttl;
    m_protocol = right.m_protocol;
    m_ttl = right.m_ttl;
    m_checksum = right.m_checksum;
    m_sourceAddress = right.m_sourceAddress;
    m_destinationAddress = right.m_destinationAddress;
    m_optionsLength = right.m_optionsLength;
    m_isTruncated = right.m_isTruncated;
    if (m_options)
    {
        delete[] m_options;
    }
    m_options = new char[m_optionsLength];
    if (m_options)
    {
        ::memcpy(m_options,right.m_options,m_optionsLength);
    }

}

IPPacket &IPPacket::operator=(const IPPacket &right)
{
    CEthernet::operator=(right);
    m_version=right.m_version;
    m_headerLength = right.m_headerLength;
    m_diffServ = right.m_diffServ;
    m_totalLength = right.m_totalLength;
    m_identification = right.m_identification;
    m_flags = right.m_flags;
    m_fragmentOffset = right.m_fragmentOffset;
    m_ttl = right.m_ttl;
    m_protocol = right.m_protocol;
    m_ttl = right.m_ttl;
    m_checksum = right.m_checksum;
    m_sourceAddress = right.m_sourceAddress;
    m_destinationAddress = right.m_destinationAddress;
    m_optionsLength = right.m_optionsLength;
    m_isTruncated = right.m_isTruncated;
    if (m_options)
    {
        delete[] m_options;
    }
    m_options = new char[m_optionsLength];
    if (m_options)
    {
        ::memcpy(m_options,right.m_options,m_optionsLength);
    }


    return *this;
}

IPPacket::~IPPacket()
{
    if (m_options)
    {
        delete[] m_options;
    }
}

CAWResult IPPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    if (CEthernet::StreamFrom(is) != CAW_OK)
    {
        return CAW_ERROR_FAILURE;
    }

    uint8_t versionheader=0;
    is>>versionheader;
    m_headerLength = (uint8_t) (versionheader & 0xf);
    m_version = (uint8_t) (versionheader >> 4 & 0xf);
    is>>m_diffServ;
    is>>m_totalLength;
    is>>m_identification;
    uint16_t sscratch=0;
    is>>sscratch;
    m_flags = (uint8_t) (sscratch >> 13 & 0x7);
    m_fragmentOffset = (uint8_t) (sscratch & 0x1fff);
    is>>m_ttl;
    is>>m_protocol;
    is>>m_checksum;
    is>>m_sourceAddress;
    is>>m_destinationAddress;
    
    if (m_headerLength > 5) {
        m_optionsLength = (m_headerLength - 5) * 4;
        if (m_options)
        {
            delete[] m_options;
        }
        m_options = new char[m_optionsLength];
        if (m_options)
        {
            is.Read(m_options,m_optionsLength);
        }
    }

    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;
}


CAWResult IPPacket::StreamTo(CAWByteStreamNetwork& os)
{
    if (CEthernet::StreamTo(os) != CAW_OK)
    {
        return CAW_ERROR_FAILURE;
    }

    int optionsheader = 0;
    if (m_options != NULL) {
        optionsheader = m_optionsLength / 4;
    }
    m_headerLength = (uint8_t) (5 + optionsheader);
    m_totalLength = GetStreamLen()-CEthernet::GetStreamLen();

    if (m_checksum == 0)
        m_checksum=CheckSum();

    os<<(uint8_t) (((m_version & 0xf) << 4) | (m_headerLength & 0xf));
    os<<m_diffServ;
    os<<m_totalLength;
    os<<m_identification;
    os<<((uint16_t) ((m_flags & 0x7) << 13 | m_fragmentOffset & 0x1fff));
    os<<m_ttl;
    os<<m_protocol;
    os<<m_checksum;
    os<<m_sourceAddress;
    os<<m_destinationAddress;
    if ((m_options != NULL) && (m_optionsLength != 0)) 
    {
        os.Write(m_options, m_optionsLength);
    }

    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;
}

CAWResult IPPacket::FromJson(Json::Value& jsonin)
{    
    CAW_INFO_TRACE_THIS("IPPacket::FromJson");

    CEthernet::FromJson(jsonin);
    if (!jsonin[IPPACKET_JSON_ETH_PAYLOAD].isNull())
    {
        Json::Value &payloadjson = jsonin[IPPACKET_JSON_ETH_PAYLOAD];
        ValueFromJson(payloadjson, IPPACKET_JSON_VERSION, m_version);
        ValueFromJson(payloadjson, IPPACKET_JSON_HEADERLENGTH, m_headerLength);
        ValueFromJson(payloadjson, IPPACKET_JSON_DIFFSERV, m_diffServ);
        ValueFromJson(payloadjson, IPPACKET_JSON_TOTALLENGTH, m_totalLength);
        ValueFromJson(payloadjson, IPPACKET_JSON_IDENTIFICATION, m_identification);
        ValueFromJson(payloadjson, IPPACKET_JSON_FLAGS, m_flags);
        ValueFromJson(payloadjson, IPPACKET_JSON_FRAGMENTOFFSET, m_fragmentOffset);
        ValueFromJson(payloadjson, IPPACKET_JSON_TTL, m_ttl);
        ValueFromJson(payloadjson, IPPACKET_JSON_PROTOCOL, m_protocol);
        ValueFromJson(payloadjson, IPPACKET_JSON_CHECKSUM, m_checksum);
        IPv4FromJson(payloadjson, IPPACKET_JSON_SOURCEADDRESS, m_sourceAddress);
        IPv4FromJson(payloadjson, IPPACKET_JSON_DESTIONADDRESS, m_destinationAddress); 
     }
    return CAW_OK;
}

CAWResult IPPacket::ToJson(Json::Value& jsonout)
{
        CAW_INFO_TRACE_THIS("IPPacket::ToJson");
    CEthernet::ToJson(jsonout);
    Json::Value payloadjson;
    payloadjson[IPPACKET_JSON_VERSION]=ValueToJson(m_version);
    payloadjson[IPPACKET_JSON_HEADERLENGTH]=ValueToJson(m_headerLength);
    payloadjson[IPPACKET_JSON_DIFFSERV]=ValueToJson(m_diffServ);
    payloadjson[IPPACKET_JSON_TOTALLENGTH]=ValueToJson(m_totalLength);
    payloadjson[IPPACKET_JSON_IDENTIFICATION]=ValueToJson(m_identification);
    payloadjson[IPPACKET_JSON_FLAGS]=ValueToJson(m_flags);
    payloadjson[IPPACKET_JSON_FRAGMENTOFFSET]=ValueToJson(m_fragmentOffset);
    payloadjson[IPPACKET_JSON_TTL]=ValueToJson(m_ttl);
    payloadjson[IPPACKET_JSON_PROTOCOL]=ValueToJson(m_protocol);
    payloadjson[IPPACKET_JSON_CHECKSUM]=ValueToJson(m_checksum);
    payloadjson[IPPACKET_JSON_SOURCEADDRESS]=IPv4ToJson(m_sourceAddress);
    payloadjson[IPPACKET_JSON_DESTIONADDRESS]=IPv4ToJson(m_destinationAddress);

    jsonout[IPPACKET_JSON_ETH_PAYLOAD]=payloadjson;
    return CAW_OK;

}


uint16_t IPPacket::GetStreamLen() const
{
    return CEthernet::GetStreamLen()+20+m_optionsLength;
}

uint16_t IPPacket::GetEncodeSpace()   const
{
    return CEthernet::GetEncodeSpace()+20+m_optionsLength;
}

void IPPacket::Dump() const
{
/*
    CAW_INFO_TRACE("IPPacket::Dump()");
    CAW_INFO_TRACE("version=" << version 
                    << "headerLength=" << headerLength 
                    << "diffServ=" << diffServ 
                    << "totalLength=" << totalLength
                    << "identification=" << identification
                    << "flags=" << flags
                    << "fragmentOffset=" << fragmentOffset
                    << "ttl=" << ttl
                    << "protocol=" << protocol
                    << "sourceAddress=" << sourceAddress
                    << "destinationAddress=" << destinationAddress
                    << "options=" << *options);
                    */
}


uint16_t IPPacket::CheckSum()
{
    //std::cout << "IPPacket::TCPCheckSum" << std::endl;
    uint16_t tochecksum=0;
    int optionsheader = 0;
    if (m_options != NULL) {
        optionsheader = m_optionsLength / 4;
    }
    m_headerLength = (uint8_t) (5 + optionsheader);
    m_totalLength = GetStreamLen()-CEthernet::GetStreamLen();;

    
    CAWMessageBlock msgblk(m_totalLength);
    
    CAWByteStreamNetwork checksumos(msgblk);

    checksumos<<(uint8_t) (((m_version & 0xf) << 4) | (m_headerLength & 0xf));
    checksumos<<m_diffServ;
    checksumos<<m_totalLength;
    checksumos<<m_identification;
    checksumos<<((uint16_t) ((m_flags & 0x7) << 13 | m_fragmentOffset & 0x1fff));
    checksumos<<m_ttl;
    checksumos<<m_protocol;
    checksumos<<m_checksum;
    checksumos<<m_sourceAddress;
    checksumos<<m_destinationAddress;
    if ((m_options != NULL) && (m_optionsLength != 0)) 
    {
        checksumos.Write(m_options, m_optionsLength);
    }


    int accumulation = 0;
    for (int i = 0; i < m_headerLength * 2; ++i) {
        uint16_t temp=0;
        checksumos>>temp;
        accumulation += 0xffff & temp;
    }
    accumulation = (accumulation >> 16 & 0xffff)
            + (accumulation & 0xffff);
    tochecksum = (uint16_t) (~accumulation & 0xffff);

    return htons(tochecksum);
}

void IPPacket::SetOption(char *data, size_t datasize)
{
    m_optionsLength=datasize;
    if (m_options)
    {
        delete[] m_options;
    }
    m_options = new char[m_optionsLength];
    if (m_options)
    {
        ::memcpy(m_options,data, m_optionsLength);
    }
}

void IPPacket::GetOption(char *&data, size_t &datasize)
{
    data=m_options;
    datasize=m_optionsLength;
}
uint8_t IPPacket::getVersion() {
    return m_version;
}

void IPPacket::setVersion(uint8_t ver) {
    m_version = ver;
}
uint8_t IPPacket::getHeaderLength() {
    return m_headerLength;
}
uint8_t IPPacket::getDscp() {
    return (uint8_t) ((m_diffServ >> DSCP_OFFSET) & DSCP_MASK);
}
void IPPacket::setDscp(uint8_t dscp) {
    m_diffServ &= ~(DSCP_MASK << DSCP_OFFSET);
    m_diffServ |= (dscp & DSCP_MASK) << DSCP_OFFSET;
}
uint8_t IPPacket::getEcn() {
    return (uint8_t) (m_diffServ & ECN_MASK);
}

void IPPacket::setEcn(uint8_t ecn) {
    m_diffServ &= ~ECN_MASK;
    m_diffServ |= (ecn & ECN_MASK);
}
uint8_t IPPacket::getDiffServ() {
    return m_diffServ;
}
void IPPacket::setDiffServ(uint8_t diffS) {
    m_diffServ = diffS;
}
uint16_t IPPacket::getTotalLength() {
    return m_totalLength;
}
uint16_t IPPacket::getIdentification() {
    return m_identification;
}
bool IPPacket::isTruncated() {
    return m_isTruncated;
}
void IPPacket::setTruncated(bool isTruncated) {
    m_isTruncated = isTruncated;
}
uint8_t IPPacket::getFlags() {
    return m_flags;
}
void IPPacket::setFlags(uint8_t f) {
    m_flags = f;
}
uint16_t IPPacket::getFragmentOffset() {
    return m_fragmentOffset;
}
void IPPacket::setFragmentOffset(uint16_t frag) {
    m_fragmentOffset = frag;
}
uint8_t IPPacket::getTtl() {
    return m_ttl;
}
void IPPacket::setTtl(uint8_t ttlvalue) {
    m_ttl = ttlvalue;
}
uint8_t IPPacket::getProtocol() {
    return m_protocol;
}
void IPPacket::setProtocol(uint8_t protocolvlaue) {
    m_protocol = protocolvlaue;
}
uint16_t IPPacket::getChecksum() {
    return m_checksum;
}
void IPPacket::setChecksum(uint16_t sum) {
    m_checksum = sum;
}
void IPPacket::resetChecksum() {
    m_checksum = 0;
}
uint32_t IPPacket::getSourceAddress() {
    return m_sourceAddress;
}
void IPPacket::setSourceAddress(uint32_t sourceaddr) {
    m_sourceAddress = sourceaddr;
}
uint32_t IPPacket::getDestinationAddress() {
    return m_destinationAddress;
}
void IPPacket::setDestinationAddress(uint32_t destinationaddress) {
    m_destinationAddress = destinationaddress;
}



