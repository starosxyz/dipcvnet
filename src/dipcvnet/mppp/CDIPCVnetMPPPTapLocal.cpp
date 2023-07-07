#include "dipcvnet/mppp/CDIPCVnetMPPPTapLocal.h"
#include "main/CDIPCVNetConfig.h"
#include "dipcvnet/mppp/CDIPCVnetMPPPManager.h"
#include "main/CDIPCVNetProcess.h"
#include "dipcvnet/CDIPCVnetSession.h"
#include "utils/MACAddress.h"
CDIPCVnetMPPPTapLocal::CDIPCVnetMPPPTapLocal(CDIPCVnetMPPPManager* switch_manager)
		:m_tap_init(false), m_mpppmgr(switch_manager)
{
}

CDIPCVnetMPPPTapLocal::~CDIPCVnetMPPPTapLocal()
{	
}
void CDIPCVnetMPPPTapLocal::ClearMacTable()
{
    m_macTable.clear();
}
void CDIPCVnetMPPPTapLocal::TapInput(const char* pkt, 
    size_t pktsize, 
    CDIPCVnetSession *psession)
{
    char* psmac = (char*)pkt+ ETHERNET_MAC_LENGTH;
    MacKey mackey;
    memcpy(mackey.dmac, psmac, ETHERNET_MAC_LENGTH);
    mackey.vlanid = 0;
    m_macTable[mackey] = psession;
    Input(pkt, pktsize);
}

void CDIPCVnetMPPPTapLocal::OnOutput(const char* pkt, size_t pktsize)
{
    char* pdmac = (char*)pkt;
    //MACAddress macaddr(pdmac);
    MacKey mackey;
    memcpy(mackey.dmac, pdmac, ETHERNET_MAC_LENGTH);
    mackey.vlanid = 0;
    if (MACAddress::isBroadcast(pdmac)|| MACAddress::isMulticast(pdmac))
    {
        std::list< CDIPCVnetSession*> sessionlist;
        m_mpppmgr->GetAllSession(sessionlist);
        for (CDIPCVnetSession* p : sessionlist)
        {
            p->LowlayerSendPacket(pkt, pktsize);
        }
    }
    else
    {
        CDIPCVnetSession* psession = m_macTable[mackey];
        if (psession)
        {
            psession->LowlayerSendPacket(pkt, pktsize);
        }
        else
        {
            std::list< CDIPCVnetSession*> sessionlist;
            m_mpppmgr->GetAllSession(sessionlist);
            for (CDIPCVnetSession* p : sessionlist)
            {
                p->LowlayerSendPacket(pkt, pktsize);
            }
        }
    }
}


CAWResult CDIPCVnetMPPPTapLocal::Tap_init()
{
	if(m_tap_init==true)
	{	
		return CAW_OK;
	}
	CAW_INFO_TRACE("CDIPCVnetMPPPTapLocal::Tap_init");
    
    CAWString strdevname=CDipcVnetConfig::Instance()->GetOsTapName();

    CAWResult rv;
    DWORD extos_ip = 0;
    CAWString strip = CDipcVnetConfig::Instance()->GetMPPPNetaddr();
    if(FALSE == CAWInetAddr::IpAddrStringTo4Bytes(strip.c_str(), extos_ip))
    {
        CAW_ERROR_TRACE("CDIPCVnetMPPPTapLocal::Tap_inits IP address translation failed");
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
    
    rv = Init(strdevname,
                                (uint32_t)ntohl(extos_ip),
                                macoctect,
                                pnetworkthread);
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetMPPPTapLocal::Tap_inits Init failed");
        return CAW_ERROR_FAILURE;
    }

    DWORD extos_netmask = 0;
    CAWString strnetmask = CDipcVnetConfig::Instance()->GetMPPPNetmask();
    CAWInetAddr::IpAddrStringTo4Bytes(strnetmask,extos_netmask);
    rv = SetVPortIPNetmask((uint32_t)ntohl(extos_netmask));
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetMPPPTapLocal::Tap_inits SetVPortIPNetmask failed "<<extos_netmask);
    }

    DWORD extos_gateway = 0;
    CAWString strgateway = CDipcVnetConfig::Instance()->GetMPPPGateway();
    CAWInetAddr::IpAddrStringTo4Bytes(strgateway,extos_gateway);
    rv = SetVPortIPGateway((uint32_t)ntohl(extos_gateway));
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetMPPPTapLocal::Tap_init SetVPortIPGateway failed "<<extos_gateway);
    }

    rv = Up();
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetMPPPTapLocal::Tap_init Up() failed");
    }
    
	m_tap_init=true;
    return CAW_OK;
}