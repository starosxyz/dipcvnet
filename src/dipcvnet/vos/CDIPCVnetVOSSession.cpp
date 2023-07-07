#include "CDIPCVnetVOSSession.h"
#include "main/CDIPCVNetProcess.h"
#include "main/CDIPCVNetConfig.h"
#include "VNETPdu.h"
#include "utils/dipcvnetutils.h"
#include "probe/CDIPCVnetDataPing.h"
#include "probe/CDIPCVNetDIPCPing.h"
#include "dipcvnet/CVNetDataChannel.h"

static bool Local_CDIPCVnet_Select_Session = true;

CDIPCVnetVOSSession::CDIPCVnetVOSSession(CDIPCVNetProcess *pprocess,
        const CDIPCAddress &app,
        CDIPCVnetVOSManager *mgr)
        :CDIPCVnetSession(pprocess, app, app.GetJno()),m_mgr(mgr)
{    
}
    
CDIPCVnetVOSSession::~CDIPCVnetVOSSession()
{

}
CAWResult CDIPCVnetVOSSession::Init()
{
    CAWResult rv = CAW_ERROR_FAILURE;
    rv = CDIPCVnetSession::Init();
    if (CAW_FAILED(rv))
    {
        return rv;
    }

    CAWString ifname = "vnet" + U32ToStr(m_session_id);

    rv = m_vosport.Init(this, ifname);
    if (CAW_FAILED(rv))
    {
        return rv;
    }
    return CAW_OK;
}
CAWResult CDIPCVnetVOSSession::OnUplayerRecvPacket(const char* pkt, size_t pktsize)
{
    m_vosport.Input(pkt, pktsize);
    return CAW_OK;
}
