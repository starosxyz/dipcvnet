/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef STPPACKET_H
#define STPPACKET_H
#include <packet/Ethernet.h>

class CAW_OS_EXPORT STPPacket : public CEthernet
{
public:
    STPPacket();
    virtual ~STPPacket();
    CAWResult StreamFrom(CAWByteStreamNetwork& is);
    CAWResult StreamTo(CAWByteStreamNetwork& os);
    CAWResult FromJson(Json::Value& jsonin);
    CAWResult ToJson(Json::Value& jsonout);
    uint16_t GetStreamLen() const;
    uint16_t GetEncodeSpace()   const;
    void Dump() const;

    void SetPayload(const char *pdata, size_t datasize);
    void GetPlayload(char* &pdata, size_t &datasize);


    STPPacket(const STPPacket& right);

    STPPacket& operator=(const STPPacket& right);
    virtual STPPacket* ClonePdu();
    void DestroyClone();
public:
    uint16_t m_datasize;
    char* m_pdata;
};

#endif //_ETHERNET_H
