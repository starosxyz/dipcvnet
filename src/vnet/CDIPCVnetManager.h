#ifndef CDIPCVNET_MANAGER_H
#define CDIPCVNET_MANAGER_H
#include <dipc/dipc.h>
#include <dipcutils/CDIPCApp.h>
#include "CDIPCVnetTap.h"

class CDIPCVNetProcess;

using namespace dipc;
typedef CAWHashMapT<DIPCAppKey,CDIPCVnetTap*,CDIPCAppKeyHashFun> VNetMAPType;

class CDIPCVnetManager
{
public:
	CDIPCVnetManager(CDIPCVNetProcess* process);
	~CDIPCVnetManager();

public:
    CAWResult CreateVnetTap(const CDIPCAddress &peer_address);
	CAWResult AddVnet(CDIPCVnetTap *pvnet,const CDIPCAddress &app);
	CAWResult RemoveVnet(const CDIPCAddress &app);
    CDIPCVnetTap *GetVnet(const CDIPCAddress &addr);

    CAWResult OnPacketRcv(const char* rcvdata, size_t datasize,
        const CDIPCAddress &peer_address);

private:
	CDIPCVNetProcess *m_process;
	VNetMAPType m_vnetmap;
};

#endif /*CDIPCVNET_MANAGER_H*/