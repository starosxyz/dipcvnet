/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef CDIPCVNET_PROCESS_H
#define CDIPCVNET_PROCESS_H
#include <dipc/dipc.h>
#include "IDIPCVNet.h"
#include "utils/CWhiteList.h"
#include "utils/CBlackList.h"

using namespace dipc;

class CDIPCVNetProcess : public IDIPCProcessSink
{
public:
	CDIPCVNetProcess();
	virtual ~CDIPCVNetProcess();
	CAWResult InitVNet();
public:
	/* dipc interace */
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
    CAWResult SendToPeer(const char *pdata, size_t datasize, const CDIPCAddress peeraddr);
	inline IDIPCProcess *GetProcess() { return m_process; }
	void OnDataRcvFromPeer(CAWMessageBlock& msg, const CDIPCAddress& fromaddr);
private:
	IDIPCProcess* m_process;
	IDIPCVNet* m_vnet;
	CWhiteList m_white_list;
	CBlackList m_black_list;
};

#endif//CDIPCVNET_PROCESS_H