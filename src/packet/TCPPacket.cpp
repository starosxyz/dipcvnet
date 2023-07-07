/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/TCPPacket.h>
#define TCP_MAX_SIZE 1200


TCPPacket::TCPPacket()
    :m_tcpsourcePort(0)
     ,m_tcpdestinationPort(0)
     ,m_tcpsequence(0)
     ,m_tcpacknowledge(0)
     ,m_tcpdataOffset(0)
     ,m_tcpflags(0)
     ,m_tcpwindowSize(0)
     ,m_tcpchecksum(0)
     ,m_tcpurgentPointer(0)
     ,m_tcpoptions(NULL)
     ,m_tcpoptionssize(0)
{

}

TCPPacket::~TCPPacket()
{
    if (m_tcpoptions)
    {
        delete[] m_tcpoptions;
        m_tcpoptions=NULL;
    }
}

CAWResult TCPPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    CAWResult result = CAW_OK;
    result = IPPacket::StreamFrom(is);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("TCPPacket::StreamFromHeader fail");
        result = CAW_ERROR_FAILURE;
    }
     else
     {
         is>>m_tcpsourcePort;
         m_tcpsourcePort=m_tcpsourcePort & 0xffff;
         is>>m_tcpdestinationPort;
         m_tcpdestinationPort=m_tcpdestinationPort & 0xffff;
         is>>m_tcpsequence;
         is>>m_tcpacknowledge;
         is>>m_tcpflags;
         m_tcpdataOffset=(uint8_t) (m_tcpflags >> 12 & 0xf);
         m_tcpflags = (uint16_t) (m_tcpflags & 0x1ff);
         is>>m_tcpwindowSize;
         is>>m_tcpchecksum;
         is>>m_tcpurgentPointer;

         if (m_tcpdataOffset > 5) {
             m_tcpoptionssize = (m_tcpdataOffset << 2) - 20;
             if (m_tcpoptions)
             {
                delete[] m_tcpoptions;
             }
             m_tcpoptions = new char[m_tcpoptionssize];
             if (m_tcpoptions)
             {
                is.Read(m_tcpoptions, m_tcpoptionssize);
             }
         }

         bool bGood = is.IsGood();
         CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
     }

     return CAW_OK;

 }

 CAWResult TCPPacket::StreamTo(CAWByteStreamNetwork& os)
{

    CAWResult result = CAW_OK;
    result = IPPacket::StreamTo(os);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("TCPPacket::StreamToHeader fail");
        result = CAW_ERROR_FAILURE;
    }
    else
    {
        if (m_tcpdataOffset == 0) {
            m_tcpdataOffset = 5; // default header length
        }
        if (m_tcpchecksum == 0)
            m_tcpchecksum=TCPCheckSum();
        os<<(uint16_t) (m_tcpsourcePort & 0xffff);
        os<<(uint16_t) (m_tcpdestinationPort & 0xffff);
        os<<m_tcpsequence;
        os<<m_tcpacknowledge;
        os<<(uint16_t)(m_tcpflags | m_tcpdataOffset << 12);
        os<<m_tcpwindowSize;
        os<<m_tcpchecksum;
        os<<m_tcpurgentPointer;

        if (m_tcpdataOffset > 5) {
            if ((m_tcpoptionssize>0) && (m_tcpoptions != NULL))
            {
                os.Write(m_tcpoptions, m_tcpoptionssize);
            }
            int padding=0;
            padding = (m_tcpdataOffset << 2) - 20 - m_tcpoptionssize;
            for (int i = 0; i < padding; i++) {
                uint8_t data=0;
                os<<data;
            }
        }

        bool bGood = os.IsGood();
        CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    }

    return CAW_OK;
}
CAWResult TCPPacket::FromJson(Json::Value& jsonin)
{
    IPPacket::FromJson(jsonin);
    if (!jsonin[TCPPACKET_JSON_TCPPAYLOAD].isNull())
    {
        Json::Value &payloadjsonin = jsonin[TCPPACKET_JSON_TCPPAYLOAD];
        ValueFromJson(payloadjsonin, TCPPACKET_JSON_SOURCEPORT, m_tcpsourcePort);
        ValueFromJson(payloadjsonin, TCPPACKET_JSON_DESTINATIONPORT, m_tcpdestinationPort);
        ValueFromJson(payloadjsonin, TCPPACKET_JSON_SEQUENCE, m_tcpsequence);
        ValueFromJson(payloadjsonin, TCPPACKET_JSON_ACKNOWLEDGE, m_tcpacknowledge);
        ValueFromJson(payloadjsonin, TCPPACKET_JSON_DATAOFFSET, m_tcpdataOffset);
        ValueFromJson(payloadjsonin, TCPPACKET_JSON_FLAGS, m_tcpflags);
        ValueFromJson(payloadjsonin, TCPPACKET_JSON_WINDOWSIZE, m_tcpwindowSize);
        ValueFromJson(payloadjsonin, TCPPACKET_JSON_CHECJSUM, m_tcpchecksum);
        ValueFromJson(payloadjsonin, TCPPACKET_JSON_URGENTPOINTER, m_tcpurgentPointer);
     }

    return CAW_OK;
}
CAWResult TCPPacket::ToJson(Json::Value& jsonout)
{
    IPPacket::ToJson(jsonout);
    Json::Value tcppayloadjson;
    if (m_tcpchecksum == 0)
        m_tcpchecksum=TCPCheckSum();

    tcppayloadjson[TCPPACKET_JSON_SOURCEPORT]=ValueToJson(m_tcpsourcePort);
    tcppayloadjson[TCPPACKET_JSON_DESTINATIONPORT]=ValueToJson(m_tcpdestinationPort);
    tcppayloadjson[TCPPACKET_JSON_SEQUENCE]=ValueToJson(m_tcpsequence);
    tcppayloadjson[TCPPACKET_JSON_ACKNOWLEDGE]=ValueToJson(m_tcpacknowledge);
    tcppayloadjson[TCPPACKET_JSON_DATAOFFSET]=ValueToJson(m_tcpdataOffset);
    tcppayloadjson[TCPPACKET_JSON_FLAGS]=ValueToJson(m_tcpflags);
    tcppayloadjson[TCPPACKET_JSON_WINDOWSIZE]=ValueToJson(m_tcpwindowSize);
    tcppayloadjson[TCPPACKET_JSON_CHECJSUM]=ValueToJson(m_tcpchecksum);
    tcppayloadjson[TCPPACKET_JSON_URGENTPOINTER]=ValueToJson(m_tcpurgentPointer);

    
    jsonout[TCPPACKET_JSON_TCPPAYLOAD]=tcppayloadjson;
    
    return CAW_OK;
}

uint16_t TCPPacket::GetStreamLen() const
{

    return IPPacket::GetStreamLen() + 20 +m_tcpoptionssize ;


}

uint16_t TCPPacket::GetEncodeSpace() const
{
   
   return IPPacket::GetEncodeSpace() +20+m_tcpoptionssize ;
}  

void TCPPacket::Dump() const{
   IPPacket::Dump();
}
uint16_t TCPPacket::getSourcePort() {
   return m_tcpsourcePort;
 }
void TCPPacket::setSourcePort(uint16_t sourcePort) {
    m_tcpsourcePort = sourcePort;
}
uint16_t TCPPacket::getDestinationPort() {
    return m_tcpdestinationPort;
}   
void TCPPacket::setDestinationPort(uint16_t destinationPort) {
    m_tcpdestinationPort = destinationPort;
}
uint32_t TCPPacket::getSequence() {
    return m_tcpsequence;
}
void TCPPacket::setSequence(uint32_t sequence) {
    m_tcpsequence = sequence;
}
uint32_t TCPPacket::getAcknowledge() {
    return m_tcpacknowledge;
}
void TCPPacket::setAcknowledge(uint32_t acknowledge) {
    m_tcpacknowledge = acknowledge;
}
uint8_t TCPPacket::getDataOffset() {
    return m_tcpdataOffset;
}
void TCPPacket::setDataOffset(uint8_t dataOffset) {
    m_tcpdataOffset = dataOffset;
}
uint16_t TCPPacket::getFlags() {
    return m_tcpflags;
}
void TCPPacket::setFlags(uint16_t flags) {
    m_tcpflags = flags;
}
uint16_t TCPPacket::getWindowSize() {
    return m_tcpwindowSize;
}
void TCPPacket::setWindowSize(uint16_t windowSize) {
    m_tcpwindowSize = windowSize;
}
uint16_t TCPPacket::getChecksum() {
    return m_tcpchecksum;
}
void TCPPacket::setChecksum(uint16_t checksum) {
    m_tcpchecksum = checksum;
}
uint16_t TCPPacket::getUrgentPointer() {
    return m_tcpurgentPointer;
}
void TCPPacket::setUrgentPointer(uint16_t urgentPointer) {
    m_tcpurgentPointer = urgentPointer;
}
 
void TCPPacket::SetTCPOption(char *data, size_t datasize)
{
    if (m_tcpoptions)
    {
        delete[] m_tcpoptions;
    }
    m_tcpoptions = new char[datasize];
    if (m_tcpoptions)
    {
        ::memcpy(m_tcpoptions, data, datasize);
        m_tcpoptionssize=datasize;
        m_tcpdataOffset = (uint8_t) (20 + datasize + 3 >> 2);
    }
}
void TCPPacket::GetTCPOption(char *&data, size_t &datasize)
{
    data=m_tcpoptions;
    datasize=m_tcpoptionssize;
}



