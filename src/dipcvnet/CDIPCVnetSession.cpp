#include "main/CDIPCVNetProcess.h"
#include "dipcvnet/CDIPCVnetSession.h"
#include "main/CDIPCVNetConfig.h"
#include "utils/DIPCVNetPolicyType.h"
#include "vnetpolicy/CDIPCVnetAutoPolicy.h"
#include "dipcvnet/CVNetDataChannel.h"

CDIPCVnetSession::CDIPCVnetSession(CDIPCVNetProcess* pprocess,
    const CDIPCAddress& app,
    uint32_t sessionid)
    :m_process(pprocess),
    m_peeraddr(app),
    m_session_id(sessionid)
    , m_policy(NULL)
{
    if(m_process)
        m_process->GetProcess()->GetLocalDIPCAddr(m_localaddr);
}
CDIPCVnetSession::~CDIPCVnetSession()
{
    if (m_policy)
    {
        delete m_policy;
        m_policy = NULL;
    }
    if (m_pdatachannel.Get())
    {
        m_pdatachannel->Close();
        m_pdatachannel = NULL;
    }
}

CAWResult CDIPCVnetSession::CleanUp()
{
    return CAW_OK;
}
CAWResult CDIPCVnetSession::Init()
{
    CAWString strpolicy = "auto";

    DIPCVNetPolicyType type = StringToVNetPolicy(strpolicy);
    switch (type)
    {
    case DIPCVNetPolicyType::ADPATER:
        break;
    case DIPCVNetPolicyType::CLONE:
        break;
    case DIPCVNetPolicyType::LOADBALANCE:
        break;
    case DIPCVNetPolicyType::AGG:
        break;
    case DIPCVNetPolicyType::NONE:
        break;
    case DIPCVNetPolicyType::AUTO:
    {
        m_policy = new CDIPCVnetAutoPolicy(this);
        if (m_policy)
        {
            m_policy->Init();
        }
        break;
    }
    default:
        break;
    }

    CAWInetAddr localaddr(CDipcVnetConfig::Instance()->GetDataChannelIp());
    m_pdatachannel = CAWAutoPtr< IDIPCVNetDataChannel>(new CVNetDataChannel(m_process));
    if (m_pdatachannel.Get() == NULL)
    {
        if (m_policy)
        {
            delete m_policy;
            m_policy = NULL;
        }
        return CAW_ERROR_FAILURE;
    }
    if (m_pdatachannel->Open(localaddr) != CAW_OK)
    {
        if (m_policy)
        {
            delete m_policy;
            m_policy = NULL;
        }
        return CAW_ERROR_FAILURE;
    }

    return CAW_OK;
}

CAWResult CDIPCVnetSession::LowlayerSendPacket(const char* pkt, size_t pktsize)
{
    if (m_policy == NULL)
    {
        return m_process->SendToPeer(pkt, pktsize, m_peeraddr);
    }
    else
    {
        return m_policy->PolicySend(pkt, pktsize);
    }
}
CAWResult CDIPCVnetSession::SendPacket(const char *pkt, size_t pktsize)
{
    return m_process->SendToPeer(pkt,pktsize,m_peeraddr);
}

CAWResult CDIPCVnetSession::OnRecvPacket(const char *pkt, size_t pktsize)
{
    if (m_policy == NULL)
    {
        return OnUplayerRecvPacket(pkt, pktsize);
    }
    else
    {
        return m_policy->PolicyRecv(pkt, pktsize);
    }
    return CAW_OK;
}

DWORD CDIPCVnetSession::AddReference()
{

    return CAWReferenceControlSingleThread::AddReference();
}

DWORD CDIPCVnetSession::ReleaseReference()
{

    return CAWReferenceControlSingleThread::ReleaseReference();
}
CAWResult CDIPCVnetSession::SendData(const char *pdata, size_t pktsize)
{
    if (m_pdatachannel.Get())
    {
        return CAW_ERROR_FAILURE;
    }

    return m_pdatachannel->SendData((char*)pdata, pktsize, m_peeraddr);
}