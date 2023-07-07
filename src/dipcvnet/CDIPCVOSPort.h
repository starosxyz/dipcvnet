#ifndef CDIPCVOSPORT_H
#define CDIPCVOSPORT_H
#include <starbase/CAWStarBase.h>
#include <voskern/voskern.h>
using namespace starbase;
class CDIPCVnetVOSSession;
class CDIPCVOSPort 
{
public:
	CDIPCVOSPort();
	virtual ~CDIPCVOSPort();
	CAWResult Init(CDIPCVnetVOSSession* psession,const CAWString &ifname);
	void Input(const char* data, size_t datasize);
	void Output(const char* data, size_t datasize);
private:
	struct vos_veth* m_if;
	CDIPCVnetVOSSession *m_psession;
	CAWString m_ifname;
};

#endif//CDIPCVOSPORT_H