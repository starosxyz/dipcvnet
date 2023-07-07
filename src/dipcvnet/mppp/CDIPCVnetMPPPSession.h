#ifndef CDIPCVNETMPPPSESSION_H
#define CDIPCVNETMPPPSESSION_H

#include "dipcvnet/CDIPCVnetSession.h"
class CDIPCVnetMPPPManager;
class CDIPCVnetMPPPSession: public CDIPCVnetSession
{
public:
    CDIPCVnetMPPPSession(CDIPCVNetProcess *pprocess, 
        const CDIPCAddress &app,
        CDIPCVnetMPPPManager *switch_manager);
    virtual ~CDIPCVnetMPPPSession();

    virtual CAWResult OnUplayerRecvPacket(const char *pkt, size_t pktsize);

    //virtual CAWResult Send(const char* pkt, size_t pktsize);
public:
    CDIPCVnetMPPPManager* m_mpppmgr;
};

#endif //CDIPCVNETMPPPSESSION_H
