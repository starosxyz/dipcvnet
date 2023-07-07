
#ifndef CDIPCVNETVOSPORTEXT_H
#define CDIPCVNETVOSPORTEXT_H

#include "dipcvnet/CDIPCVnetSession.h"
#include "dipcvnet/CDIPCVOSPort.h"


class CDIPCVnetVOSManager;
class CDIPCVnetVOSPortExt
{
public:
    CDIPCVnetVOSPortExt(CDIPCVnetVOSManager* mgr);
    virtual ~CDIPCVnetVOSPortExt();
public:
    virtual CAWResult Init();
    virtual CAWResult Output(const char* pkt, size_t pktsize);
    void Input(const char* data, size_t datasize);
private:
    struct vos_veth* m_if;
    CDIPCVnetVOSManager* m_pmgr;
    CAWString m_ifname;
};


#endif//CDIPCVNETVOSSESSION_H

