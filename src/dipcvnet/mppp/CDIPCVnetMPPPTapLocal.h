#ifndef CDIPCVNETMPPPTAPLOCAL_H
#define CDIPCVNETMPPPTAPLOCAL_H
#include "starbase/CAWStarBase.h"
#include "starbase/CAWTap.h"
#include "MacHashFun.h"
#include "IDIPCVSwitch.h"
using namespace starbase;

class CDIPCVnetMPPPManager;
class CDIPCVNetProcess;
class CDIPCVnetSession;
class CDIPCVnetMPPPTapLocal:public CAWTap
{
public:
	CDIPCVnetMPPPTapLocal(CDIPCVnetMPPPManager* switch_manager);
    virtual ~CDIPCVnetMPPPTapLocal();
public:

	virtual void OnOutput(const char* pkt, size_t pktsize);
	void TapInput(const char* pkt, 
		size_t pktsize,
		CDIPCVnetSession* psession);
public:
	CAWResult Tap_init();
	void ClearMacTable();
private:
	bool m_tap_init;
	std::unordered_map<MacKey, CDIPCVnetSession*, MacKeyHashFun> m_macTable;
	CDIPCVnetMPPPManager* m_mpppmgr;
	CAWString m_strTapName;
	uint32_t m_ipaddr;
	char m_mac[ETHERNET_MAC_LENGTH];
};

#endif//CDIPCVNETMPPPTAPLOCAL_H