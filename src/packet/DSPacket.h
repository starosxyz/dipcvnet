/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _DSPACKET_H
#define _DSPACKET_H
#include <packet/Ethernet.h>


#define DSPACKET_JSON_LENGTH "length"
#define DSPACKET_JSON_SENDERDP "senderdp"
#define DSPACKET_JSON_SENDERPORT "senderport"

/*
  @类名:DSPacket
  @功能:对DSPacket包的管理
*/

class CAW_OS_EXPORT DSPacket : public CEthernet
{
public:
    DSPacket(    );
    virtual ~DSPacket();
    CAWResult StreamFrom(CAWByteStreamNetwork& is);
    CAWResult StreamTo(CAWByteStreamNetwork& os);
    CAWResult FromJson(Json::Value& jsonin);
    CAWResult ToJson(Json::Value& jsonout);
    uint16_t GetStreamLen() const;
    uint16_t GetEncodeSpace()   const;
    void Dump() const;

    void SetSenderDP(uint64_t fromdp);
    void SetSenderPort(uint32_t todp);

    uint64_t GetSenderDP();
    uint32_t GetSenderPort();

    void SetPeerDP(uint64_t dpid);
    void SetPeerPort(uint32_t peerport);

    uint64_t GetPeerDP();
    uint32_t GetPeerPort();

    
    void SetFlags(uint32_t flags);
    uint32_t GetFlags();


    DSPacket(const DSPacket& right);

    DSPacket& operator=(const DSPacket& right);

    void SetFromDataCenterId(uint32_t datacentid);
    void SetFromClusterId(uint32_t clusterid);

    uint32_t GetFromDataCenterId();
    uint32_t GetFromClusterId();

    void SetToDataCenterId(uint32_t datacentid);
    void SetToClusterId(uint32_t clusterid);

    uint32_t GetToDataCenterId();
    uint32_t GetToClusterId();

    void SetSendCost(uint32_t cost);
    uint32_t GetSendCost();

public:
    uint32_t m_length;
    uint32_t m_flags;
    uint32_t m_senderport;
    uint32_t m_peerport;
    uint64_t m_senderdp;
    uint64_t m_peerdp;
    uint32_t m_sendcost;
    uint32_t m_fromdatacenterid;
    uint32_t m_fromclusterid;
    uint32_t m_todatacenterid;
    uint32_t m_toclusterid;
};


#endif //_ETHERNET_H
