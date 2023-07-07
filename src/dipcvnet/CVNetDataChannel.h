#ifndef CDIPCVNET_DATACHANNEL_H
#define CDIPCVNET_DATACHANNEL_H
#include <dipc/dipc.h>
#include <dipc/IDIPCDataChannel.h>
#include "IDIPCVNetDataChannel.h"
using namespace dipc;

class CDIPCVNetProcess;
class CVNetDataChannel : public IDIPCDataChannelSink
	,public IDIPCVNetDataChannel
	, public CAWReferenceControlSingleThread
{
public:
	CVNetDataChannel(CDIPCVNetProcess *pprocess);
	virtual ~CVNetDataChannel();
	virtual CAWResult Close();
	virtual CAWResult Open(const CAWInetAddr& localaddr);
    virtual void OnDataRcv(CAWMessageBlock& msg, 
        const CDIPCAddress& fromaddr,
        const CAWInetAddr& aAddrPeer);
	virtual CAWResult SendData(const char* pkt, size_t pktsize, const CDIPCAddress& peeraddr);
	virtual DWORD AddReference();
	virtual DWORD ReleaseReference();
private:
	CDIPCVNetProcess* m_pprocess;
	IDIPCDataChannel * m_channel;
};

#endif//CDIPCVNET_DATACHANNEL_H