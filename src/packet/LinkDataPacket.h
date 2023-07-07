/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _LINKDATAPACKET_H
#define _LINKDATAPACKET_H
#include <packet/Ethernet.h>


#define LINK_DATA_MAX_SIZE 1400

class CAW_OS_EXPORT LinkDataPacket : public CEthernet
{
public:
    LinkDataPacket();
    virtual ~LinkDataPacket();
    CAWResult StreamFrom(CAWByteStreamNetwork& is);
    CAWResult StreamTo(CAWByteStreamNetwork& os);
    CAWResult FromJson(Json::Value& jsonin);
    CAWResult ToJson(Json::Value& jsonout);
    uint16_t GetStreamLen() const;
    uint16_t GetEncodeSpace()   const;
    void Dump() const;

    inline void SetDstDP(uint64_t dp) { m_dstdpid = dp; }
    inline void SetDstPort(uint32_t port) { m_dstport = port; }

    inline uint64_t GetSrcDP() const { return m_srcdpid; }
    inline uint32_t GetSrcPort() const { return m_srcport; }

    inline void SetSrcDP(uint64_t dpid) { m_srcdpid = dpid; }
    inline void SetSrcPort(uint32_t port) { m_srcport = port; }

    inline uint64_t GetDstDP() const { return m_dstdpid; }
    inline uint32_t GetDstPort() const { return m_dstport; }

    inline uint32_t GetXid() const { return m_xid; }
    inline void SetXid(uint32_t xid) { m_xid = xid; }

    inline uint16_t GetFlags() const { return m_flags; }
    inline void SetFlags(uint16_t flags) { m_flags = flags; }

    void SetPayload(const char *pdata, size_t datasize);
    void GetPlayload(char* &pdata, size_t &datasize);


    LinkDataPacket(const LinkDataPacket& right);

    LinkDataPacket& operator=(const LinkDataPacket& right);
    virtual LinkDataPacket* ClonePdu();
    void DestroyClone();
public:
    uint32_t m_dstport;
    uint32_t m_srcport;
    uint64_t m_dstdpid;
    uint64_t m_srcdpid;
    uint32_t m_xid;
    uint16_t m_flags;
    uint16_t m_datasize;
    char* m_pdata;
};


#endif //_ETHERNET_H
