#ifndef CDIPCVNET_TAP_H
#define CDIPCVNET_TAP_H
#include <dipc/dipc.h>
#include <starbase/CAWTap.h>
#include "starbase/CAWACEInclude.h"
#include "wface/CAWACEWrapper.h"
#include "wface/CAWConnectionInterface.h"
#include "starbase/CAWByteStream.h"
#include "starbase/CAWUtils.h"
#include "dipcutils/CDIPCAddress.h"

using namespace dipc;
using namespace starbase;

class CDIPCVNetProcess;
class CDIPCVnetTap: public CAWTap
{
public:
	CDIPCVnetTap(const CDIPCAddress peer_address, CDIPCVNetProcess *process);
	virtual ~CDIPCVnetTap();
	virtual void OnOutput(const char* pkt, size_t pktsize);
protected:
    CDIPCAddress m_peer_address;
    CDIPCVNetProcess *m_process;
};

#endif /*CDIPCVNET_TAP_H*/