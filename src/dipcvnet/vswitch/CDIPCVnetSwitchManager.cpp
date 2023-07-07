#include "dipcvnet/vswitch/CDIPCVnetSwitchManager.h"
//#include "dipcvnet/vswitch/CDIPCVnetSwitchPort.h"
#include "dipcvnet/vswitch/CDIPCVnetSwitchSession.h"
#include "dipcvnet/vswitch/CDIPCVnetSwitchPortLocal.h"
#include "utils/dipcvnetutils.h"
#include "main/CDIPCVNetProcess.h"



CDIPCVnetSwitchManager::CDIPCVnetSwitchManager(CDIPCVNetProcess *process)
	:CDIPCVnetManager(process), m_vswitch(NULL)
    , m_localport(this)
{
}

CDIPCVnetSwitchManager::~CDIPCVnetSwitchManager()
{
    if (m_vswitch)
    {
        DestroyMacLearningSwitch(m_vswitch);
        m_vswitch = NULL;
    }
}

CAWResult CDIPCVnetSwitchManager::Init()
{
    if (!m_vswitch)
    {
        m_vswitch = CreateMacLearningSwitch();
    }
    if(!m_vswitch)
    {
        CAW_ERROR_TRACE("CDIPCVnetSwitchManager::Init() CreateMacLearningSwitch() ==NULL");
        return CAW_ERROR_FAILURE;
    }

    CAWResult rv = m_localport.Tap_init();
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwitchManager::Init() pSwhichSink->Tap_init() failed");
        return CAW_ERROR_FAILURE;
    }
    rv = m_vswitch->AddSink(&m_localport);
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwitchManager::Init() m_switch_manager->AddSinkPort(pSwhichSink) failed");
        return CAW_ERROR_FAILURE;
    }
    rv = m_localport.Up();
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwitchManager::Init() pSwhichSink->Up() failed");
    }

    return CAW_OK;
}


CAWResult CDIPCVnetSwitchManager::OnPacketRcv(const char* rcvdata, size_t datasize,
        const CDIPCAddress &peer_address)
{
	return CAW_OK;
}

CAWResult CDIPCVnetSwitchManager::VNetAddSession(const CDIPCAddress& app)
{
    CDIPCVnetSwitchSession* psession = new CDIPCVnetSwitchSession(m_process, app, this);
    if (psession == NULL)
    {

        return CAW_ERROR_FAILURE;
    }
    if (psession->Init() != CAW_OK)
    {
        delete psession;
        return CAW_ERROR_FAILURE;
    }
    CAWAutoPtr<CDIPCVnetSession> session(psession);
    return AddSession(session, app);
}
CAWResult CDIPCVnetSwitchManager::VNetRemoveSession(const CDIPCAddress& app)
{
    IDIPCVnetSession* p = VNetGetSession(app);
    if (p == NULL)
    {
        return CAW_ERROR_FAILURE;
    }
    CDIPCVnetSwitchSession* psession = dynamic_cast<CDIPCVnetSwitchSession*>(p);

    if (psession == NULL)
    {

        return CAW_ERROR_FAILURE;
    }

    return RemoveSession(app);
}
IDIPCVnetSession* CDIPCVnetSwitchManager::VNetGetSession(const CDIPCAddress& addr)
{
    CDIPCVnetSession* psession = GetSession(addr);
    return psession;
}