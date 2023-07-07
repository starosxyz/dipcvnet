#include "CVNetDataChannel.h"
#include "main/CDIPCVNetProcess.h"
CVNetDataChannel::CVNetDataChannel(CDIPCVNetProcess* pprocess)
	:m_pprocess(pprocess),
	m_channel(pprocess->GetProcess()->GetDataChannel())
{
	CAW_INFO_TRACE_THIS("CVNetDataChannel::CVNetDataChannel");
}
CVNetDataChannel::~CVNetDataChannel()
{
	CAW_INFO_TRACE_THIS("CVNetDataChannel::~CVNetDataChannel");
	if (m_channel)
	{
		m_channel->CloseDataChannel();
		m_channel = NULL;
	}
}
CAWResult CVNetDataChannel::Close()
{
	CAW_INFO_TRACE_THIS("CVNetDataChannel::Close");
	if (m_channel)
	{
		m_channel->CloseDataChannel();
		m_channel = NULL;
	}
    return CAW_OK;
}
CAWResult CVNetDataChannel::Open(const CAWInetAddr& localaddr)
{
	CAW_INFO_TRACE_THIS("CVNetDataChannel::Open localaddr "<< localaddr.GetIpDisplayName());
	m_channel = m_pprocess->GetProcess()->GetDataChannel();
	if (m_channel == NULL)
	{
		return CAW_ERROR_FAILURE;
	}
	if (m_channel->OpenDataChannel(this, localaddr) != CAW_OK)
	{
		CAW_ERROR_TRACE_THIS("CVNetDataChannel::Open OpenDataChannel localaddr " << localaddr.GetIpDisplayName());
		return CAW_ERROR_FAILURE;
	}
	return CAW_OK;
}

void CVNetDataChannel::OnDataRcv(CAWMessageBlock& msg, 
        const CDIPCAddress& fromaddr,
        const CAWInetAddr& aAddrPeer)
{
	CAW_INFO_TRACE_THIS("CVNetDataChannel::OnDataRcv aAddrPeer " << aAddrPeer.GetIpDisplayName());
	if (m_pprocess)
	{
		m_pprocess->OnDataRcvFromPeer(msg, fromaddr);
	}
}
DWORD CVNetDataChannel::AddReference()
{
	return CAWReferenceControlSingleThread::AddReference();
}
DWORD CVNetDataChannel::ReleaseReference()
{
	return CAWReferenceControlSingleThread::ReleaseReference();
}
CAWResult CVNetDataChannel::SendData(const char* pkt, 
	size_t pktsize, 
	const CDIPCAddress& peeraddr)
{
	if (m_channel == NULL)
	{
		return CAW_ERROR_FAILURE;
	}
	CAWMessageBlock msg(
		pktsize,
		(char*)pkt,
		CAWMessageBlock::DONT_DELETE,
		pktsize
	);
    CDIPCApp App;
    m_pprocess->GetProcess()->GetApp(peeraddr.GetNodeAddress(), App);
	return m_channel->SendToAppPeer(msg,App,App.GetAppDataChannel());
}