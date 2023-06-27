#ifndef CDIPCVNETSWITCHPORT_H
#define CDIPCVNETSWITCHPORT_H
#include "IVSwitch.h"
#include "CDIPCVNetProcess.h"
using namespace bxjstack::vswitch;

class CDIPCVnetSwhichSink: public IVSwitchSink
{
public:
	CDIPCVnetSwhichSink(const CDIPCAddress peer_address, 
        CDIPCVNetProcess *process, IVSwitch *switch_manager);
    virtual ~CDIPCVnetSwhichSink();
	
public:
    virtual void OnOutput(IVSwitchSink *from,
        const char *rcvdata,
        size_t datasize,
        bool flood);
    virtual void OnStateChange(int state);

public:
	CAWResult InputData(const char *pdata,size_t datasize);
    
private:
	CDIPCAddress m_peer_address;
	CDIPCVNetProcess *m_process;
    IVSwitch *m_switch_manager;
};

#endif//CDIPCVNETSWITCHPORT_H