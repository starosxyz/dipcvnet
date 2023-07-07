#include "dipcvnet/mppp/CDIPCVnetMPPPManager.h"

#include "dipcvnet/mppp/CDIPCVnetMPPPSession.h"
#include "dipcvnet/mppp/CDIPCVnetMPPPTapLocal.h"
#include "utils/dipcvnetutils.h"




CDIPCVnetMPPPManager::CDIPCVnetMPPPManager(CDIPCVNetProcess *process)
	:CDIPCVnetManager(process),
    m_process(process),
    m_ptaplocal(this)
{
    CAW_INFO_TRACE_THIS("CDIPCVnetMPPPManager::CDIPCVnetMPPPManager");
}

CDIPCVnetMPPPManager::~CDIPCVnetMPPPManager()
{
    CAW_INFO_TRACE_THIS("CDIPCVnetMPPPManager::~CDIPCVnetMPPPManager");
}

CAWResult CDIPCVnetMPPPManager::Init()
{
    CAW_INFO_TRACE_THIS("CDIPCVnetMPPPManager::Init");
    if (m_ptaplocal.Tap_init() != CAW_OK)
    {
        CAW_ERROR_TRACE_THIS("CDIPCVnetMPPPManager::Init Tap_init");
        return CAW_ERROR_FAILURE;
    }

    return CAW_OK;
}

CAWResult CDIPCVnetMPPPManager::VNetAddSession(const CDIPCAddress& app)
{
    CAW_INFO_TRACE_THIS("CDIPCVnetMPPPManager::VNetAddSession");
    CDIPCVnetMPPPSession* psession = new CDIPCVnetMPPPSession(m_process, app, this);
    if (psession == NULL)
    {
        
        return CAW_ERROR_FAILURE;
    }
    CAWAutoPtr<CDIPCVnetSession> session(psession);
    return AddSession(session, app);
}
CAWResult CDIPCVnetMPPPManager::VNetRemoveSession(const CDIPCAddress& app)
{
    CAW_INFO_TRACE_THIS("CDIPCVnetMPPPManager::VNetRemoveSession");
    IDIPCVnetSession* p = VNetGetSession(app);
    if (p == NULL)
    {
        return CAW_ERROR_FAILURE;
    }
    CDIPCVnetMPPPSession* psession = dynamic_cast<CDIPCVnetMPPPSession*>(p);

    if (psession == NULL)
    {

        return CAW_ERROR_FAILURE;
    }

    return RemoveSession(app);
}
IDIPCVnetSession* CDIPCVnetMPPPManager::VNetGetSession(const CDIPCAddress& addr)
{
    CDIPCVnetSession* psession = GetSession(addr);
    return psession;
}

