#ifndef CDIPCVNETVOSMANAGER_H
#define CDIPCVNETVOSMANAGER_H
#include <dipc/dipc.h>
#include <dipcutils/CDIPCApp.h>
#include "dipcvnet/CDIPCVnetManager.h"
#include "dipcvnet/vos/CDIPCVnetVOSPortLocal.h"
#include "dipcvnet/vos/CDIPCVnetVOSPortExt.h"
class CDIPCVNetProcess;

class CDIPCVnetVOSManager : public CDIPCVnetManager
{
public:
	CDIPCVnetVOSManager(CDIPCVNetProcess* process);
	virtual ~CDIPCVnetVOSManager();
	CAWResult Init();
	virtual CAWResult VNetAddSession(const CDIPCAddress& app);
	virtual CAWResult VNetRemoveSession(const CDIPCAddress& app);
	virtual IDIPCVnetSession* VNetGetSession(const CDIPCAddress& addr);
public:
	inline CDIPCVnetVOSPortLocal* GetLocalPort() { return &m_localport; }
	inline CDIPCVnetVOSPortExt* GetVOSPort() { return &m_vosport; }
private:
	CDIPCVnetVOSPortLocal m_localport;
	CDIPCVnetVOSPortExt m_vosport;
};

#endif /*CDIPCVNETVOSMANAGER_H*/