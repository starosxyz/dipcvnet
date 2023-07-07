#ifndef CDIPCVNETSESSIONMANAGER_H
#define CDIPCVNETSESSIONMANAGER_H
#include "CDIPCVnetSession.h"
#include <dipc/dipc.h>
using namespace dipc;

typedef CAWHashMapT<DIPCAppKey,CAWAutoPtr<CDIPCVnetSession>,CDIPCAppKeyHashFun> SESSMapType;
class CDIPCVNetProcess;
class CDIPCVnetSessionManager
{
public:
    CDIPCVnetSessionManager(CDIPCVNetProcess *pprocess);
    ~CDIPCVnetSessionManager();
    
public:
	CAWResult AddSession(CAWAutoPtr<CDIPCVnetSession> &session, const CDIPCAddress &addr);
	CAWResult RemoveSession(const CDIPCAddress &addr);
	CDIPCVnetSession *GetSession(const CDIPCAddress &addr);
    void GetAllSession(std::list< CDIPCVnetSession*>& list);
private:
	SESSMapType m_sessions;
};


#endif//CDIPCVNETSESSIONMANAGER_H