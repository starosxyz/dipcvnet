/***********************************************************************
    Copyright (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/


#ifndef CSTAR5GCORECONFIG_H
#define CSTAR5GCORECONFIG_H
#include <starbase/CAWACEInclude.h>
#include <wface/CAWACEWrapper.h>

#include <starbase/CConfigFile.h>
#include <starbase/CAWString.h>


using namespace wface;
using namespace starbase;


class CDipcVnetConfig 
{

public:
    static CDipcVnetConfig *Instance();
    CDipcVnetConfig();
    ~CDipcVnetConfig();

    inline const CAWString &GetExtosMac() const { return m_extos_mac; }
    inline const CAWString &GetExtosIp() const { return m_extos_ip; }
    inline const CAWString &GetExtosIpMask() const { return m_extos_ip_mask; }
    inline const CAWString &GetExtosOstap() const { return m_extos_ostap; }
    
    inline bool IsExtosEnable() {return m_extosenable;}

    inline const CAWString &GetDipcMode() const { return m_dipc_mode; }    
    inline const CAWString &GetDipcZone() const { return m_dipc_zone; }
    inline const CAWString &GetDipcIp() const { return m_dipc_ip; }
    inline const CAWString &GetDipcHostname() const { return m_dipc_hostname; }
    inline const CAWString &GetDipcPassword() const { return m_dipc_password; }

    inline const uint32_t GetExtosIpUint() { return m_extos_ip_uint32_t; }

protected:
    CAWResult readconfig();
private:
    CAWString m_extos_mac;
    CAWString m_extos_ip;
    CAWString m_extos_ip_mask;
	CAWString m_extos_ostap;
	bool m_extosenable;

    CAWString m_dipc_mode;
    CAWString m_dipc_zone;
    CAWString m_dipc_ip;
    CAWString m_dipc_hostname;
    CAWString m_dipc_password;

    uint32_t m_extos_ip_uint32_t;
};



#endif//CCPROXYCONFIG_H
