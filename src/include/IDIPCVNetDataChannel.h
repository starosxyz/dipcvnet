#ifndef IDIPCVNETDATACHANNEL_H
#define IDIPCVNETDATACHANNEL_H
#include "dipc/dipc.h"

class IDIPCVNetDataChannel: public IAWReferenceControl
{
public:
	virtual CAWResult Open(const CAWInetAddr& localaddr) = 0;
	virtual CAWResult Close() = 0;
	virtual CAWResult SendData(const char* pkt, size_t pktsize, const CDIPCAddress& peeraddr) = 0;
protected:
	virtual ~IDIPCVNetDataChannel(){}
};

#endif//IDIPCVNETDATACHANNEL_H