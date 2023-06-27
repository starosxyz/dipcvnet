/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef CDIPCVNET_PROCESS_H
#define CDIPCVNET_PROCESS_H
#include <dipc/dipc.h>
#include "CDIPCVnetManager.h"
#include "CDIPCVnetSwitchManager.h"

using namespace dipc;
#include "IVSwitch.h"
using namespace bxjstack::vswitch;

#define DIPCVNet_JNO 333
//class C5GUEManager;
class CDIPCVNetProcess : public IDIPCProcessSink
{
    typedef int TType;
    enum 
    {
        TT_LAN,
        TT_P2P,
        TT_UNKNOWN = -1,
    };
public:
	CDIPCVNetProcess();
	virtual ~CDIPCVNetProcess();
	virtual void OnProcessUpdateState(const CDIPCProcess& updateprocess);
	virtual void OnProcessShutdown();
	virtual void OnBootOK();
	virtual void OnProcessRun(int argc, char** argv, IDIPCProcess* dipcProcess);
	virtual void OnNodeConnected(const CDIPCNode& node);
	virtual void OnPeerConnected(const CDIPCApp& peeraddr);
	virtual void OnNodeDisconnect(const CDIPCNode& node);
	virtual void OnPeerDisconnect(const CDIPCApp& peeraddr);
	virtual void OnPacketRcv(CAWMessageBlock& msg, const CDIPCAddress& fromaddr);
	virtual void OnPacketRcvFromPeerNode(CAWMessageBlock& msg, const CDIPCAddress& fromaddr);
	virtual void OnPersistentdataChange(const CAWString& strurl);
    virtual void SendToPeer(const char *pdata, size_t datasize, const CDIPCAddress peeraddr);
	inline IDIPCProcess *GetProcess() { return m_process; }

private:
	IDIPCProcess* m_process;
    TType m_mode;
	CDIPCVnetManager m_vnetmgr;
    CDIPCVnetSwhichManager m_lanmanager;
};

#endif//CDIPCVNET_PROCESS_H