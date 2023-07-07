#ifndef IDIPCVNET_H
#define IDIPCVNET_H
#include "dipc/dipc.h"

class IDIPCVnetSession;
class IDIPCVNet
{
public:
	virtual CAWResult Init() = 0;
	virtual CAWResult VNetAddSession(const CDIPCAddress &app) = 0;
	virtual CAWResult VNetRemoveSession(const CDIPCAddress &app) = 0;
    virtual IDIPCVnetSession * VNetGetSession(const CDIPCAddress &addr) = 0;
	virtual ~IDIPCVNet(){}
};

#endif