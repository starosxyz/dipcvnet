/***********************************************************************************
*           版权所有 (C)2016-2021, 南京北星极网络科技有限公司
*************************************************************************************/
#include "CMacTable.h"

CMacTable::CMacTable(uint64_t dpid,
        uint32_t port,
        char mac[MACTABLE_MAC_LEN],
        uint16_t vlanid)
    :m_dpid(dpid),m_port(port)
{
    if (vlanid == 65535)
        m_vlanid = 0;
    else
        m_vlanid = vlanid;
    ::memset(m_mac, 0, sizeof(m_mac));
    memcpy(m_mac, mac, MACTABLE_MAC_LEN);
}
CMacTable::CMacTable():m_dpid(0),m_port(0)
{
    ::memset(m_mac, 0, sizeof(m_mac));
    m_vlanid = 0;
}
CMacTable::~CMacTable()
{
}
CMacTable::CMacTable(const CMacTable& right)
{
    (*this) = right;
}


CMacTable& CMacTable::operator=(const CMacTable& right)
{
    m_port = right.m_port;
    m_dpid = right.m_dpid;
    memcpy(m_mac, right.m_mac, MACTABLE_MAC_LEN);
    m_vlanid = right.m_vlanid;
    return (*this);  
}


bool CMacTable::operator==(const CMacTable& right) const
{
    return ((m_currenttime == right.m_currenttime) &&
        (m_port == right.m_port) &&
        (m_dpid == right.m_dpid) &&
        (::memcmp(m_mac, right.m_mac, MACTABLE_MAC_LEN)) &&
        (m_vlanid == right.m_vlanid));

}

bool CMacTable::operator!=(const CMacTable& right)
{
    return !(CMacTable::operator==(right));
}

void CMacTable::Print()
{
}
void CMacTable::GetMac(char mac[MACTABLE_MAC_LEN])
{
    ::memcpy(mac,m_mac,MACTABLE_MAC_LEN);
}
