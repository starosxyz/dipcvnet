#include "dipcvnet/vswitch/CDIPCVnetSwitchPortLocal.h"
#include "main/CDIPCVNetConfig.h"
#include "dipcvnet/vswitch/CDIPCVnetSwitchManager.h"
CDIPCVnetSwitchPortLocal::CDIPCVnetSwitchPortLocal(CDIPCVnetSwitchManager* switch_manager)
		:m_tap_init(false),m_switch_manager(switch_manager)
{
}

CDIPCVnetSwitchPortLocal::~CDIPCVnetSwitchPortLocal()
{	
}

//IDIPCVSwitchSink OnOutput input to Tap
void CDIPCVnetSwitchPortLocal::OnOutput(IDIPCVSwitchSink *from,
        const char *rcvdata,
        size_t datasize,
        bool flood)
{
	if(m_tap_init==false)
	{
		Tap_init();
		CAW_ERROR_TRACE("CDIPCVnetSwitchPortLocal::OnOutput tap_init failed");
		return ;
	}
    CAW_INFO_TRACE("CDIPCVnetSwitchPortLocal::OnOutput IDIPCVSwitch Output Tap datasize= "<<datasize);
	Input(rcvdata, datasize);
}

//CAWTap OnOutput input to switch_manager
void CDIPCVnetSwitchPortLocal::OnOutput(const char* pkt, size_t pktsize)
{
    CAW_INFO_TRACE("CDIPCVnetSwitchPortLocal::OnOutput Tap Input IDIPCVSwitch datasize="<<pktsize);
	m_switch_manager->GetVSwitch()->Input(this,pkt,pktsize);
}

void CDIPCVnetSwitchPortLocal::OnStateChange(int state)
{
	
}	

CAWResult CDIPCVnetSwitchPortLocal::Tap_init()
{
	if(m_tap_init==true)
	{	
		return CAW_OK;
	}
	CAW_INFO_TRACE("CDIPCVnetSwitchPortLocal::Tap_init");

    CAWString strdevname=CDipcVnetConfig::Instance()->GetOsTapName();

    CAWResult rv;
    DWORD extos_ip = 0;
    CAWString strip = CDipcVnetConfig::Instance()->GetVswitchNetaddr();
    if(FALSE == CAWInetAddr::IpAddrStringTo4Bytes(strip.c_str(), extos_ip))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwitchPortLocal::Tap_inits IP address translation failed");
        return CAW_ERROR_FAILURE;
    }
    
    char macoctect[6] = {0};
    uint32_t mac_one;
    char str[128] = {0};
    uint32_t count = 0;
    srand((uint32_t)time(NULL));
    for(int i=0; i<6; i++)
    {
        mac_one = (uint32_t)rand() % (uint32_t)256;
        macoctect[i] = (char)mac_one;
        count = count+sprintf(&str[count],"%x%x:",(mac_one/(uint32_t)16),(mac_one%(uint32_t)16));
    }
    str[count-1] = '\0';

    IAWReactor *pnetworkthread = CAWThreadManager::Instance()->GetThreadReactor(CAWThreadManager::TT_NETWORK);
    if  (pnetworkthread==NULL)
    {
        CAW_ERROR_TRACE("CAWThreadManager::Instance()->GetThreadReactor(CAWThreadManager::TT_NETWORK) failed");
        return CAW_ERROR_FAILURE;
    }
    
    CAW_INFO_TRACE("strdevname="<<strdevname<<
        ", vnetip="<<strip.c_str()<<", Mac "<<str);
    
    rv = CAWTap::Init(strdevname,
                                (uint32_t)ntohl(extos_ip),
                                macoctect,
                                pnetworkthread);
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwitchPortLocal::Tap_inits Init failed");
        return CAW_ERROR_FAILURE;
    }

    DWORD extos_netmask = 0;
    CAWString strnetmask = CDipcVnetConfig::Instance()->GetVswitchNetmask();
    CAWInetAddr::IpAddrStringTo4Bytes(strnetmask.c_str(), extos_netmask);
    rv = SetVPortIPNetmask((uint32_t)ntohl(extos_netmask));
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwitchPortLocal::Tap_inits SetVPortIPNetmask failed "<<extos_netmask);
    }

    DWORD extos_gateway = 0;
    CAWString strgateway = CDipcVnetConfig::Instance()->GetVswitchGateway();
    CAWInetAddr::IpAddrStringTo4Bytes(strgateway.c_str(), extos_gateway);
    rv = SetVPortIPGateway((uint32_t)ntohl(extos_gateway));
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwitchPortLocal::Tap_init SetVPortIPGateway failed "<<extos_gateway);
    }

    rv = Up();
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwitchPortLocal::Tap_init Up() failed");
    }
    
	m_tap_init=true;
    return CAW_OK;
}