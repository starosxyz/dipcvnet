#include "CDIPCVnetDataPing.h"
#include "dipcutils/DIPCPdu.h"
#include "dipcvnet/CVNetDataChannel.h"
#include "VNETPdu.h"
#include "utils/dipcvnetutils.h"

CDIPCVnetDataPing::CDIPCVnetDataPing(const CDIPCAddress& App,
	IDIPCVNetDataChannel* pchannel)
	:m_App(App)
	,m_datachannel(pchannel)
	, m_rtt(0)
	, m_lastms(0)
	, m_lastseq(0)
{
	CAW_INFO_TRACE_THIS("CDIPCVnetDataPing::CDIPCVnetDataPing");
}
CDIPCVnetDataPing::~CDIPCVnetDataPing()
{
	CAW_INFO_TRACE_THIS("CDIPCVnetDataPing::~CDIPCVnetDataPing");
}

CAWResult CDIPCVnetDataPing::SendPingRequest()
{
	CAW_INFO_TRACE("CDIPCVnetDataPing::SendPingRequest() pktsize" << sizeof(VNetKeepAlivePdu));
	m_lastseq++;
	VNetKeepAlivePdu request;
	::memset(&request, 0, sizeof(request));
	m_lastms = GetCurrentMs();
	request.timestamp = caw_htonll(m_lastms);

	request.head.pdutype = htons(DIPC_VNET_KEEPALIVE_REQUEST);
	request.head.pdulenth = htons(sizeof(VNetKeepAlivePdu));
	request.head.xid = htonl(m_lastseq);

	CDIPCApp peeraddr;



	CAWResult rv = m_datachannel->SendData((char*) & request, sizeof(request), m_App);
	if(CAW_FAILED(rv))
		return CAW_ERROR_FAILURE;
	CAW_INFO_TRACE("CDIPCVnetDataPing::SendPingRequest() pdulenth" << sizeof(VNetKeepAlivePdu)
		<< " timestamp" << m_lastms << " xid" << m_lastseq);
	return CAW_OK;
}
CAWResult CDIPCVnetDataPing::HandlePingResponse(const char* pkt, size_t pktsize)
{
	CAW_INFO_TRACE("CDIPCVnetDataPing::HandlePingResponse() pktsize" << pktsize);
	if(pktsize < sizeof(VNetKeepAlivePdu))
	{
		return CAW_ERROR_FAILURE;
	}
	VNetKeepAlivePdu *response=(VNetKeepAlivePdu * )pkt;
	uint16_t pdulenth = ntohs(response->head.pdulenth);
	if (pktsize != sizeof(VNetKeepAlivePdu)
		|| pktsize != pdulenth)
	{
		return CAW_ERROR_FAILURE;
	}
	if (ntohs(response->head.pdutype) != DIPC_VNET_DATA_KEEPALIVE_RESPONSE
		|| ntohl(response->head.xid) != m_lastseq)
	{
		return CAW_ERROR_FAILURE;
	}
	uint64_t currentms = GetCurrentMs();
	m_rtt = currentms - m_lastms;
	CAW_INFO_TRACE("CDIPCVnetDataPing::HandlePingResponse() pdulenth" << sizeof(VNetKeepAlivePdu)
		<< " timestamp" << m_lastms << " xid" << m_lastseq << " m_rtt" << m_rtt);
	return CAW_OK;
}

CAWResult CDIPCVnetDataPing::HandlePingRequest(const char* pkt, size_t pktsize)
{
	CAW_INFO_TRACE("CDIPCVnetDataPing::HandlePingRequest() pktsize"<< pktsize);
	if (pktsize < sizeof(VNetKeepAlivePdu))
	{
		return CAW_ERROR_FAILURE;
	}
	VNetKeepAlivePdu* response = (VNetKeepAlivePdu*)pkt;
	uint16_t pdulenth = ntohs(response->head.pdulenth);
	if (pktsize != sizeof(VNetKeepAlivePdu)
		|| pktsize != pdulenth)
	{
		return CAW_ERROR_FAILURE;
	}
	if (ntohs(response->head.pdutype) != DIPC_VNET_DATA_KEEPALIVE_REQUET)
	{
		return CAW_ERROR_FAILURE;
	}
	response->head.pdutype = htons(DIPC_VNET_DATA_KEEPALIVE_RESPONSE);
	CAWResult rv = m_datachannel->SendData((char*)&response, sizeof(response), m_App);
	if (CAW_FAILED(rv))
		return CAW_ERROR_FAILURE;
	CAW_INFO_TRACE("CDIPCVnetDataPing::HandlePingRequest() pdulenth" << sizeof(VNetKeepAlivePdu)
		<< " xid" << ntohl(response->head.xid));
	return CAW_OK;

}

/*
 * RTT（ Round Trip Time)
 * client send ping1 ,record the timestap T1,
 * when ping response ,it is T2,so
 * RTT=T2-T1
 */
uint64_t CDIPCVnetDataPing::GetRTT() const
{
	return m_rtt;
}