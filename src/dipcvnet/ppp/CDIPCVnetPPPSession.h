
#ifndef CDIPCVNETPPPSESSION_H
#define CDIPCVNETPPPSESSION_H

#include "dipcvnet/CDIPCVnetSession.h"
#include "dipcvnet/CDIPCVnetTap.h"
#include "starbase/CAWStarBase.h"
#include "starbase/CAWTap.h"

class CDIPCVNetProcess;
class CDIPCVnetPPPManager;
class CDIPCVnetPPPSession : public CDIPCVnetSession,public CAWTap
{
public:
    CDIPCVnetPPPSession(CDIPCVNetProcess* pprocess,
        const CDIPCAddress& app,
        CDIPCVnetPPPManager* pmgr);
    virtual ~CDIPCVnetPPPSession();

    CAWResult SessionInit();
public:

    /* CDIPCVnetSession interface */
    //virtual CAWResult Send(const char* pkt, size_t pktsize);
    virtual CAWResult OnUplayerRecvPacket(const char *pkt, size_t pktsize);

    /* tap interface */
    virtual void OnOutput(const char* pkt, size_t pktsize);
private:
    CDIPCVnetPPPManager* m_pmgr;
};

#endif //CDIPCVNETPPPSESSION_H
