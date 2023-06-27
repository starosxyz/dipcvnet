#ifndef CDIPCVNETSWITCHMANAGER_H
#define CDIPCVNETSWITCHMANAGER_H
#include "IVSwitch.h"
using namespace bxjstack::vswitch;
#include <dipc/dipc.h>
#include <dipcutils/CDIPCApp.h>
#include "CDIPCVnetTap.h"

class CDIPCVNetProcess;

typedef CAWHashMapT<DIPCAppKey,IVSwitchSink*,CDIPCAppKeyHashFun> VLANMAPType;
class CDIPCVnetSwhichManager
{
public:
	CDIPCVnetSwhichManager(CDIPCVNetProcess *process);
	~CDIPCVnetSwhichManager();
	
public:
	CAWResult Init();

	CAWResult CreateSwitchPort(const CDIPCAddress &peer_address);
	CAWResult AddSwitchPort(IVSwitchSink *pvnet,const CDIPCAddress &peer_address);
	CAWResult RemoveSwitchPort(const CDIPCAddress &peer_address);
    IVSwitchSink *GetSwitchPort(const CDIPCAddress &peer_address);

    CAWResult OnPacketRcv(const char* rcvdata, size_t datasize ,const CDIPCAddress &peer_address);

private:
	IVSwitch *m_switch_manager;
	CDIPCVNetProcess *m_process;
	VLANMAPType m_vlanmap;
};

#endif//CDIPCVNETSWITCHPORTLOCAL_H