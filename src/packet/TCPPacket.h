/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _TCPPACKET_H
#define _TCPPACKET_H
#include <packet/IPPacket.h>


#define TCPPACKET_JSON_SOURCEPORT "sourceport"
#define TCPPACKET_JSON_DESTINATIONPORT "destinationport"
#define TCPPACKET_JSON_SEQUENCE "sequence"
#define TCPPACKET_JSON_ACKNOWLEDGE "acknowledge"
#define TCPPACKET_JSON_DATAOFFSET "dataoffset"
#define TCPPACKET_JSON_FLAGS "flags"
#define TCPPACKET_JSON_WINDOWSIZE "windowsize"
#define TCPPACKET_JSON_CHECJSUM "tcpchecksum"
#define TCPPACKET_JSON_URGENTPOINTER "urgentpointer"

#define TCPPACKET_JSON_TCPPAYLOAD "tcp"

static const uint16_t TCP_HEADER_LENGTH = 20;

class CAW_OS_EXPORT TCPPacket : public IPPacket
{
public:
    TCPPacket();
    virtual ~TCPPacket();
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;
    virtual uint16_t TCPCheckSum()=0;
    virtual uint16_t TCPCheckSumv6()=0;

    virtual void Dump() const;
    uint16_t getSourcePort();
    void setSourcePort(uint16_t sourcePort);
    uint16_t getDestinationPort();
    void setDestinationPort(uint16_t destinationPort);
    uint32_t getSequence();
    void setSequence(uint32_t sequence);
    uint32_t getAcknowledge();
    void setAcknowledge(uint32_t acknowledge);
    uint8_t getDataOffset();
    void setDataOffset(uint8_t dataOffset);
    uint16_t getFlags();
    void setFlags(uint16_t flags);
    uint16_t getWindowSize();
    void setWindowSize(uint16_t windowSize);
    uint16_t getChecksum();
    void setChecksum(uint16_t checksum);
    uint16_t getUrgentPointer();
    void setUrgentPointer(uint16_t urgentPointer);

    void SetTCPOption(char *data, size_t datasize);
    void GetTCPOption(char *&data, size_t &datasize);
    size_t GetTCPOptionSize(){return m_tcpoptionssize;}
public:
    uint16_t m_tcpsourcePort;
    uint16_t m_tcpdestinationPort;
    uint32_t m_tcpsequence;
    uint32_t m_tcpacknowledge;
    uint8_t m_tcpdataOffset;
    uint16_t m_tcpflags;
    uint16_t m_tcpwindowSize;
    uint16_t m_tcpchecksum;
    uint16_t m_tcpurgentPointer;
    char *m_tcpoptions;
    size_t m_tcpoptionssize;
};


#endif //_ETHERNET_H

