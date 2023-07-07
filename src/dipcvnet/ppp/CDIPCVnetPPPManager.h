#ifndef CDIPCVNET_PPP_MANAGER_H
#define CDIPCVNET_PPP_MANAGER_H
#include <dipc/dipc.h>
#include <dipcutils/CDIPCApp.h>
#include "dipcvnet/CDIPCVnetManager.h"

class CDIPCVNetProcess;

class CDIPCVnetPPPManager : public CDIPCVnetManager
{
public:
	CDIPCVnetPPPManager(CDIPCVNetProcess* process);
	virtual ~CDIPCVnetPPPManager();
	CAWResult Init();
	virtual CAWResult VNetAddSession(const CDIPCAddress& app);
	virtual CAWResult VNetRemoveSession(const CDIPCAddress& app);
	virtual IDIPCVnetSession* VNetGetSession(const CDIPCAddress& addr);
};

#endif /*CDIPCVNET_PPP_MANAGER_H*/