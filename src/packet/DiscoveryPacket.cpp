/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/

#include <packet/DiscoveryPacket.h>

DiscoveryPacket::DiscoveryPacket()
{
    CAW_INFO_TRACE_THIS("DiscoveryPacket::DiscoveryPacket"); 

    length=0;
    xid=0;
    from=0;
    to=0;
    linklength=0;
}
DiscoveryPacket::~DiscoveryPacket()
{

    CAW_INFO_TRACE_THIS("DiscoveryPacket::~DiscoveryPacket"); 

}

DiscoveryPacket::DiscoveryPacket(const DiscoveryPacket &right)
{
    length = right.length;
    from = right.from;
    to=right.to;
    xid=right.xid;
    linklength=right.linklength;
    m_links.clear();
    for(const Discoverylink &link:right.m_links)
    {
        m_links.push_back(link);
    }
}

DiscoveryPacket &DiscoveryPacket::operator=(const DiscoveryPacket &right)
{
    length = right.length;
    from = right.from;
    to=right.to;
    xid=right.xid;
    linklength=right.linklength;
    m_links.clear();
    for(const Discoverylink &link:right.m_links)
    {
        m_links.push_back(link);
    }

    return *this;
}


CAWResult DiscoveryPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    CAWResult result = CAW_OK;
    is>>length;
    is>>linklength;
    is>>xid;
    is>>from;
    is>>to;
    int size=(int)linklength;
    if (linklength%sizeof(Discoverylink) != 0)
    {
        CAW_ASSERTE(0);
        return CAW_ERROR_FAILURE;
    }
    while (size>0)
    {
        Discoverylink onlink;
        is>>onlink.src.dpit;
        is>>onlink.src.port;
        is>>onlink.src.pad;
        is>>onlink.dst.dpit;
        is>>onlink.dst.port;
        is>>onlink.dst.pad;
        AddLink(onlink);
        size-=sizeof(Discoverylink);
    }

    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return result;
}
CAWResult DiscoveryPacket::StreamTo(CAWByteStreamNetwork& os)
{
    length = GetStreamLen();
    linklength=sizeof(Discoverylink)*m_links.size();
    os<<length;
    os<<linklength;
    os<<xid;
    os<<from;
    os<<to;
    for(Discoverylink &onlink : m_links)
    {
        os<<onlink.src.dpit;
        os<<onlink.src.port;
        os<<onlink.src.pad;
        os<<onlink.dst.dpit;
        os<<onlink.dst.port;
        os<<onlink.dst.pad;
    }

    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;
}
CAWResult DiscoveryPacket::FromJson(Json::Value& jsonin)
{
    //CAWResult result = CAW_OK;
    ValueFromJson(jsonin,DISCOVERY_JSON_XID, xid);
    ValueFromJson(jsonin,DISCOVERY_JSON_FROM, from);
    ValueFromJson(jsonin,DISCOVERY_JSON_TO, to);

    FromJson2(jsonin);
    ValueFromJson(jsonin,DISCOVERY_JSON_LINKLENTH, linklength);
    return CAW_OK;
}
CAWResult DiscoveryPacket::FromJson2(Json::Value& jsonin)
{
    CAWResult result = CAW_OK;
    if (!jsonin[DISCOVERY_JSON_PATHS].isNull()){
        if (jsonin[DISCOVERY_JSON_PATHS].isArray())
        {
            Json::Value arrayvalue=jsonin[DISCOVERY_JSON_PATHS];
            for(unsigned int i=0;i<arrayvalue.size();i++){
                Discoverylink action;
                Json::Value onevalue = arrayvalue[i];
                Json::Value src;
                Json::Value dst;

                if (!onevalue[DISCOVERY_JSON_LINK_SRC].isNull())
                {
                    src=onevalue[DISCOVERY_JSON_LINK_SRC];
                }

                if (!onevalue[DISCOVERY_JSON_LINK_DST].isNull())
                {
                    dst=onevalue[DISCOVERY_JSON_LINK_DST];
                }

                ValueFromJson(src,DISCOVERY_JSON_CONNECTPOINT_DP, action.src.dpit);
                ValueFromJson(src,DISCOVERY_JSON_CONNECTPOINT_PORT, action.src.port);
                ValueFromJson(dst,DISCOVERY_JSON_CONNECTPOINT_DP, action.dst.dpit);
                ValueFromJson(dst,DISCOVERY_JSON_CONNECTPOINT_PORT, action.dst.port);

                
                m_links.push_back(action);
            }
            linklength=m_links.size()*sizeof(Discoverylink);
        }
    }

    return result;
}



CAWResult DiscoveryPacket::ToJson(Json::Value& jsonout)
{
    jsonout[DISCOVERY_JSON_XID]=ValueToJson(xid);

    jsonout[DISCOVERY_JSON_FROM]=ValueToJson(from);
    jsonout[DISCOVERY_JSON_TO]=ValueToJson(to);
    jsonout[DISCOVERY_JSON_LINKLENTH]=ValueToJson(linklength);

    return ToJson2(jsonout);

}

CAWResult DiscoveryPacket::ToJson2(Json::Value& jsonout)
{
    Json::Value arrays;
    for(Discoverylink &link : m_links){
        Json::Value value;
        Json::Value srcvalue;
        Json::Value dstvalue;

        srcvalue[DISCOVERY_JSON_CONNECTPOINT_DP]=ValueToJson(link.src.dpit);
        srcvalue[DISCOVERY_JSON_CONNECTPOINT_PORT]=ValueToJson(link.src.port);

        dstvalue[DISCOVERY_JSON_CONNECTPOINT_DP]=ValueToJson(link.dst.dpit);
        dstvalue[DISCOVERY_JSON_CONNECTPOINT_PORT]=ValueToJson(link.dst.port);

        value[DISCOVERY_JSON_LINK_SRC]=srcvalue;
        value[DISCOVERY_JSON_LINK_DST]=dstvalue;

        arrays.append(value);
    }
    
    jsonout[DISCOVERY_JSON_PATHS]=arrays;

    return CAW_OK;

}

uint16_t DiscoveryPacket::GetStreamLen() const
{
    return sizeof(DiscoveryStruct)+m_links.size()*sizeof(Discoverylink);
}
uint16_t DiscoveryPacket::GetEncodeSpace()       const
{
    return sizeof(DiscoveryStruct)+m_links.size()*sizeof(Discoverylink);
}
void DiscoveryPacket::Dump() const
{

}

void DiscoveryPacket::AddLink(Discoverylink &link)
{
    m_links.push_back(link);
}

void DiscoveryPacket::GetLinks(std::list<Discoverylink> &list)
{
    for(Discoverylink &link : m_links)
    {
        list.push_back(link);
    }
}
void DiscoveryPacket::SetFrom(uint64_t fromdp)
{
    from=fromdp;
}
void DiscoveryPacket::SetTo(uint64_t todp)
{
    to=todp;
    
}
void DiscoveryPacket::SetXid(uint32_t nxid)
{
    xid=nxid;
}
uint64_t DiscoveryPacket::GetFrom()
{
    return from;
}
uint64_t DiscoveryPacket::GetTo()
{
    return to;
}
uint32_t DiscoveryPacket::GetXid()
{
    return xid;
}

void DiscoveryPacket::ModifyLastDpPort(uint32_t port, uint64_t dp)
{
    if (m_links.size()>0)
    {
        Discoverylink &link = m_links.back();
        link.dst.dpit=dp;
        link.dst.port=port;
    }
}

bool DiscoveryPacket::IsHaveTheDP(uint64_t dpid)
{
    for(Discoverylink &onlink : m_links)
    {
        if (onlink.src.dpit == dpid)
        {
            return true;
        }
        if (onlink.dst.dpit == dpid)
        {
            return true;
        }
    }
    return false;
}

void DiscoveryPacket::GetLoopList(uint64_t dpid, std::list<Discoverylink> &lists)
{
    std::list<Discoverylink>::iterator it = m_links.begin();

    std::list<Discoverylink>::iterator itstart = m_links.begin();
    std::list<Discoverylink>::iterator itend = m_links.end();
    while(itstart != itend)
    {
        Discoverylink &onlink=*itstart;

        if (onlink.src.dpit == dpid)
        {
            break;
        }
        itstart++;
    }
    for(it =itstart;it!=itend;it++ )
    {
        Discoverylink &addlink=*it;
        lists.push_back(addlink);
    }
}

bool DiscoveryPacket::DpIsEnd(uint64_t dpid)
{
    if (to==dpid)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

CAWResult DiscoveryPacket::AddNewLink(uint32_t port, uint64_t dpid, uint32_t cost)
{
    if (m_links.size() == 0)
    {
        Discoverylink newlink;
        ::memset(&newlink, 0, sizeof(newlink));
        newlink.src.dpit=dpid;
        newlink.src.port=port;
        newlink.src.pad=cost;
        m_links.push_back(newlink);
        return CAW_OK;
    }
    else 
    {
        Discoverylink &backlink = m_links.back();
        if (backlink.dst.dpit != dpid)
        {
            CAW_ERROR_TRACE("DiscoveryPacket::AddNewLink, befor link dst.dpit="<<backlink.dst.dpit<<


"new dp="<<dpid);
            CAW_ASSERTE(0);
            return CAW_ERROR_FAILURE;
        }
        Discoverylink newlink;
        ::memset(&newlink, 0, sizeof(newlink));
        newlink.src.dpit=dpid;
        newlink.src.port=port;
        newlink.src.pad=cost;
        m_links.push_back(newlink);
        return CAW_OK;
    }
}
size_t DiscoveryPacket::GetLinkSize()
{
    return m_links.size();
}

