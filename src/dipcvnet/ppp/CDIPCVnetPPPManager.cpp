#include "dipcvnet/ppp/CDIPCVnetPPPManager.h"
#include "dipcvnet/ppp/CDIPCVnetPPPSession.h"
#include "main/CDIPCVNetProcess.h"
#include "main/CDIPCVNetConfig.h"
#include "utils/dipcvnetutils.h"


CDIPCVnetPPPManager::CDIPCVnetPPPManager(CDIPCVNetProcess* process)
	:CDIPCVnetManager(process)
{
}

CDIPCVnetPPPManager::~CDIPCVnetPPPManager()
{
}
CAWResult CDIPCVnetPPPManager::Init()
{
	return CAW_OK;
}
CAWResult CDIPCVnetPPPManager::VNetAddSession(const CDIPCAddress& app)
{
    CDIPCVnetPPPSession* psession = new CDIPCVnetPPPSession(m_process, app, this);
    if (psession == NULL)
    {

        return CAW_ERROR_FAILURE;
    }

    if (psession->SessionInit()!= CAW_OK)
    {
        delete psession;
        return CAW_ERROR_FAILURE;
    }
    CAWAutoPtr<CDIPCVnetSession> session(psession);
    return AddSession(session, app);
}
CAWResult CDIPCVnetPPPManager::VNetRemoveSession(const CDIPCAddress& app)
{
    IDIPCVnetSession* p = VNetGetSession(app);
    if (p == NULL)
    {
        return CAW_ERROR_FAILURE;
    }
    CDIPCVnetPPPSession* psession = dynamic_cast<CDIPCVnetPPPSession*>(p);

    if (psession == NULL)
    {

        return CAW_ERROR_FAILURE;
    }

    return RemoveSession(app);
}
IDIPCVnetSession* CDIPCVnetPPPManager::VNetGetSession(const CDIPCAddress& addr)
{
	CDIPCVnetSession* psession = GetSession(addr);
	return psession;
}
