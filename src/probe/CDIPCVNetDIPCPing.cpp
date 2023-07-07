#include "CDIPCVNetDIPCPing.h"
#include "dipcutils/DIPCPdu.h"
#include "main/CDIPCVNetProcess.h"
#include "VNETPdu.h"
#include "utils/dipcvnetutils.h"

CDIPCVNetDIPCPing::CDIPCVNetDIPCPing(CDIPCVNetProcess* pProcess, const CDIPCAddress& App)
	:m_Process(pProcess)
	,m_App(App)
	, m_lastms(0)
	, m_lastseq(0)
{
	CAW_INFO_TRACE_THIS("CDIPCVNetDIPCPing::CDIPCVNetDIPCPing");
}
CDIPCVNetDIPCPing::~CDIPCVNetDIPCPing()
{
	CAW_INFO_TRACE_THIS("CDIPCVNetDIPCPing::~CDIPCVNetDIPCPing");
}

CAWResult CDIPCVNetDIPCPing::SendPingRequest()
{
	CAW_INFO_TRACE("CDIPCVNetDIPCPing::SendPingRequest() pktsize" << sizeof(VNetKeepAlivePdu));
	m_lastseq++;
	VNetKeepAlivePdu request;
	::memset(&request, 0, sizeof(request));

	m_lastms = GetCurrentMs();
	request.timestamp = caw_htonll(m_lastms);

	request.head.pdutype = htons(DIPC_VNET_KEEPALIVE_REQUEST);
	request.head.pdulenth = htons(sizeof(VNetKeepAlivePdu));
	request.head.xid = htonl(m_lastseq);

	CAW_INFO_TRACE("CDIPCVNetDIPCPing::SendPingRequest() pdulenth" << sizeof(VNetKeepAlivePdu)
		<< " timestamp" << m_lastms << " xid" << m_lastseq);
	m_Process->SendToPeer((char*)&request, sizeof(request), m_App.GetDIPCAddress());
	return CAW_OK;
}
CAWResult CDIPCVNetDIPCPing::HandlePingResponse(const char* pkt, size_t pktsize)
{
	CAW_INFO_TRACE("CDIPCVNetDIPCPing::HandlePingResponse() pktsize" << pktsize);
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
	if (ntohs(response->head.pdutype) != DIPC_VNET_KEEPALIVE_RESPONSE
		|| ntohl(response->head.xid) != m_lastseq)
	{
		return CAW_ERROR_FAILURE;
	}
	uint64_t currentms = GetCurrentMs();
	m_rtt = currentms - m_lastms;
	CAW_INFO_TRACE("CDIPCVNetDIPCPing::HandlePingResponse() pdulenth" << sizeof(VNetKeepAlivePdu)
		<< " timestamp" << m_lastms << " xid" << m_lastseq << " m_rtt" << m_rtt);
	return CAW_OK;
}

CAWResult CDIPCVNetDIPCPing::HandlePingRequest(const char* pkt, size_t pktsize)
{
	CAW_INFO_TRACE("CDIPCVNetDIPCPing::HandlePingRequest() pktsize" << pktsize);
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
	if (ntohs(response->head.pdutype) != DIPC_VNET_KEEPALIVE_REQUEST)
	{
		return CAW_ERROR_FAILURE;
	}

	response->head.pdutype = htons(DIPC_VNET_KEEPALIVE_RESPONSE);
	m_Process->SendToPeer((char*)&response, sizeof(response), m_App.GetDIPCAddress());
	CAW_INFO_TRACE("CDIPCVNetDIPCPing::HandlePingRequest() pdulenth" << sizeof(VNetKeepAlivePdu)
		<< " xid" << ntohl(response->head.xid));
	return CAW_OK;

}

/*
 * RTT�� Round Trip Time)
 * client send ping1 ,record the timestap T1,
 * when ping response ,it is T2,so
 * RTT=T2-T1
 */
uint64_t CDIPCVNetDIPCPing::GetRTT() const
{
	return m_rtt;
}