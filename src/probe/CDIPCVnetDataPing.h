/***********************************************************************
    Copyright (C) 2018-2023 南京北星极网络科技有限公司
**********************************************************************/
#ifndef CDIPCVNET_DATA_PING_H
#define CDIPCVNET_DATA_PING_H
#include "dipc/dipc.h"
#include "IDIPCProbe.h"
#include "IDIPCVNetDataChannel.h"
using namespace dipc;

class CDIPCVnetDataPing : public IDIPCProbe
{
public:
	CDIPCVnetDataPing(const CDIPCAddress& App,
		IDIPCVNetDataChannel *pchannel);
	virtual ~CDIPCVnetDataPing();
	/* probe interface */
	virtual CAWResult SendPingRequest();
	virtual CAWResult HandlePingResponse(const char* pkt, size_t pktsize);
	virtual CAWResult HandlePingRequest(const char* pkt, size_t pktsize);
	/*
	 * RTT（ Round Trip Time)
	 * client send ping1 ,record the timestap T1,
	 * when ping response ,it is T2,so
	 * RTT=T2-T1
	 */
	virtual uint64_t GetRTT() const;
private:
	CDIPCAddress m_App;
	IDIPCVNetDataChannel *m_datachannel;
	uint64_t m_rtt;
	uint64_t m_lastms;
	uint32_t m_lastseq;
};	

#endif  //CDIPCVNET_DATA_PING_H


