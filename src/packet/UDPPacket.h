/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _UDPPACKET_H
#define _UDPPACKET_H
#include <packet/IPPacket.h>


#define CPACKET_JSON_SPORT "sourceport"
#define CPACKET_JSON_DPORT "destinationport"
#define CPACKET_JSON_CHECKSUM "udpchecksum"
#define CPACKET_JSON_PAYLOAD_SIZE "payloadsize"

#define CPACKET_JSON_UDPPAYLOAD "udp"

static const uint16_t DHCP_SERVER_PORT = 67;
static const uint16_t DHCP_CLIENT_PORT = 68;

static const uint16_t UDP_HEADER_LENGTH = 8;


class CAW_OS_EXPORT UDPPacket : public IPPacket
{
public:
    UDPPacket();
    virtual ~UDPPacket();
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;
    virtual void Dump() const;
    void SetSourcePort(uint16_t port);
    void SetDestinationPort(uint16_t port);
    uint16_t GetSourcePort();
    uint16_t GetDestinationPort();
    virtual uint16_t UDPCheckSum()=0;
public:
    uint16_t m_sourcePort;
    uint16_t m_destinationPort;
    uint16_t m_udplength;
    uint16_t m_udpchecksum;
};


#endif //_ETHERNET_H

