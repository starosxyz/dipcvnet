/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef TPPORT_H
#define TPPORT_H

#include <wface/CAWACEWrapper.h>
#include <starbase/CAWUtils.h>
#include <json/jsoncpp.h>


static const uint16_t TP_MAX_PORT = (uint16_t) 0xffff;
static const uint16_t TP_MIN_PORT = (uint16_t) 0;

/*
  @类名:TpPort
  @功能:Tp端口的管理
*/

class CAW_OS_EXPORT TpPort
{
public:
    TpPort();
    TpPort(uint16_t tpport);
    ~TpPort();
    TpPort(const TpPort &right);
    VlanId& operator=(const TpPort& right);
    bool operator==(const TpPort& right) const;
    bool operator!=(const TpPort& right) const;
    uint16_t toShort();
    CAWString toString();
private:
    uint16_t m_tpport;
};



#endif//VLANID_H

