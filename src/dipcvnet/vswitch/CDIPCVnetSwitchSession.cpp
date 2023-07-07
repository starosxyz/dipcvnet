#include "dipcvnet/vswitch/CDIPCVnetSwitchSession.h"
#include "main/CDIPCVNetProcess.h"
#include "dipcvnet/vswitch/CDIPCVnetSwitchManager.h"

CDIPCVnetSwitchSession::CDIPCVnetSwitchSession(CDIPCVNetProcess* pprocess,
    const CDIPCAddress& app,
    CDIPCVnetSwitchManager* switch_manager)
    :CDIPCVnetSession(pprocess,app,app.GetJno())
    ,m_pswitchmgr(switch_manager)
{
}
    
CDIPCVnetSwitchSession::~CDIPCVnetSwitchSession()
{
    m_pswitchmgr->GetVSwitch()->RemoveSink(this);
}

void CDIPCVnetSwitchSession::OnStateChange(int state)
{

}

CAWResult CDIPCVnetSwitchSession::Init()
{
    CAWResult rv =  CDIPCVnetSession::Init();
    m_pswitchmgr->GetVSwitch()->AddSink(this, DEFAULT_COST);

    return rv;
}
CAWResult CDIPCVnetSwitchSession::OnUplayerRecvPacket(const char *pkt, size_t pktsize)
{
    return m_pswitchmgr->GetVSwitch()->Input(this, pkt, pktsize);
}
    

void CDIPCVnetSwitchSession::OnOutput(IDIPCVSwitchSink *from,
        const char *rcvdata,
        size_t datasize,
        bool flood)
{
    CAW_INFO_TRACE("CDIPCVnetSwitchSession::OnOutput IDIPCVSwitch Output SendToPeer datasize= "<<datasize);
    LowlayerSendPacket(rcvdata,datasize);
}
        
CAWResult CDIPCVnetSwitchSession::InputData(const char *pdata,size_t datasize)
{
    CAW_INFO_TRACE("CDIPCVnetSwitchSession::InputData Input IDIPCVSwitch datasize="<<datasize);
    return m_pswitchmgr->GetVSwitch()->Input(this, pdata, datasize);
}

