#include "CDIPCVnetSwitchManager.h"

#include "CDIPCVnetSwitchPort.h"
#include "CDIPCVnetSwitchPortLocal.h"

static void ConvertKey(const CDIPCAddress& addr,DIPCAppKey* pkey)
{
    pkey->m_did= addr.GetDataCenterId();
    pkey->m_cid=addr.GetClusterId();
    pkey->m_nodeid=addr.GetNodeId();
    pkey->m_jno=addr.GetJno();
}


CDIPCVnetSwhichManager::CDIPCVnetSwhichManager(CDIPCVNetProcess *process)
	:m_process(process),m_switch_manager(NULL)
{
}

CDIPCVnetSwhichManager::~CDIPCVnetSwhichManager()
{
	DestroyMacLearningSwitch(m_switch_manager);
}

CAWResult CDIPCVnetSwhichManager::Init()
{
    if(!m_switch_manager)
        m_switch_manager=CreateMacLearningSwitch();
    if(!m_switch_manager)
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::Init() CreateMacLearningSwitch() ==NULL");
        return CAW_ERROR_FAILURE;
    }
    CDIPCVnetSwhichSinklocal*pSwhichSink=new CDIPCVnetSwhichSinklocal(m_switch_manager);
    if(!pSwhichSink)
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::Init() new CDIPCVnetSwhichSinklocal() failed");
        return CAW_ERROR_FAILURE;
    }
    CAWResult rv = pSwhichSink->Tap_init();
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::Init() pSwhichSink->Tap_init() failed");
        delete pSwhichSink;
        return CAW_ERROR_FAILURE;
    }
    rv = m_switch_manager->AddSink(pSwhichSink);
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::Init() m_switch_manager->AddSinkPort(pSwhichSink) failed");
        delete pSwhichSink;
        return CAW_ERROR_FAILURE;
    }
    rv = pSwhichSink->Up();
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::Init() pSwhichSink->Up() failed");
    }

    return CAW_OK;
}

CAWResult CDIPCVnetSwhichManager::CreateSwitchPort(const CDIPCAddress &peer_address)
{

    CDIPCVnetSwhichSink* pnew =new CDIPCVnetSwhichSink(peer_address,m_process,m_switch_manager);
    if(!pnew)
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::CreateSwitchPort() new CDIPCVnetSwhichSink failed");
        return CAW_ERROR_FAILURE;
    }
    return AddSwitchPort(pnew,peer_address);
}

CAWResult CDIPCVnetSwhichManager::AddSwitchPort(IVSwitchSink *pvnet,const CDIPCAddress &peer_address)
{
    DIPCAppKey key;
    ConvertKey(peer_address,&key);
	m_vlanmap[key]=pvnet;
    CAWResult rv = m_switch_manager->AddSink(pvnet);
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::AddSwitchPort() m_pswitch->AddSinkPort(pvnet) failed");
        return CAW_ERROR_FAILURE;
    }
	return CAW_OK;
}

CAWResult CDIPCVnetSwhichManager::RemoveSwitchPort(const CDIPCAddress &peer_address)
{
    DIPCAppKey key;
    ConvertKey(peer_address,&key);
    
    CAWResult rv = m_switch_manager->RemoveSink(m_vlanmap[key]);
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::RemoveSwitchPort() m_pswitch->RemoveSink(pvnet) failed");
        return CAW_ERROR_FAILURE;
    }
    m_vlanmap.erase(key);
	return CAW_OK;
}

IVSwitchSink *CDIPCVnetSwhichManager::GetSwitchPort(const CDIPCAddress &peer_address)
{
    DIPCAppKey key;
    ConvertKey(peer_address,&key);
	return m_vlanmap[key];
}


CAWResult CDIPCVnetSwhichManager::OnPacketRcv(const char* rcvdata, size_t datasize,
        const CDIPCAddress &peer_address)
{
    IVSwitchSink *pswitchsink=GetSwitchPort(peer_address);
    if(!pswitchsink)
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::OnPacketRcv() GetSwitchPort failed");
        return CAW_ERROR_FAILURE;
    }
    CDIPCVnetSwhichSink *pswitchsinkport = dynamic_cast<CDIPCVnetSwhichSink*>(pswitchsink);
    CAWResult rv = pswitchsinkport->InputData(rcvdata,datasize);
    if(CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVnetSwhichManager::OnPacketRcv() pswitchsink->InputData failed");
        return CAW_ERROR_FAILURE;
    }
	return CAW_OK;
}

