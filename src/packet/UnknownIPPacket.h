/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _CUNKNOWNIPPACKET_H
#define _CUNKNOWNIPPACKET_H

#include <packet/IPPacket.h>

/*
  @类名:UnknownTCPPacket
  @功能:对TCP下层协议的管理
*/

class CAW_OS_EXPORT UnknownIPPacket : public IPPacket
{
public:
    UnknownIPPacket();
    UnknownIPPacket(uint8_t protocol);
    UnknownIPPacket(const UnknownIPPacket& right);
    UnknownIPPacket& operator=(const UnknownIPPacket& right);
    virtual ~UnknownIPPacket();
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;
    virtual void Dump() const;
public:
    size_t SetAppPayload(char *data, size_t datasize);
    void GetAppPayload(char *&payload, size_t &payloadsize);

public:
    char m_etherpayload[ETHERNET_MAX_SIZE];
    size_t m_etherpayloadsize;
};

#endif //_CUNKNOWNEthernet_H

