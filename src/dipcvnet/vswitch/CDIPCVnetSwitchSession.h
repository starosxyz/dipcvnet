#ifndef CDIPCVNETLANSESSION_H
#define CDIPCVNETLANSESSION_H
#include "IDIPCVSwitch.h"
#include "dipcvnet/CDIPCVnetSession.h"
//#include "dipcvnet/vswitch/CDIPCVnetSwitchPort.h"

class CDIPCVnetSwitchManager;
class CDIPCVNetProcess;
class CDIPCVnetSwitchSession: public CDIPCVnetSession
	,public IDIPCVSwitchSink
{
public:
    CDIPCVnetSwitchSession(CDIPCVNetProcess *pprocess, 
        const CDIPCAddress &app,
        CDIPCVnetSwitchManager*switch_manager);
    virtual ~CDIPCVnetSwitchSession();
    virtual void OnStateChange(int state);

    virtual CAWResult Init();
public:
    virtual CAWResult OnUplayerRecvPacket(const char *pkt, size_t pktsize);
    //virtual CAWResult Send(const char* pkt, size_t pktsize);
public:
    virtual void OnOutput(IDIPCVSwitchSink *from,
        const char *rcvdata,
        size_t datasize,
        bool flood);
    CAWResult InputData(const char *pdata,size_t datasize);
private:
    CDIPCVnetSwitchManager* m_pswitchmgr;
};

#endif //CDIPCVNETLANSESSION_H
