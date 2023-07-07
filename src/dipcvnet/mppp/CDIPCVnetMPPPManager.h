#ifndef CDIPCVNETMPPPMANAGER_H
#define CDIPCVNETMPPPMANAGER_H
#include "IDIPCVSwitch.h"

#include <dipc/dipc.h>
#include <dipcutils/CDIPCApp.h>
#include "dipcvnet/CDIPCVnetManager.h"
#include "dipcvnet/mppp/CDIPCVnetMPPPTapLocal.h"
class CDIPCVNetProcess;

class CDIPCVnetMPPPManager : public CDIPCVnetManager
{
public:
	CDIPCVnetMPPPManager(CDIPCVNetProcess *process);
	virtual ~CDIPCVnetMPPPManager();
	
public:
	CAWResult Init();

	/* IDIPCVNet*/
	virtual CAWResult VNetAddSession(const CDIPCAddress& app);
	virtual CAWResult VNetRemoveSession(const CDIPCAddress& app);
	virtual IDIPCVnetSession* VNetGetSession(const CDIPCAddress& addr);

    //CAWResult OnPacketRcv(const char* rcvdata, size_t datasize ,const CDIPCAddress &peer_address);
	inline CDIPCVnetMPPPTapLocal *GetLocalTap() { return &m_ptaplocal; }
private:
	CDIPCVNetProcess* m_process;
	CDIPCVnetMPPPTapLocal m_ptaplocal;
};

#endif//CDIPCVNETMPPPMANAGER_H