#include "dipcvnet/ppp/CDIPCVnetPPPSession.h"
#include "main/CDIPCVNetProcess.h"
#include "main/CDIPCVNetConfig.h"


CDIPCVnetPPPSession::CDIPCVnetPPPSession(CDIPCVNetProcess* pprocess,
    const CDIPCAddress& app,
    CDIPCVnetPPPManager* pmgr)
        :CDIPCVnetSession(pprocess, app,app.GetJno()),
    m_pmgr(pmgr)
{    
}
    
CDIPCVnetPPPSession::~CDIPCVnetPPPSession()
{
    CAWTap::Close();
}

CAWResult CDIPCVnetPPPSession::OnUplayerRecvPacket(const char *pkt, size_t pktsize)
{
    CAWTap::Input(pkt,pktsize);
    return CAW_OK;
}

void CDIPCVnetPPPSession::OnOutput(const char* pkt, size_t pktsize)
{
    LowlayerSendPacket(pkt,pktsize);
}


CAWResult CDIPCVnetPPPSession::SessionInit()
{
    CDIPCAddress localdipcaddr = GetLocalAddress();
    CDIPCAddress peer_address = GetPeerAddress();   
    uint32_t local_NodeId=localdipcaddr.GetNodeId();
    uint16_t local_Jno=localdipcaddr.GetJno();
    uint32_t peer_NodeId=peer_address.GetNodeId();
    uint16_t peer_Jno=peer_address.GetJno();

    std::string sub_strname=CDipcVnetConfig::Instance()->GetOsTapName().GetRawData();
    std::string strname = sub_strname + '-' + std::to_string(peer_Jno);
    CAWString strdevname = strname.c_str();

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

    DWORD extos_ip = 0;
    CAWString strip = CDipcVnetConfig::Instance()->GetPPPNetaddr();
    if(FALSE == CAWInetAddr::IpAddrStringTo4Bytes(strip.c_str(),extos_ip))
    {
        CAW_ERROR_TRACE("CDIPCVnetP2PSession::SessionInit() IP address translation failed");
        return CAW_ERROR_FAILURE;
    }
    
    CAWResult rv = CAW_ERROR_FAILURE;
    IAWReactor *pnetworkthread = CAWThreadManager::Instance()->GetThreadReactor(CAWThreadManager::TT_NETWORK);
    if  (pnetworkthread==NULL)
    {
        CAW_ERROR_TRACE("CAWThreadManager::Instance()->GetThreadReactor(CAWThreadManager::TT_NETWORK) failed");
        return CAW_ERROR_FAILURE;
    }

    CAW_INFO_TRACE("strdevname="<<strdevname.c_str()<<
        ", vnetip="<<strip.c_str()<<", Mac "<<str);

    rv = CAWTap::Init(strdevname,
            (uint32_t)ntohl(extos_ip),
            macoctect,
            pnetworkthread); 
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetP2PSession::SessionInit() CDIPCVnetTap Init failed");
        exit(1);
        return CAW_ERROR_FAILURE;
    }

    DWORD extos_netmask = 0;
    CAWString strnetmask = CDipcVnetConfig::Instance()->GetPPPNetmask();
    CAWInetAddr::IpAddrStringTo4Bytes(strnetmask.c_str(), extos_netmask);
    rv = SetVPortIPNetmask((uint32_t)ntohl(extos_netmask));
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetP2PSession::SessionInit() SetVPortIPNetmask failed "<<extos_netmask);
    }

    DWORD extos_gateway = 0;
    CAWString strgateway = CDipcVnetConfig::Instance()->GetPPPGateway();
    CAWInetAddr::IpAddrStringTo4Bytes(strgateway.c_str(),extos_gateway);
    rv = SetVPortIPGateway((uint32_t)ntohl(extos_gateway));
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetP2PSession::SessionInit() SetVPortIPGateway failed "<<extos_gateway);
    }

    rv = Up();
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetManager::CreateVnetTap Up() failed ");
    }

    return CAW_OK;
}

