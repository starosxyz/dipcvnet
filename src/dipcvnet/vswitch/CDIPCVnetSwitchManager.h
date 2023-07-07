#ifndef CDIPCVNETSWITCHMANAGER_H
#define CDIPCVNETSWITCHMANAGER_H
#include "IDIPCVSwitch.h"
#include "dipcvnet/CDIPCVnetManager.h"
#include <dipc/dipc.h>
#include <dipcutils/CDIPCApp.h>
#include "dipcvnet/CDIPCVnetTap.h"
#include "dipcvnet/vswitch/CDIPCVnetSwitchPortLocal.h"
class CDIPCVNetProcess;

typedef CAWHashMapT<DIPCAppKey,IDIPCVSwitchSink*,CDIPCAppKeyHashFun> VLANMAPType;
class CDIPCVnetSwitchManager : public CDIPCVnetManager
{
public:
	CDIPCVnetSwitchManager(CDIPCVNetProcess *process);
	virtual ~CDIPCVnetSwitchManager();
	
public:
	CAWResult Init();
	CAWResult OnPacketRcv(const char* rcvdata, size_t datasize,
		const CDIPCAddress& peer_address);
	/* IVNET interface */
	virtual CAWResult VNetAddSession(const CDIPCAddress& app);
	virtual CAWResult VNetRemoveSession(const CDIPCAddress& app);
	virtual IDIPCVnetSession* VNetGetSession(const CDIPCAddress& addr);

public:
	IDIPCVSwitch* GetVSwitch() { return m_vswitch; }
private:
	IDIPCVSwitch *m_vswitch;
	CDIPCVnetSwitchPortLocal m_localport;
};

#endif//CDIPCVNETSWITCHPORTLOCAL_H