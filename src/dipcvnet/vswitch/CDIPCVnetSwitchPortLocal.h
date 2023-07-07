#ifndef CDIPCVNETSWITCHPORTLOCAL_H
#define CDIPCVNETSWITCHPORTLOCAL_H
#include "IDIPCVSwitch.h"
#include "main/CDIPCVNetProcess.h"
#include "starbase/CAWTap.h"

using namespace starbase;

class CDIPCVnetSwitchManager;
class CDIPCVnetSwitchPortLocal: public IDIPCVSwitchSink,public CAWTap
{
public:
	CDIPCVnetSwitchPortLocal(CDIPCVnetSwitchManager* switch_manager);
    virtual ~CDIPCVnetSwitchPortLocal();
public:

    //IDIPCVSwitchSink OnOutput input to Tap
    virtual void OnOutput(IDIPCVSwitchSink *from,
        const char *rcvdata,
        size_t datasize,
        bool flood);

	////CAWTap OnOutput input to switch_manager
	virtual void OnOutput(const char* pkt, size_t pktsize);

    virtual void OnStateChange(int state);
    
public:
	CAWResult Tap_init();
private:
    CDIPCVnetSwitchManager* m_switch_manager;
	bool m_tap_init;
};

#endif//CDIPCVNETSWITCHPORTLOCAL_H