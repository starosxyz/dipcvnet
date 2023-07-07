/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/VlanId.h>

VlanId::VlanId()
    :m_vlanid(VLAN_ID_UNTAGGED)
{
}

VlanId::VlanId(uint16_t vlanid)
    :m_vlanid(vlanid)
{
}
VlanId::~VlanId()
{
}

VlanId::VlanId(const VlanId &right)
{
    m_vlanid=right.m_vlanid;
}
VlanId& VlanId::operator=(const VlanId& right)
{
    if (&right != this)
    {
        m_vlanid=right.m_vlanid;
    }
    return *this;
}

bool VlanId::operator==(const VlanId& right) const
{
    return m_vlanid == right.m_vlanid;
}

bool VlanId::operator!=(const VlanId& right) const
{
    return !(*this == right);
}

uint16_t VlanId::toShort()
{
    return m_vlanid;
}

CAWString VlanId::toString() 
{
    if (m_vlanid == VLAN_ID_ANY_VALUE) {
        return CAWString("Any");
    }
    if (m_vlanid == VLAN_ID_UNTAGGED) {
        return CAWString("None");
    }
    char buffer[1024]={0};
    snprintf(buffer,sizeof(buffer), "%d", m_vlanid);
    return CAWString(buffer);
}
