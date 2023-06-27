#include "CDIPCVnetManager.h"

#include "CDIPCVNetProcess.h"
#include "CDIPCVNetConfig.h"


static void ConvertKey(const CDIPCAddress& addr,DIPCAppKey* pkey)
{
    pkey->m_did= addr.GetDataCenterId();
    pkey->m_cid=addr.GetClusterId();
    pkey->m_nodeid=addr.GetNodeId();
    pkey->m_jno=addr.GetJno();
    CAW_INFO_TRACE("CDIPCVnetManager ConvertKey, GetDataCenterId= "<<addr.GetDataCenterId()
                <<" , GetClusterId= "<<addr.GetClusterId()
                <<" , GetNodeId= "<<addr.GetNodeId()
                <<" , GetJno= "<<addr.GetJno());
}

CDIPCVnetManager::CDIPCVnetManager(CDIPCVNetProcess* process)
	:m_process(process)
{
}

CDIPCVnetManager::~CDIPCVnetManager()
{
}
//bug
CAWResult CDIPCVnetManager::CreateVnetTap(const CDIPCAddress &peer_address)
{
    CDIPCVnetTap *pnew=new CDIPCVnetTap(peer_address,m_process);
    if (pnew==NULL)
    {
        CAW_ERROR_TRACE("CDIPCVnetManager::CreateVnetTap new CDIPCVnetTap failure");
        return CAW_ERROR_FAILURE;
    }
    IAWReactor *pnetworkthread = CAWThreadManager::Instance()->GetThreadReactor(CAWThreadManager::TT_NETWORK);

    if  (pnetworkthread==NULL)
    {
        CAW_ERROR_TRACE("CAWThreadManager::Instance()->GetThreadReactor(CAWThreadManager::TT_NETWORK) failed");
        return CAW_ERROR_FAILURE;
    }

    char macoctect[6] = {0};
    CAWMacAddress addrmac;
    addrmac.ParseAddress(CDipcVnetConfig::Instance()->GetExtosMac().c_str());
    addrmac.ToOctet((char*)macoctect);

    CAWString strdevname=CDipcVnetConfig::Instance()->GetExtosOstap();
    uint32_t vnetip=CDipcVnetConfig::Instance()->GetExtosIpUint();

    for(int i=0;i<6;i++)
    {
        CAW_INFO_TRACE("macoctect["<<i<<"] "<<macoctect[i]);
    }
    CAW_INFO_TRACE("strdevname="<<strdevname<<
        ", vnetip="<<vnetip);

    CAWResult rv = pnew->Init(strdevname,
                                vnetip,
                                macoctect,
                                pnetworkthread);
    if (CAW_FAILED(rv))
    {
        CAW_ERROR_TRACE("CDIPCVNetProcess::OnNodeConnected CDIPCVnetTap Init failed");
        delete pnew;
        return CAW_ERROR_FAILURE;
    }
    pnew->Up();
    AddVnet(pnew,peer_address);
    return CAW_OK;
}


CAWResult CDIPCVnetManager::AddVnet(CDIPCVnetTap *pvnet,
	const CDIPCAddress &addr)
{
    //CDIPCAddress addr = app.GetDIPCAddress();
    DIPCAppKey key;
    ConvertKey(addr,&key);
    
    VNetMAPType::iterator it= m_vnetmap.find(key);
    if (it != m_vnetmap.end())
    {
        CAW_ERROR_TRACE("CDIPCVnetManager::AddVnet failed ,m_vnetmap.find(key) != NULL");
        return CAW_ERROR_FAILURE;
    }
    m_vnetmap[key] = pvnet;               
    return CAW_OK;
}
CAWResult CDIPCVnetManager::RemoveVnet(const CDIPCAddress &addr)
{
    //CDIPCAddress addr = app.GetDIPCAddress();
    DIPCAppKey key;
    ConvertKey(addr,&key);
    
    VNetMAPType::iterator it= m_vnetmap.find(key);
    if (it == m_vnetmap.end())
    {
        CAW_ERROR_TRACE("CDIPCVnetManager::RemoveVnet failed ,m_vnetmap.find(key) == NULL");
        return CAW_ERROR_FAILURE;
    }
    CAW_INFO_TRACE("CDIPCVnetManager::RemoveVnet, CDIPCVnetTap= "
                <<(void*)m_vnetmap[key]);
    m_vnetmap.erase(it);
    return CAW_OK;
}

CDIPCVnetTap *CDIPCVnetManager::GetVnet(const CDIPCAddress &addr)
{
    //CDIPCAddress addr = app.GetDIPCAddress();
    DIPCAppKey key;
    ConvertKey(addr,&key);

    VNetMAPType::iterator it= m_vnetmap.find(key);
    if (it == m_vnetmap.end())
    {
        CAW_ERROR_TRACE("CDIPCVnetManager::GetVnet failed ,m_vnetmap.find(key) == NULL");
        return NULL;
    }
    CAW_INFO_TRACE("CDIPCVnetManager::GetVnet, CDIPCVnetTap= "
                <<(void*)m_vnetmap[key]);

    return m_vnetmap[key];
}

CAWResult CDIPCVnetManager::OnPacketRcv(const char* rcvdata, size_t datasize,
    const CDIPCAddress &peer_address)
{
    CDIPCVnetTap *pnew = GetVnet(peer_address);
    if(!pnew)
    {
        CAW_ERROR_TRACE("CDIPCVNetProcess::OnPacketRcv,GetVnet failed");
        return CAW_ERROR_FAILURE;
    }
    CAW_INFO_TRACE("CDIPCVnetManager::OnPacketRcv, Input Tap  datasize= "<<datasize);
    pnew->Input(rcvdata, datasize);
    return CAW_OK;
}

