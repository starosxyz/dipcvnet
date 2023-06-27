#ifndef CDIPCVNETSWITCHPORTLOCAL_H
#define CDIPCVNETSWITCHPORTLOCAL_H
#include "IVSwitch.h"
#include "CDIPCVNetProcess.h"
using namespace bxjstack::vswitch;

class CDIPCVnetSwhichSinklocal: public IVSwitchSink
    ,public CAWTap
{
public:
	CDIPCVnetSwhichSinklocal(IVSwitch* switch_manager);
    virtual ~CDIPCVnetSwhichSinklocal();
public:

    //IVSwitchSink OnOutput input to Tap
    virtual void OnOutput(IVSwitchSink *from,
        const char *rcvdata,
        size_t datasize,
        bool flood);

	////CAWTap OnOutput input to switch_manager
	virtual void OnOutput(const char* pkt, size_t pktsize);

    virtual void OnStateChange(int state);
    
public:
	CAWResult Tap_init();
private:
	IVSwitch* m_switch_manager;
	bool m_tap_init;
};

#endif//CDIPCVNETSWITCHPORTLOCAL_H