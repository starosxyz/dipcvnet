#ifndef CDIPCVNETSESSION_H
#define CDIPCVNETSESSION_H
#include "IDIPCVnetSession.h"
#include <dipc/dipc.h>
#include <dipcutils/CDIPCApp.h>
#include "IDIPCProbe.h"
#include "IDIPCVNetDataChannel.h"
#include "IDIPCVNetPolicy.h"
using namespace dipc;

using namespace dipc::utils;
class CDIPCVNetProcess;
class CDIPCVnetSession : public IDIPCVnetSession,
	public CAWReferenceControlSingleThread
{
public:
	CDIPCVnetSession(CDIPCVNetProcess *pprocess, 
		const CDIPCAddress &app,
		uint32_t sessionid);
	virtual ~CDIPCVnetSession();

	/* interface */
	virtual CAWResult OnUplayerRecvPacket(const char* pkt, size_t pktsize)=0;
	virtual CAWResult LowlayerSendPacket(const char* pkt, size_t pktsize);
public:
	virtual CAWResult Init();
	CAWResult CleanUp();
    virtual CAWResult SendPacket(const char *pkt, size_t pktsize);
    virtual CAWResult OnRecvPacket(const char *pkt, size_t pktsize);
	virtual CAWResult SendData(const char* pdata, size_t pktsize);
    virtual DWORD AddReference();
    virtual DWORD ReleaseReference();

    inline CDIPCVNetProcess *GetVNetProcess() { return m_process; };
	inline const CDIPCAddress& GetLocalAddress() const { return m_localaddr; };
	inline const CDIPCAddress& GetPeerAddress() const { return m_peeraddr; };
	//inline const CDIPCApp& GetDIPCApp() const { return m_peerapp; }
    inline void SetSessionId(const uint32_t &session_id) { m_session_id = session_id; }
    inline const uint32_t& GetSessionId() const { return m_session_id; }
	inline IDIPCVNetDataChannel* GetDataChannel() { return m_pdatachannel.Get(); }
protected:
	CDIPCVNetProcess *m_process;
	CDIPCAddress m_peeraddr;
	CDIPCAddress m_localaddr;
    uint32_t m_session_id;
	IDIPCVNetPolicy* m_policy;
	CAWAutoPtr<IDIPCVNetDataChannel> m_pdatachannel;
};

#endif