
#ifndef IDIPCVNETPOLICY_H
#define IDIPCVNETPOLICY_H
#include "dipc/dipc.h"

class IDIPCVNetPolicy
{
public:
    virtual ~IDIPCVNetPolicy() {}
    virtual CAWResult Init()=0;
    virtual CAWResult PolicySend(const char* pkt, size_t pktsize)=0;
    virtual CAWResult PolicyRecv(const char* pkt, size_t pktsize)=0;
};


#endif//IDIPCVNETPOLICY_H

