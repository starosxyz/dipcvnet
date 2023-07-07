#include "CDIPCVnetAutoPolicy.h"
#include "main/CDIPCVNetProcess.h"
#include "main/CDIPCVNetConfig.h"
#include "VNETPdu.h"
#include "utils/dipcvnetutils.h"
#include "probe/CDIPCVnetDataPing.h"
#include "probe/CDIPCVNetDIPCPing.h"
#include "dipcvnet/CVNetDataChannel.h"
#include "main/CDIPCVNetProcess.h"
CDIPCVnetAutoPolicy::CDIPCVnetAutoPolicy(CDIPCVnetSession *psession)
        :m_psession(psession),
        m_dataprobe(NULL),
        m_dipcprob(NULL), m_timeroutms(3000)
{    
    CAW_INFO_TRACE_THIS("CDIPCVnetAutoPolicy::CDIPCVnetAutoPolicy");
}
    
CDIPCVnetAutoPolicy::~CDIPCVnetAutoPolicy()
{
    CAW_INFO_TRACE_THIS("CDIPCVnetAutoPolicy::~CDIPCVnetAutoPolicy");
    if (m_dataprobe)
        delete m_dataprobe;
    if (m_dipcprob)
        delete m_dipcprob;
}
CAWResult CDIPCVnetAutoPolicy::Init()
{
    CAWResult rv = CAW_ERROR_FAILURE;
    CAW_INFO_TRACE_THIS("CDIPCVnetAutoPolicy::Init");
    CDIPCVNetProcess* pprocess = m_psession->GetVNetProcess();

    CDIPCAddress peeraddr = m_psession->GetPeerAddress();
    m_dataprobe = new CDIPCVnetDataPing(peeraddr, m_psession->GetDataChannel());
    if (m_dataprobe == NULL)
    {
        CAW_ERROR_TRACE_THIS("CDIPCVnetAutoPolicy::Init out of memory");
        return CAW_ERROR_FAILURE;
    }
    m_dipcprob = new CDIPCVNetDIPCPing(pprocess, peeraddr);
    if (m_dipcprob == NULL)
    {
        CAW_ERROR_TRACE_THIS("CDIPCVnetAutoPolicy::Init out of memory");
        return CAW_ERROR_FAILURE;
    }

    uint32_t timeroutms = 3000;

    if (timeroutms != 0)
    {
        StartProbeTimer(timeroutms);
    }

    CAW_INFO_TRACE_THIS("CDIPCVnetAutoPolicy::Init end");
    return CAW_OK;
}
CAWResult CDIPCVnetAutoPolicy::PolicyRecv(const char* pkt, size_t pktsize)
{
    CAW_INFO_TRACE_THIS("CDIPCVnetAutoPolicy::PolicyRecv pktsize="<< pktsize);
    struct DIPCPDUHead* phead = (struct DIPCPDUHead*)pkt;
    uint16_t pdutype=0;
    uint16_t pdulenth=0;
    uint32_t xid=0;

    pdutype = ntohs(phead->pdutype);
    pdulenth = ntohs(phead->pdulenth);
    xid = ntohl(phead->xid);

    switch (pdutype)
    {
    case DIPC_VNET_DATA:
    {
        struct VNetPacketPDU* p = (struct VNetPacketPDU*)pkt;
        uint32_t datasize = ntohl(p->datasize);
        if (datasize + sizeof(struct VNetPacketPDU) != pdulenth)
        {
            CAW_ERROR_TRACE("CDIPCVnetAutoPolicy::OnRecvPacket rong datasize");
            return CAW_ERROR_FAILURE;
        }
        m_psession->OnUplayerRecvPacket(p->data, datasize);
        break;
    }
    case DIPC_VNET_KEEPALIVE_REQUEST:
    {
        if (m_dipcprob)
        {
            m_dipcprob->HandlePingRequest(pkt, pktsize);
        }
        break;
    }
    case DIPC_VNET_KEEPALIVE_RESPONSE:
    {
        if (m_dipcprob)
        {
            m_dipcprob->HandlePingResponse(pkt, pktsize);
        }
        break;
    }
    case DIPC_VNET_DATA_KEEPALIVE_REQUET:
    {
        if (m_dataprobe)
        {
            m_dataprobe->HandlePingRequest(pkt, pktsize);
        }
        break;
    }
    case DIPC_VNET_DATA_KEEPALIVE_RESPONSE:
    {
        if (m_dataprobe)
        {
            m_dataprobe->HandlePingResponse(pkt, pktsize);
        }
        break;
    }
    default:
        break;
    }
    return CAW_OK;
}
CAWResult CDIPCVnetAutoPolicy::PolicySend(const char* pkt, size_t pktsize)
{
    static char buffer[1024 * 2];
    struct VNetPacketPDU* p = (struct VNetPacketPDU*)buffer;
    ::memcpy(p->data, pkt, pktsize);
    p->datasize = htonl(pktsize);
    uint16_t pdulenth = pktsize + sizeof(struct VNetPacketPDU);
    p->head.pdulenth = htons(pdulenth);
    p->head.pdutype = htons(DIPC_VNET_DATA);

    CAW_INFO_TRACE_THIS("CDIPCVnetAutoPolicy::PolicySend pktsize=" << pdulenth);
    return m_psession->SendPacket(buffer, pdulenth);
}

CAWResult CDIPCVnetAutoPolicy::StartProbeTimer(uint32_t ms)
{
    CAW_INFO_TRACE_THIS("CDIPCVnetAutoPolicy::StartProbeTimer ms=" << ms);
    if (ms == 0)
    {
        m_timer.Cancel();
        return CAW_OK;
    }
    m_timer.Schedule(this, CAWTimeValue(ms / 1000, 0), 0);
    return CAW_OK;
}
CAWResult CDIPCVnetAutoPolicy::StopProbeTimer()
{
    CAW_INFO_TRACE_THIS("CDIPCVnetAutoPolicy::StopProbeTimer");
    m_timer.Cancel();
    return CAW_OK;
}
void CDIPCVnetAutoPolicy::OnTimer(CAWTimerWrapperID* aId)
{
    if (m_dataprobe)
    {
        m_dataprobe->SendPingRequest();
    }
    if (m_dipcprob)
    {
        m_dipcprob->SendPingRequest();
    }
}