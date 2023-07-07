#ifndef IDIPCVNETRROBE_H
#define IDIPCVNETRROBE_H
#include "dipc/dipc.h"

class IDIPCProbe
{
public:
	virtual CAWResult SendPingRequest() = 0;
	virtual CAWResult HandlePingResponse(const char *pkt, size_t pktsize) = 0;
	virtual CAWResult HandlePingRequest(const char* pkt, size_t pktsize) = 0;
	/* 
	 * RTT（ Round Trip Time)
	 * client send ping1 ,record the timestap T1, 
	 * when ping response ,it is T2,so 
	 * RTT=T2-T1
	 */
	virtual uint64_t GetRTT() const = 0;
	virtual ~IDIPCProbe(){}
};

#endif