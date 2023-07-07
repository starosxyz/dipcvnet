/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/ICMPPacket.h>
#include <packet/checksum.h>


ICMPPacket::ICMPPacket()
    :IPPacket(1)
{
    m_icmpType = 0;
    m_icmpCode = 0;
    m_icmpchecksum = 0;
    m_icmpidentifier = 0;
    m_icmpsequencnum = 0;
    m_etherpayloadsize = 0;
    ::memset(m_etherpayload, 0, sizeof(m_etherpayload));
}

ICMPPacket::~ICMPPacket()
{

}

/**
 * @return the icmpType
 */
uint8_t ICMPPacket::getIcmpType() {
    return m_icmpType;
}

void ICMPPacket::setIcmpType(uint8_t icmptype) {
    m_icmpType = icmptype;
}

/**
 * @return the icmp code
 */
uint8_t ICMPPacket::getIcmpCode() {
    return m_icmpCode;
}

/**
 * @param icmpCode code to set
 * @return this
 */
void ICMPPacket::setIcmpCode(uint8_t icmpcode) {
    m_icmpCode = icmpcode;
}

/**
 * @return the checksum
 */
uint16_t ICMPPacket::getChecksum() {
    return m_icmpchecksum;
}

    /**
     * @param checksum the checksum to set
     * @return this
     */
void ICMPPacket::setChecksum(uint16_t checksum) {
    m_icmpchecksum = checksum;
}

CAWResult ICMPPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    IPPacket::StreamFrom(is);
    is>>m_icmpType;
    is>>m_icmpCode;
    is>>m_icmpchecksum;
    is>>m_icmpidentifier;
    is>>m_icmpsequencnum;
    if(m_totalLength<28+m_optionsLength)
    {
        return CAW_ERROR_FAILURE;
    }
    if (m_totalLength==28+m_optionsLength)
    {
        return CAW_OK;
    }
    m_etherpayloadsize = m_totalLength-28-m_optionsLength;
    if (m_etherpayloadsize>1500)
    {
        return CAW_ERROR_FAILURE;
    }
    is.Read(m_etherpayload, m_etherpayloadsize);

    return CAW_OK;
}

CAWResult ICMPPacket::StreamTo(CAWByteStreamNetwork& os)
{
    CAW_INFO_TRACE_THIS("ICMPPacket::StreamTo, m_icmppayloadsize="<<m_etherpayloadsize);

    size_t ossize = os.GetChainedCapacity();
    size_t ethsize = GetStreamLen();


    m_icmpchecksum=ICMPCheckSum();
    //CAW_INFO_TRACE_THIS("ICMPPacket::StreamToHeader, checksum="<<checksum);

    IPPacket::StreamTo(os);
    os<<m_icmpType;
    os<<m_icmpCode;
    os<<m_icmpchecksum;
    os<<m_icmpidentifier;
    os<<m_icmpsequencnum;

    ossize = os.GetChainedCapacity();

    if (m_etherpayloadsize != 0)
    {
        os.Write(m_etherpayload, m_etherpayloadsize);
    }
    BOOL bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;

}

CAWResult ICMPPacket::FromJson(Json::Value& jsonin)
{
    CEthernet::FromJson(jsonin);
    if (!jsonin[ICMP_JSON_PAYLOAD].isNull())
    {
        Json::Value &payloadjson = jsonin[ICMP_JSON_PAYLOAD];
        ValueFromJson(payloadjson, ICMP_JSON_TYPE, m_icmpType);
        ValueFromJson(payloadjson, ICMP_JSON_CODE, m_icmpCode);
        ValueFromJson(payloadjson, ICMP_JSON_CHECKSUM, m_icmpchecksum);
        ValueFromJson(payloadjson, ICMP_JSON_ID, m_icmpidentifier);
        ValueFromJson(payloadjson, ICMP_JSON_SEQ, m_icmpsequencnum);

     }

    return CAW_OK;
}

CAWResult ICMPPacket::ToJson(Json::Value& jsonout)
{
    CEthernet::ToJson(jsonout);

    Json::Value ippayloadjson;
    m_icmpchecksum=ICMPCheckSum();

    ippayloadjson[ICMP_JSON_TYPE]=ValueToJson(m_icmpType);
    ippayloadjson[ICMP_JSON_CODE]=ValueToJson(m_icmpCode);
    ippayloadjson[ICMP_JSON_CHECKSUM]=ValueToJson(m_icmpchecksum);
    ippayloadjson[ICMP_JSON_ID]=ValueToJson(m_icmpidentifier); 
    ippayloadjson[ICMP_JSON_SEQ]=ValueToJson(m_icmpsequencnum); 

    jsonout[ICMP_JSON_PAYLOAD]=ippayloadjson;

    return CAW_OK;
}
uint16_t ICMPPacket::ICMPCheckSum()
{
    //std::cout << "ICMPPacket::ICMPCheckSum" << std::endl;
    uint16_t tochecksum=0;
    uint16_t length = 8+m_etherpayloadsize;

    CAWMessageBlock msgblk(length);
    
    CAWByteStreamNetwork checksumos(msgblk);

    checksumos<<m_icmpType;
    checksumos<<m_icmpCode;
    checksumos<<tochecksum;
    checksumos<<m_icmpidentifier;
    checksumos<<m_icmpsequencnum;
    if (m_etherpayloadsize != 0)
    {
        checksumos.Write(m_etherpayload, m_etherpayloadsize);
    }
    uint16_t checksum= icmp_checksum((uint16_t*)msgblk.GetTopLevelReadPtr(), (int)msgblk.GetChainedLength());
    return htons(checksum);
}

uint16_t ICMPPacket::icmp_checksum(uint16_t *buffer,int size)
{
    unsigned long cksum=0;
    while(size>1)
    {
        cksum+=*buffer++;
        size-=sizeof(uint16_t);
    }
    if(size)
    {
        cksum+=*(uint8_t *)buffer;
    }
    cksum=(cksum>>16)+(cksum&0xffff);
    cksum+=(cksum>>16);
    return (uint16_t)(~cksum);
}

uint16_t ICMPPacket::GetStreamLen() const
{
    uint16_t size= 8+m_etherpayloadsize+IPPacket::GetStreamLen();
    //CAW_INFO_TRACE("ICMPPacket::GetStreamLen(), size="<<size);
    return size;
}

uint16_t ICMPPacket::GetEncodeSpace()   const
{
    uint16_t size= 8+m_etherpayloadsize+IPPacket::GetEncodeSpace();
    //CAW_INFO_TRACE("ICMPPacket::GetStreamLen(), size="<<size);
    return size;
}
void ICMPPacket::Dump() const
{
}
void ICMPPacket::SetIdentifier(uint16_t id)
{
    m_icmpidentifier=id;
}
uint16_t ICMPPacket::GetIdentifier()
{
    return m_icmpidentifier;
}
void ICMPPacket::SetSequencenum(uint16_t seq)
{
    m_icmpsequencnum = seq;
}

uint16_t ICMPPacket::GetSequencenum()
{
    return m_icmpsequencnum;
}
size_t  ICMPPacket::SetAppPayload(char *data, size_t datasize)
{
    if ((data == NULL) || (datasize==0))
    {
        return 0;
    }
    if (datasize>ETHERNET_MAX_SIZE)
    {
        m_etherpayloadsize=ETHERNET_MAX_SIZE;
    }
    else 
    {
        m_etherpayloadsize=datasize;
    }
    ::memcpy(m_etherpayload, data, m_etherpayloadsize);
    return m_etherpayloadsize;
}

void ICMPPacket::GetAppPayload(char *&payload, size_t &payloadsize)
{
    payload=m_etherpayload;
    payloadsize=m_etherpayloadsize;
}




