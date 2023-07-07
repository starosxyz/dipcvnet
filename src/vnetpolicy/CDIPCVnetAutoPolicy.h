
#ifndef CDIPCVNETAUTOPOLICY_H
#define CDIPCVNETAUTOPOLICY_H
#include "IDIPCVNetPolicy.h"
#include "dipcvnet/CDIPCVnetSession.h"

class CDIPCVnetAutoPolicy : public CAWTimerWrapperIDSink,public IDIPCVNetPolicy
{
public:
    CDIPCVnetAutoPolicy(CDIPCVnetSession *psession);
    virtual ~CDIPCVnetAutoPolicy();
    
public:
    virtual CAWResult Init();
    virtual CAWResult PolicySend(const char* pkt, size_t pktsize);
    virtual CAWResult PolicyRecv(const char* pkt, size_t pktsize);
public:
    virtual void OnTimer(CAWTimerWrapperID* aId);
    CAWResult StartProbeTimer(uint32_t ms);
    CAWResult StopProbeTimer();
private:
    CDIPCVnetSession *m_psession;
    IDIPCProbe* m_dataprobe;
    IDIPCProbe* m_dipcprob;
    CAWTimerWrapperID m_timer;
    uint32_t m_timeroutms;
};


#endif//CDIPCVNETAUTOPOLICY_H

