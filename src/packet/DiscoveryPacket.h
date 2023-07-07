/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/

#ifndef _DISCOVERYPACKET_H
#define _DISCOVERYPACKET_H
#include <packet/IPacketParser.h>
#define DISCOVERY_JSON_FROM "fromdp"
#define DISCOVERY_JSON_TO "todp"
#define DISCOVERY_JSON_XID "xid"
#define DISCOVERY_JSON_LINKLENTH "linklength"
#define DISCOVERY_JSON_PATHS "paths"

#define DISCOVERY_JSON_LINK_SRC "src"
#define DISCOVERY_JSON_LINK_DST "dst"
#define DISCOVERY_JSON_CONNECTPOINT_DP "dpid"
#define DISCOVERY_JSON_CONNECTPOINT_PORT "port"


/*
{
    "xid":"1",
    "path":[
        {
            "src":{
                "dpid":"2222",
                "port":"2"
            }
            "dst":{
                "dpid":"444",
                "port":"2"
            }
        },
        {
            "src":{
                "dpid":"444",
                "port":"3"
            }
            "dst":{
                "dpid":"5555",
                "port":"4"
            }
        }
    ]
}
*/

struct DiscoveryConnectpoint 
{
    uint64_t dpit;
    uint32_t port;
    uint32_t pad;
};

struct Discoverylink 
{
    DiscoveryConnectpoint src;
    DiscoveryConnectpoint dst;
};

struct DiscoveryStruct
{
    uint16_t length;
    uint16_t linklength;
    uint32_t xid;
    uint64_t from;
    uint64_t to;
};

class CAW_OS_EXPORT DiscoveryPacket : public IPacketParser
{
public:
    DiscoveryPacket(    );
    virtual ~DiscoveryPacket();
    CAWResult StreamFrom(CAWByteStreamNetwork& is);
    CAWResult StreamTo(CAWByteStreamNetwork& os);
    CAWResult FromJson(Json::Value& jsonin);
    CAWResult FromJson2(Json::Value& jsonin);
    CAWResult ToJson(Json::Value& jsonout);
    CAWResult ToJson2(Json::Value& jsonout);
    uint16_t GetStreamLen() const;
    uint16_t GetEncodeSpace()   const;
    void Dump() const;
    void SetJson(const CAWString &strjson);
    CAWString GetJson();

    void AddLink(Discoverylink &link);
    void GetLinks(std::list<Discoverylink> &list);
    void SetFrom(uint64_t fromdp);
    void SetTo(uint64_t todp);
    void SetXid(uint32_t xid);
    uint64_t GetFrom();
    uint64_t GetTo();
    uint32_t GetXid();
    void ModifyLastDpPort(uint32_t port, uint64_t dp);
    bool IsHaveTheDP(uint64_t dpid);
    bool DpIsEnd(uint64_t dpid);
    CAWResult AddNewLink(uint32_t port, uint64_t dpid, uint32_t cost);
    size_t GetLinkSize();
    DiscoveryPacket(const DiscoveryPacket& right);
    void GetLoopList(uint64_t dpid, std::list<Discoverylink> &lists);

    DiscoveryPacket& operator=(const DiscoveryPacket& right);


public:
    uint16_t length;
    uint16_t linklength;
    uint32_t xid;
    uint64_t from;
    uint64_t to;
    std::list<Discoverylink> m_links;
};

#endif //_ETHERNET_H
