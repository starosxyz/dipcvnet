#ifndef CDIPCVNET_MANAGER_H
#define CDIPCVNET_MANAGER_H
#include <dipc/dipc.h>
#include <dipcutils/CDIPCApp.h>
#include "CDIPCVnetTap.h"
#include "dipcvnet/CDIPCVnetSessionManager.h"
#include "IDIPCVNet.h"
class CDIPCVNetProcess;
using namespace dipc;
class CDIPCVnetManager : public IDIPCVNet
{
public:
	CDIPCVnetManager(CDIPCVNetProcess* process);
	virtual ~CDIPCVnetManager();
	
	virtual CAWResult AddSession(CAWAutoPtr<CDIPCVnetSession>& session, const CDIPCAddress &app);
	virtual CAWResult RemoveSession(const CDIPCAddress &app);
    virtual CDIPCVnetSession *GetSession(const CDIPCAddress &addr);
	void GetAllSession(std::list< CDIPCVnetSession*>& list);
	inline CDIPCVnetSessionManager* GetSessionManager() { return &m_sessionmgr; }
protected:
	CDIPCVNetProcess *m_process;
	CDIPCVnetSessionManager m_sessionmgr;
};

#endif /*CDIPCVNET_MANAGER_H*/