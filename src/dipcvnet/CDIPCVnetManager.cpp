#include "CDIPCVnetManager.h"
#include "main/CDIPCVNetProcess.h"
#include "main/CDIPCVNetConfig.h"
#include "utils/dipcvnetutils.h"


CDIPCVnetManager::CDIPCVnetManager(CDIPCVNetProcess* process)
	:m_process(process)
	, m_sessionmgr(process)
{
	CAW_INFO_TRACE_THIS("CDIPCVnetManager::CDIPCVnetManager");
}

CDIPCVnetManager::~CDIPCVnetManager()
{
	CAW_INFO_TRACE_THIS("CDIPCVnetManager::~CDIPCVnetManager");
}
CAWResult CDIPCVnetManager::AddSession(CAWAutoPtr<CDIPCVnetSession>& session, const CDIPCAddress& app)
{
	CAW_INFO_TRACE_THIS("CDIPCVnetManager::AddSession");
	return m_sessionmgr.AddSession(session,app);
}
CAWResult CDIPCVnetManager::RemoveSession(const CDIPCAddress &app)
{
	CAW_INFO_TRACE_THIS("CDIPCVnetManager::RemoveSession");
	return m_sessionmgr.RemoveSession(app);
}
CDIPCVnetSession *CDIPCVnetManager::GetSession(const CDIPCAddress &addr)
{
	return m_sessionmgr.GetSession(addr);
}
void CDIPCVnetManager::GetAllSession(std::list< CDIPCVnetSession*>& list)
{
	m_sessionmgr.GetAllSession(list);
}