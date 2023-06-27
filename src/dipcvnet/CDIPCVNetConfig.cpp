/***********************************************************************
    Copyright (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/


#include "CDIPCVNetConfig.h"
#include "starbase/CAWVOS.h"
#include "starbase/CAWInetAddr.h"
#include "starbase/CAWMacAddress.h"
#include <vos/vos_base.h>
#include <json/jsoncpp.h>


#ifdef CAW_RELEASE
#undef PRINT_INFO
#define PRINT_INFO
#endif

using namespace starbase;

static CAWString GetDIPCVNETHome()
{
	CAWString str=VOS_GetEnv("DIPCVNET_ROOT");
    if (str.size()==0)
    {
        str="/opt/staros.xyz/dipcvnet";
    }
    
	return str;
}
static bool IsJson(const CAWString &strjson,Json::Value &json)
{
    Json::Reader reader;  

    if (strjson.size() == 0)
    {
        return false;
    }
    if (!reader.parse(strjson.c_str(), json))
    {
        return false;
    }
    if(!json.isObject())
    {
        return false;
    }
    
    return true;
}


CDipcVnetConfig *CDipcVnetConfig::Instance()
{
    return CAWSingletonT<CDipcVnetConfig>::Instance();
}

CDipcVnetConfig::CDipcVnetConfig()
{
    readconfig();
}

CDipcVnetConfig::~CDipcVnetConfig()
{
}

CAWResult CDipcVnetConfig::readconfig()
{
    
    CAWString projectspath = GetDIPCVNETHome();
    CAWString conffile = projectspath+"/conf/dipcvnet.cfg";
    CConfigFile::Instance().Open(conffile);

    m_dipc_mode = CConfigFile::Instance().GetStringParam(CAWString("vnet"), CAWString("mode"), "p2p");
    m_dipc_ip = CConfigFile::Instance().GetStringParam(CAWString("vnet"), CAWString("serverip"), "127.0.0.1:24445");
    m_dipc_zone = CConfigFile::Instance().GetStringParam(CAWString("vnet"), CAWString("zone"), "1.1.2.0");
    m_dipc_hostname = CConfigFile::Instance().GetStringParam(CAWString("vnet"), CAWString("hostname"), "user2");
    m_dipc_password = CConfigFile::Instance().GetStringParam(CAWString("vnet"), CAWString("password"), "pass2");
    
    m_extos_mac = CConfigFile::Instance().GetStringParam(CAWString("port"), CAWString("mac"), "");
    m_extos_ip = CConfigFile::Instance().GetStringParam(CAWString("port"), CAWString("ip"), "");
    m_extos_ip_mask = CConfigFile::Instance().GetStringParam(CAWString("port"), CAWString("mask"), "");
	m_extos_ostap = CConfigFile::Instance().GetStringParam(CAWString("port"), CAWString("ostap"), "exos_tap0");
	m_extosenable=CConfigFile::Instance().GetBoolParam(CAWString("port"), CAWString("enable"), false);

    CAWInetAddr extosaddr(m_extos_ip, 0);
    m_extos_ip_uint32_t = ntohl(extosaddr.GetIpAddrIn4Bytes());
    
	CConfigFile::Instance().Close();

    std::cout<<"CDipcVnet DipcMode="<<m_dipc_mode.c_str()<<"\n"
            <<"CDipcVnet DipcZone="<<m_dipc_zone.c_str()<<"\n"
            <<"CDipcVnet DipcIp="<<m_dipc_ip.c_str()<<"\n"
            <<"CDipcVnet DipcHostname="<<m_dipc_hostname.c_str()<<"\n"
            <<"CDipcVnet DipcPassword="<<m_dipc_password.c_str()<<std::endl;

    std::cout<<"CDipcVnet m_extos_mac="<<m_extos_mac.c_str()<<"\n"
            <<"CDipcVnet m_extos_ip="<<m_extos_ip.c_str()<<"\n"
            <<"CDipcVnet m_extos_ip_mask="<<m_extos_ip_mask.c_str()<<"\n"
            <<"CDipcVnet m_extos_ostap="<<m_extos_ostap.c_str()<<"\n"
            <<"CDipcVnet m_extosenable="<<m_extosenable<<std::endl;


    return CAW_OK;
}

