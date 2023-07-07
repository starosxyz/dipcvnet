/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _CUNKNOWNEthernet_H
#define _CUNKNOWNEthernet_H

#include <packet/Ethernet.h>

/*
  @类名:UnknownEthernet
  @功能:对以太网协议的管理
*/

class CAW_OS_EXPORT UnknownEthernet : public CEthernet
{
public:
    UnknownEthernet();
    UnknownEthernet(uint16_t type);
    UnknownEthernet(const UnknownEthernet& right);
    UnknownEthernet& operator=(const UnknownEthernet& right);
    virtual ~UnknownEthernet();
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

