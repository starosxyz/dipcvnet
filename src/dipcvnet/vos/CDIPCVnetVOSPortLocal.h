#ifndef CDIPCVNETVOSPORTLOCAL_H
#define CDIPCVNETVOSPORTLOCAL_H
#include "IDIPCVSwitch.h"
#include "main/CDIPCVNetProcess.h"
#include "starbase/CAWTap.h"

using namespace starbase;

class CDIPCVnetVOSManager;
class CDIPCVnetVOSPortLocal: public CAWTap
{
public:
	CDIPCVnetVOSPortLocal(CDIPCVnetVOSManager* switch_manager);
    virtual ~CDIPCVnetVOSPortLocal();
public:

	////CAWTap OnOutput input to switch_manager
	virtual void OnOutput(const char* pkt, size_t pktsize);
    
public:
	CAWResult Tap_init();
private:
    CDIPCVnetVOSManager* m_switch_manager;
	bool m_tap_init;
};

#endif//CDIPCVNETVOSPORTLOCAL_H