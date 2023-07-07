#include "dipcvnet/vos/CDIPCVnetVOSManager.h"

#include "main/CDIPCVNetProcess.h"
#include "main/CDIPCVNetConfig.h"
#include "utils/dipcvnetutils.h"
#include "dipcvnet/vos/CDIPCVnetVOSSession.h"

CDIPCVnetVOSManager::CDIPCVnetVOSManager(CDIPCVNetProcess* process)
	:CDIPCVnetManager(process)
    , m_localport(this)
    , m_vosport(this)
{
}

CDIPCVnetVOSManager::~CDIPCVnetVOSManager()
{
}
CAWResult CDIPCVnetVOSManager::Init()
{
    CAWResult rv = m_localport.Tap_init();
    if (rv != CAW_OK)
    {
        CAW_ERROR_TRACE_THIS("CDIPCVnetVOSManager::Init failure");
        return CAW_ERROR_FAILURE;
    }
    if (m_vosport.Init() != CAW_OK)
    {
        CAW_ERROR_TRACE_THIS("CDIPCVnetVOSManager::Init m_vosport ");
        return CAW_ERROR_FAILURE;
    }

    return CAW_OK;
}
CAWResult CDIPCVnetVOSManager::VNetAddSession(const CDIPCAddress& app)
{
    CDIPCVnetVOSSession* psession = new CDIPCVnetVOSSession(m_process, app, this);
    if (psession == NULL)
    {

        return CAW_ERROR_FAILURE;
    }
    if (psession->Init()!= CAW_OK)
    {
        delete psession;
        return CAW_ERROR_FAILURE;
    }
    CAWAutoPtr<CDIPCVnetSession> session(psession);
    return AddSession(session, app);
}
CAWResult CDIPCVnetVOSManager::VNetRemoveSession(const CDIPCAddress& app)
{
    IDIPCVnetSession* p = VNetGetSession(app);
    if (p == NULL)
    {
        return CAW_ERROR_FAILURE;
    }
    CDIPCVnetVOSSession* psession = dynamic_cast<CDIPCVnetVOSSession*>(p);

    if (psession == NULL)
    {

        return CAW_ERROR_FAILURE;
    }

    return RemoveSession(app);
}
IDIPCVnetSession* CDIPCVnetVOSManager::VNetGetSession(const CDIPCAddress& addr)
{
    CDIPCVnetSession* psession = GetSession(addr);
    return psession;
}