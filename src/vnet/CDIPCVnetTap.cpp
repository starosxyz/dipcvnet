
#include "CDIPCVNetProcess.h"
#include "CDIPCVnetTap.h"

extern CDIPCVNetProcess *process_a;

CDIPCVnetTap::CDIPCVnetTap(const CDIPCAddress peer_address, CDIPCVNetProcess *process)
    :m_peer_address(peer_address),m_process(process)
{

}

CDIPCVnetTap::~CDIPCVnetTap()
{
}

void CDIPCVnetTap::OnOutput(const char* pkt, size_t pktsize)
{
    if (m_process)
    {
        CAW_INFO_TRACE("CDIPCVnetTap::OnOutput, Tap Output datasize= "<<pktsize);
        m_process->SendToPeer(pkt,pktsize,m_peer_address);
    }
}