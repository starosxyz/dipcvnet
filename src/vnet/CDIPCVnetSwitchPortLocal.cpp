#include "CDIPCVnetSwitchPortLocal.h"
#include "CDIPCVNetConfig.h"

CDIPCVnetSwhichSinklocal::CDIPCVnetSwhichSinklocal(IVSwitch* switch_manager)
		:m_tap_init(false),m_switch_manager(switch_manager)
{
}

CDIPCVnetSwhichSinklocal::~CDIPCVnetSwhichSinklocal()
{	
}

//IVSwitchSink OnOutput input to Tap
void CDIPCVnetSwhichSinklocal::OnOutput(IVSwitchSink *from,
        const char *rcvdata,
        size_t datasize,
        bool flood)
{
	if(m_tap_init==false)
	{
		Tap_init();
		CAW_ERROR_TRACE("CDIPCVnetSwhichSinklocal::OnOutput tap_init failed");
		return ;
	}
    CAW_INFO_TRACE("CDIPCVnetSwhichSinklocal::OnOutput IVSwitch Output Tap datasize= "<<datasize);
	Input(rcvdata, datasize);
}

//CAWTap OnOutput input to switch_manager
void CDIPCVnetSwhichSinklocal::OnOutput(const char* pkt, size_t pktsize)
{
    CAW_INFO_TRACE("CDIPCVnetSwhichSinklocal::OnOutput Tap Input IVSwitch datasize="<<pktsize);
	m_switch_manager->Input(this,pkt,pktsize);
}

void CDIPCVnetSwhichSinklocal::OnStateChange(int state)
{
	
}	

CAWResult CDIPCVnetSwhichSinklocal::Tap_init()
{
	if(m_tap_init==true)
	{	
		return CAW_OK;
	}
	CAW_INFO_TRACE("CDIPCVnetSwhichSinklocal::Tap_init");
    IAWReactor *pnetworkthread = CAWThreadManager::Instance()->GetThreadReactor(CAWThreadManager::TT_NETWORK);

    if  (pnetworkthread==NULL)
    {
        CAW_ERROR_TRACE("CAWThreadManager::Instance()->GetThreadReactor(CAWThreadManager::TT_NETWORK) failed");
        return CAW_ERROR_FAILURE;
    }
    
    char macoctect[6] = {0};
    CAWMacAddress addrmac;
    addrmac.ParseAddress(CDipcVnetConfig::Instance()->GetExtosMac().c_str());
    addrmac.ToOctet((char*)macoctect);

    CAWString strdevname=CDipcVnetConfig::Instance()->GetExtosOstap();
    uint32_t vnetip=CDipcVnetConfig::Instance()->GetExtosIpUint();

    for(int i=0;i<6;i++)
    {
        CAW_INFO_TRACE("macoctect["<<i<<"] "<<macoctect[i]);
    }
    CAW_INFO_TRACE("strdevname="<<strdevname<<
        ", vnetip="<<vnetip);
    
    CAWResult rv = Init(strdevname,
                                vnetip,
                                macoctect,
                                pnetworkthread);
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVNetProcess::OnNodeConnected CDIPCVnetTap Init failed");
        return CAW_ERROR_FAILURE;
    }
	m_tap_init=true;
    return CAW_OK;
}