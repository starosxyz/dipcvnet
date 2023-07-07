/***********************************************************************
    Copyright (C) 2018-2023 南京北星极网络科技有限公司
**********************************************************************/
#ifndef CDIPCVNET_DIPC_PING_H
#define CDIPCVNET_DIPC_PING_H
#include "dipc/dipc.h"
#include "dipcutils/CDIPCApp.h"
#include "IDIPCProbe.h"

class CDIPCVNetProcess;

using namespace dipc::utils;
class CDIPCVNetDIPCPing : public IDIPCProbe
{
public:
	CDIPCVNetDIPCPing(CDIPCVNetProcess* pProcess, const CDIPCAddress& App);
	virtual ~CDIPCVNetDIPCPing();
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
	CDIPCVNetProcess* m_Process;
	CDIPCApp m_App;
	uint64_t m_rtt;
	uint64_t m_lastms;
	uint32_t m_lastseq;
};


#endif  //CDIPCVNET_DIPC_PING_H


