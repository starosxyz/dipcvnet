#ifndef IDIPCVNETSESSION_H
#define IDIPCVNETSESSION_H
#include "dipc/dipc.h"

class IDIPCVnetSession: public IAWReferenceControl
{
public:
	virtual CAWResult SendData(const char* pkt, size_t pktsize) = 0;
	virtual CAWResult SendPacket(const char *pkt, size_t pktsize) = 0;
	virtual CAWResult OnRecvPacket(const char *pkt, size_t pktsize) = 0;
protected:
	virtual ~IDIPCVnetSession(){}
};

#endif