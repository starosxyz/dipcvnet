#include "main/CDIPCVNetProcess.h"
#include "dipcvnet/CDIPCVnetSessionManager.h"
#include "utils/dipcvnetutils.h"
#include "main/CDIPCVNetProcess.h"
CDIPCVnetSessionManager::CDIPCVnetSessionManager(CDIPCVNetProcess* pprocess)
{
}
CDIPCVnetSessionManager::~CDIPCVnetSessionManager()
{
}

CAWResult CDIPCVnetSessionManager::AddSession(CAWAutoPtr<CDIPCVnetSession> &session,
        const CDIPCAddress &addr)
{
    DIPCAppKey key;
    ConvertKey(addr,&key);

    SESSMapType::iterator it= m_sessions.find(key);
    if (it != m_sessions.end())
    {
        CAW_ERROR_TRACE("CDIPCVnetSessionManager::AddSession m_sessions.find(key) != NULL");
        return CAW_ERROR_FAILURE;
    }
    m_sessions[key] = session;
    CAW_INFO_TRACE("CDIPCVnetSessionManager::AddSession, CDIPCVnetSession= "
                <<(void*)(m_sessions[key].Get()));
    return CAW_OK;
}
    
CAWResult CDIPCVnetSessionManager::RemoveSession(const CDIPCAddress &addr)
{
    DIPCAppKey key;
    ConvertKey(addr,&key);
    
    SESSMapType::iterator it= m_sessions.find(key);
    if (it == m_sessions.end())
    {
        CAW_ERROR_TRACE("CDIPCVnetSessionManager::RemoveSesion, m_sessions.find(key) == NULL");
        return CAW_ERROR_FAILURE;
    }
    m_sessions.erase(it);
    return CAW_OK;
}

CDIPCVnetSession *CDIPCVnetSessionManager::GetSession(const CDIPCAddress &addr)
{
    DIPCAppKey key;
    ConvertKey(addr,&key);

    SESSMapType::iterator it= m_sessions.find(key);
    if (it == m_sessions.end())
    {
        CAW_ERROR_TRACE("CDIPCVnetSessionManager::GetSession, m_sessions.find(key) == NULL");
        return NULL;
    }
    CAWAutoPtr<CDIPCVnetSession> &sesson=it->second;

    return sesson.Get();
}

void CDIPCVnetSessionManager::GetAllSession(std::list< CDIPCVnetSession*>& list)
{
    SESSMapType::iterator it = m_sessions.begin();
    while (it != m_sessions.end())
    {
        list.push_back((it->second).Get());
        it++;
    }
}

#if 0
CAWResult CDIPCVnetSessionManager::CreateSession(CDIPCVNetProcess* pprocess,
        const CDIPCApp &App)
{
    CDIPCVnetSelectSession* pP2PSession = new CDIPCVnetSelectSession(pprocess,App);
    if(!pP2PSession)
    {
        CAW_ERROR_TRACE("CDIPCVnetSessionManager::CreateSession new memory error");
        return CAW_ERROR_FAILURE;
    }
    
    CAWResult rv = pP2PSession->SessionInit();
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSessionManager::CreateSession SessionInit failed");
        return CAW_ERROR_FAILURE;
    }
    
    if(!m_session_list.empty())
    {
        rv = pP2PSession->StartProbeTimer(1000);
        if(CAW_FAILED(rv))
        {
            CAW_ERROR_TRACE("CDIPCVnetSessionManager::CreateSession StartProbeTimer failed");
            return CAW_ERROR_FAILURE;
        }
    }
    uint32_t sessionid = 0;
    for(int i=0; i<=m_session_list.size(); i++)
    {
        SESSListType::iterator it= std::find(m_session_list.begin(),m_session_list.end(),i);
        if (it == m_session_list.end())
        {
            m_session_list.push_front(i);
            sessionid = i;
            break;
        }
    }
    
    pP2PSession->SetSessionId(sessionid);
    CDIPCVnetSession* pVnetSession = (CDIPCVnetSession*)pP2PSession;
    CAWAutoPtr<CDIPCVnetSession> session(pVnetSession);
    
    rv = AddSession(session,App.GetDIPCAddress());
    {
        CAW_ERROR_TRACE("CDIPCVnetSessionManager::CreateSession AddSession failed");
        return CAW_ERROR_FAILURE;
    }

}
#endif