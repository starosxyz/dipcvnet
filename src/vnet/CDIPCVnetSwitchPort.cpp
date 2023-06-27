#include "CDIPCVnetSwitchPort.h"


CDIPCVnetSwhichSink::CDIPCVnetSwhichSink(const CDIPCAddress peer_address, 
        CDIPCVNetProcess *process, IVSwitch *switch_manager)
        :m_peer_address(peer_address),m_process(process),m_switch_manager(switch_manager)
{	
}

CDIPCVnetSwhichSink::~CDIPCVnetSwhichSink()
{	
}

void CDIPCVnetSwhichSink::OnOutput(IVSwitchSink *from,
        const char *rcvdata,
        size_t datasize,
        bool flood)
{
	if (!m_process)
	{
		CAW_ERROR_TRACE("CDIPCVnetSwhichSink::OnOutput m_process==NULL");
        return;
	}
    CAW_INFO_TRACE("CDIPCVnetSwhichSink::OnOutput IVSwitch Output SendToPeer datasize= "<<datasize);
    m_process->SendToPeer(rcvdata,datasize,m_peer_address);

}

void CDIPCVnetSwhichSink::OnStateChange(int state)
{
	
}

CAWResult CDIPCVnetSwhichSink::InputData(const char *pdata,size_t datasize)
{
    CAW_INFO_TRACE("CDIPCVnetSwhichSink::InputData OnPacketRcv Input IVSwitch datasize="<<datasize);
    return m_switch_manager->Input(this,pdata,datasize);
}

