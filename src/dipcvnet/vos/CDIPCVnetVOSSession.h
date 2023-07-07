
#ifndef CDIPCVNETVOSSESSION_H
#define CDIPCVNETVOSSESSION_H

#include "dipcvnet/CDIPCVnetSession.h"
#include "dipcvnet/CDIPCVOSPort.h"


class CDIPCVNetProcess;
class CDIPCVnetVOSManager;
class CDIPCVnetVOSSession : public CDIPCVnetSession
{
public:
    CDIPCVnetVOSSession(CDIPCVNetProcess *pprocess,
        const CDIPCAddress &app,
        CDIPCVnetVOSManager *mgr);
    virtual ~CDIPCVnetVOSSession();
    
public:
    virtual CAWResult Init();
    //virtual CAWResult Send(const char* pkt, size_t pktsize);
    virtual CAWResult OnUplayerRecvPacket(const char* pkt, size_t pktsize);
private:
    bool m_blocal_session;
    CDIPCVnetVOSManager *m_mgr;
    CDIPCVOSPort m_vosport;
};


#endif//CDIPCVNETVOSSESSION_H

