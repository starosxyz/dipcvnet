/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _CUNKNOWNTCPPACKET_H
#define _CUNKNOWNTCPPACKET_H

#include <packet/TCPPacket.h>

class CAW_OS_EXPORT UnknownTCPPacket : public TCPPacket
{
public:
    UnknownTCPPacket();
    UnknownTCPPacket(const UnknownTCPPacket& right);
    UnknownTCPPacket& operator=(const UnknownTCPPacket& right);
    virtual ~UnknownTCPPacket();
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;
    virtual void Dump() const;
    virtual uint16_t TCPCheckSum();
    virtual uint16_t TCPCheckSumv6();
public:
    size_t SetAppPayload(char *data, size_t datasize);
    void GetAppPayload(char *&payload, size_t &payloadsize);

public:
    char m_etherpayload[ETHERNET_MAX_SIZE];
    size_t m_etherpayloadsize;
};

#endif //_CUNKNOWNTCPPACKET_H

