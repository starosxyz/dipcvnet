/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef VLANID_H
#define VLANID_H

#include <starbase/CAWACEInclude.h>
#include <starbase/CAWUtils.h>
#include <json/jsoncpp.h>
#include <starbase/CAWDefines.h>
using namespace starbase;

static const uint16_t VLAN_ID_UNTAGGED = (uint16_t) 0xffff;

// In a traffic selector, this means that a VLAN ID must be present, but
// can have any value. We use the same value as OpenFlow, but this is not
// required.
static const uint16_t VLAN_ID_ANY_VALUE = (uint16_t) 0x1000;

static const uint16_t VLAN_ID_NO_VID = 0;       // 0 is not used for VLAN ID
static const uint16_t VLAN_ID_RESERVED = 4095;  // represents all tagged traffic

class CAW_OS_EXPORT VlanId 
{
public:
    VlanId();
    VlanId(uint16_t vlanid);
    ~VlanId();
    VlanId(const VlanId &right);
    VlanId& operator=(const VlanId& right);
    bool operator==(const VlanId& right) const;
    bool operator!=(const VlanId& right) const;
    uint16_t toShort();
    CAWString toString();
private:
    uint16_t m_vlanid;
};


#endif//VLANID_H
