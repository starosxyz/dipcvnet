#include "dipcvnet/mppp/CDIPCVnetMPPPSession.h"
#include "main/CDIPCVNetProcess.h"
#include "dipcvnet/mppp/CDIPCVnetMPPPTapLocal.h"
#include "dipcvnet/mppp/CDIPCVnetMPPPManager.h"


CDIPCVnetMPPPSession::CDIPCVnetMPPPSession(CDIPCVNetProcess* pprocess,
    const CDIPCAddress& app,
    CDIPCVnetMPPPManager* switch_manager)
    :CDIPCVnetSession(pprocess, app,app.GetJno())
    , m_mpppmgr(switch_manager)
{

}
    
CDIPCVnetMPPPSession::~CDIPCVnetMPPPSession()
{
}
    
CAWResult CDIPCVnetMPPPSession::OnUplayerRecvPacket(const char *pkt, size_t pktsize)
{
    m_mpppmgr->GetLocalTap()->TapInput(pkt,pktsize,this);
    return CAW_OK;
}

