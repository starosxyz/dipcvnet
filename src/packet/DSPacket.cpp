/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/DSPacket.h>

#define DSPACKET_SIZE 1400

DSPacket::DSPacket()
    :CEthernet(0x9944)
{
    //char tempdmac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
    char tempdmac[6]={(char)0x01,(char)0x80,(char)0xC2,(char)0x00,(char)0x00,(char)0x03};
    char tempsmac[6]={(char)0x02,(char)0xeb,(char)0x9f,(char)0x66,(char)0xc8,(char)0x44};
    ::memcpy(m_dmac, tempdmac, 6);
    ::memcpy(m_smac, tempsmac, 6);

    m_length=0;
    m_senderport=0;
    m_senderdp=0;
    m_peerport=0;
    m_peerdp=0;
    m_flags=0;
    m_sendcost=0;
    m_fromdatacenterid=0;
    m_fromclusterid=0;
    m_todatacenterid=0;
    m_toclusterid=0;

}
DSPacket::~DSPacket()
{


}

DSPacket::DSPacket(const DSPacket &right)
    :CEthernet(right)
{
    m_length=right.m_length;
    m_senderport=right.m_senderport;
    m_senderdp=right.m_senderdp;
    m_peerdp=right.m_peerdp;
    m_peerport=right.m_peerport;
    m_sendcost = right.m_sendcost;
    m_flags = right.m_flags;
    m_fromdatacenterid=right.m_fromdatacenterid;
    m_fromclusterid=right.m_fromclusterid;
    m_todatacenterid=right.m_todatacenterid;
    m_toclusterid=right.m_toclusterid;
}

DSPacket &DSPacket::operator=(const DSPacket &right)
{
    CEthernet::operator=(right);

    m_length=right.m_length;
    m_senderport=right.m_senderport;
    m_senderdp=right.m_senderdp;
    m_peerdp=right.m_peerdp;
    m_peerport=right.m_peerport;
    m_sendcost = right.m_sendcost;
    m_flags = right.m_flags;

    m_fromdatacenterid=right.m_fromdatacenterid;
    m_fromclusterid=right.m_fromclusterid;
    m_todatacenterid=right.m_todatacenterid;
    m_toclusterid=right.m_toclusterid;

    return *this;
}

CAWResult DSPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    CEthernet::StreamFrom(is);
    is>>m_length;
    is>>m_flags;
    is>>m_senderport;
    is>>m_peerport;
    is>>m_senderdp;
    is>>m_peerdp;
    is >> m_sendcost;
    is>>m_fromclusterid;
    is>>m_fromdatacenterid;
    is>>m_toclusterid;
    is>>m_todatacenterid;

    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;
}

CAWResult DSPacket::StreamTo(CAWByteStreamNetwork& os)
{

    m_length= 52;

    CEthernet::StreamTo(os);

    os<<m_length;
    os<<m_flags;
    os<<m_senderport;
    os<<m_peerport;
    os<<m_senderdp;
    os<<m_peerdp;
    os<<m_sendcost;
    os<<m_fromclusterid;
    os<<m_fromdatacenterid;
    os<<m_toclusterid;
    os<<m_todatacenterid;

    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;
}
CAWResult DSPacket::FromJson(Json::Value& jsonin)
{
    CEthernet::FromJson(jsonin);
    ValueFromJson(jsonin,DSPACKET_JSON_LENGTH, m_length);
    ValueFromJson(jsonin,DSPACKET_JSON_SENDERDP, m_senderdp);
    ValueFromJson(jsonin,DSPACKET_JSON_SENDERPORT, m_senderport);
    return CAW_OK;

}

CAWResult DSPacket::ToJson(Json::Value& jsonout)
{
    CEthernet::ToJson(jsonout);
    jsonout[DSPACKET_JSON_LENGTH]=ValueToJson(m_length);
    jsonout[DSPACKET_JSON_SENDERDP]=ValueToJson(m_senderdp);
    jsonout[DSPACKET_JSON_SENDERPORT]=ValueToJson(m_senderport);
    return CAW_OK;
}


uint16_t DSPacket::GetStreamLen() const
{
    return 52 +CEthernet::GetStreamLen();

}

uint16_t DSPacket::GetEncodeSpace()       const
{
    return 52 +CEthernet::GetEncodeSpace();
}
void DSPacket::Dump() const
{

}

void DSPacket::SetSenderDP(uint64_t senderdp)
{
    m_senderdp=senderdp;
}
void DSPacket::SetSenderPort(uint32_t senderport)
{
    m_senderport=senderport;
}

uint64_t DSPacket::GetSenderDP()
{
    return m_senderdp;
}

uint32_t DSPacket::GetSenderPort()
{
    return m_senderport;
}

void DSPacket::SetFlags(uint32_t flags)
{
    m_flags=flags;
}

uint32_t DSPacket::GetFlags()
{
    return m_flags;
}

void DSPacket::SetSendCost(uint32_t cost)
{
    m_sendcost = cost;
}
uint32_t DSPacket::GetSendCost()
{
    return m_sendcost;
}

void DSPacket::SetPeerDP(uint64_t peerdp)
{
    m_peerdp=peerdp;
}
void DSPacket::SetPeerPort(uint32_t peerport)
{
    m_peerport=peerport;
}

uint64_t DSPacket::GetPeerDP()
{
    return m_peerdp;
}

uint32_t DSPacket::GetPeerPort()
{
    return m_peerport;
}

void DSPacket::SetFromDataCenterId(uint32_t datacentid)
{
    m_fromdatacenterid = datacentid;
}

void DSPacket::SetFromClusterId(uint32_t clusterid)
{
    m_fromclusterid=clusterid;
}


uint32_t DSPacket::GetFromDataCenterId()
{
    return m_fromdatacenterid;
}

uint32_t DSPacket::GetFromClusterId()
{
    return m_fromclusterid;
}

void DSPacket::SetToDataCenterId(uint32_t datacentid)
{
    m_todatacenterid=datacentid;
}

void DSPacket::SetToClusterId(uint32_t clusterid)
{
    m_toclusterid=clusterid;
}


uint32_t DSPacket::GetToDataCenterId()
{
    return m_todatacenterid;
}

uint32_t DSPacket::GetToClusterId()
{
    return m_toclusterid;
}

