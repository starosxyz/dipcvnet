#include "dipcvnet/vos/CDIPCVnetVOSPortExt.h"
#include "main/CDIPCVNetProcess.h"
#include "main/CDIPCVNetConfig.h"
#include "VNETPdu.h"
#include "utils/dipcvnetutils.h"
#include "dipcvnet/vos/CDIPCVnetVOSManager.h"
#include "dipcvnet/CVNetDataChannel.h"

int VOSPortExtOutput(struct vos_veth* veth, void* m)
{
    char buffer[1024 * 2];

    void* data = (void*)buffer;
    int len = vos_mbuf_getpktlen(m);
    int ret = vos_mbuf_copydata(m, data, 0, len);
    if (ret < 0) {
        vos_mbuf_free(m);
        return -1;
    }

    CDIPCVnetVOSPortExt* pport = (CDIPCVnetVOSPortExt*)vos_veth_get_cookie(veth);
    if (pport)
    {
        pport->Output((char *)data, len);
    }
    return 0;
}

CDIPCVnetVOSPortExt::CDIPCVnetVOSPortExt(CDIPCVnetVOSManager* mgr)
        :m_if(NULL),
        m_pmgr(NULL)
{
    
}
    
CDIPCVnetVOSPortExt::~CDIPCVnetVOSPortExt()
{
    if (m_if)
    {
        destroy_vos_veth(m_if);
        m_if = NULL;
    }
}
CAWResult CDIPCVnetVOSPortExt::Init()
{
    m_ifname = CDipcVnetConfig::Instance()->GetVosTapName();
        
    DWORD vos_tap_ip = 0;
    CAWString str_ip = CDipcVnetConfig::Instance()->GetVosTapIp();
    CAWInetAddr::IpAddrStringTo4Bytes(str_ip.c_str(),vos_tap_ip);

    DWORD vos_tap_mask = 0;
    CAWString str_mask = CDipcVnetConfig::Instance()->GetVosTapMask();
    CAWInetAddr::IpAddrStringTo4Bytes(str_mask.c_str(),vos_tap_mask);

    DWORD vos_tap_gateway = 0;
    CAWString str_gateway = CDipcVnetConfig::Instance()->GetVosTapGateway();
    CAWInetAddr::IpAddrStringTo4Bytes(str_gateway.c_str(),vos_tap_gateway);

    /* create port */

    vos_in_addr_t ipaddr = (uint32_t)ntohl(vos_tap_ip);
    vos_in_addr_t netmask = (uint32_t)ntohl(vos_tap_mask);
    vos_in_addr_t broadcast = 0;
    vos_in_addr_t gw = (uint32_t)ntohl(vos_tap_gateway);
    
    char macaddr[VOS_MAC_LEN] = { 0 };
    uint32_t mac_one;
    char str[128] = {0};
    uint32_t count = 0;
    bool random_mac = CDipcVnetConfig::Instance()->IsOsTapRandomMac();
    if(!random_mac)
    {
        CAWMacAddress addrmac;
        addrmac.ParseAddress(CDipcVnetConfig::Instance()->GetVosTapMac().c_str());
        addrmac.ToOctet((char*)macaddr);
        for(int i=0;i<6;i++)
        {
            mac_one = (uint8_t)macaddr[i];
            count = count+sprintf(&str[count],"%x%x:",(mac_one/(uint32_t)16),(mac_one%(uint32_t)16));
        }
    }else{
        srand((uint32_t)time(NULL));
        for(int i=0; i<6; i++)
        {
            mac_one = (uint32_t)rand() % (uint32_t)256;
            macaddr[i] = (char)mac_one;
            count = count+sprintf(&str[count],"%x%x:",(mac_one/(uint32_t)16),(mac_one%(uint32_t)16));
        }
    }
    
    m_if = create_vos_veth(&ipaddr, 
        &netmask,
        &broadcast, 
        &gw, 
        macaddr, 
        (char*)m_ifname.c_str(), 
        VOSPortExtOutput, this);
    if (m_if == NULL)
    {
        return CAW_ERROR_FAILURE;
    }

    return CAW_OK;
}
CAWResult CDIPCVnetVOSPortExt::Output(const char* pkt, size_t pktsize)
{
    if (m_pmgr)
    {
        m_pmgr->GetLocalPort()->Input(pkt, pktsize);
    }
    return CAW_OK;
}
void CDIPCVnetVOSPortExt::Input(const char* data, size_t datasize)
{
    if (m_if)
    {
        vos_veth_input(m_if, data, datasize, 0);
    }
}