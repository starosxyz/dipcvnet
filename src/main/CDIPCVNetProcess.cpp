/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/

#include "CDIPCVNetProcess.h"
#include "CDIPCVNetConfig.h"
#include "dipcvnet/ppp/CDIPCVnetPPPManager.h"
#include "dipcvnet/mppp/CDIPCVnetMPPPManager.h"
#include "dipcvnet/vos/CDIPCVnetVOSManager.h"
#include "dipcvnet/vswitch/CDIPCVnetSwitchManager.h"
#include "utils/DIPCVNetDebug.h"
#include "utils/DIPCVNetType.h"


CDIPCVNetProcess::CDIPCVNetProcess()
    :m_process(NULL)
    , m_vnet(NULL)
{

}
CDIPCVNetProcess::~CDIPCVNetProcess()
{

}
CAWResult CDIPCVNetProcess::InitVNet()
{
    CAWString strmode= CDipcVnetConfig::Instance()->GetDipcMode(); /* from config*/

    DIPCVNetType type = StringToVNet(strmode);

    switch (type)
    {
    case DIPCVNetType::MPPP:
        m_vnet = new CDIPCVnetMPPPManager(this);
        break;
    case DIPCVNetType::PPP:
        m_vnet = new CDIPCVnetPPPManager(this);
        break;
    case DIPCVNetType::VSWITCH:
        m_vnet = new CDIPCVnetSwitchManager(this);
        break;
    case DIPCVNetType::VOS:
        m_vnet = new CDIPCVnetVOSManager(this);
        break;
    default:
        break;
    }

    if (m_vnet == NULL)
    {
        dipcvnet_printf("no mode for vnet\n");
        return CAW_ERROR_FAILURE;
    }

    if (m_vnet->Init() != CAW_OK)
    {
        dipcvnet_printf("vnet init failure\n");
        return CAW_ERROR_FAILURE;
    }

    m_white_list.Init();
    m_black_list.Init();

    return CAW_OK;
}
void CDIPCVNetProcess::OnProcessUpdateState(const CDIPCProcess& updateprocess)
{

}
void CDIPCVNetProcess::OnProcessShutdown()
{
}
void CDIPCVNetProcess::OnBootOK()
{
}

/* main */
void CDIPCVNetProcess::OnProcessRun(int argc, char** argv, IDIPCProcess* dipcProcess)
{
	m_process = dipcProcess;
	m_process->ProcessRunFinishNotify();
    
#if 0
    CAWString mode = CDipcVnetConfig::Instance()->GetDipcMode();
    DIPCVNetPeer_JNO = CDipcVnetConfig::Instance()->GetDipcPeerJno();

    CDIPCAddress local_adddr;
    m_process->GetLocalDIPCAddr(local_adddr);
    CDIPCApp local_app(local_adddr);
    m_process->GetApp(m_process->GetLocalNodeAddress(),local_app);
    CAWResult rt = m_sessionmanager.CreateSession(this,local_app);
    if(CAW_FAILED(rt))
    {
        CAW_FATAL_TRACE("--------CDIPCVNetProcess::OnProcessRun--number Failure");
    }else{
        CAW_INFO_TRACE("--------CDIPCVNetProcess::OnProcessRun--number Success");
    }
    return ;
    if( (mode=="lan")||(mode=="LAN") )
    {
        m_mode = CDIPCVNetProcess::TT_LAN;
        CAWResult rv = m_lanmanager.Init();
        if(CAW_FAILED(rv))
        {
            CAW_FATAL_TRACE("--------CDIPCVNetProcess::OnProcessRun--number Mode TT_LAN Failure");
            return;
        }
        CAW_INFO_TRACE("CDIPCVNetProcess::OnProcessRun Mode TT_LAN");
    }
    else
    {
        m_mode = CDIPCVNetProcess::TT_P2P;
        CAW_INFO_TRACE("CDIPCVNetProcess::OnProcessRun Mode TT_P2P");
    }
    CAW_INFO_TRACE("--------CDIPCVNetProcess::OnProcessRun--number Success");
#endif
}


void CDIPCVNetProcess::OnNodeConnected(const CDIPCNode& node)
{
    CAW_INFO_TRACE("CDIPCVNetProcess::OnNodeConnected");
}

void CDIPCVNetProcess::OnPeerConnected(const CDIPCApp& peeraddr)
{
    CDIPCAddress peer_address = peeraddr.GetDIPCAddress();
    CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerConnected, peer="<<peer_address.ToString());
    uint16_t peerjno = peer_address.GetJno();
    CAWResult rv = m_black_list.Veryfy(peerjno);
    if (rv ==VNET_BL_FIND_OK)
    {
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerConnected It is not CDIPCVNetProcess");
        return;
    }
    rv = m_white_list.Veryfy(peerjno);
    if (rv == VNET_WL_FIND_FAILURE)
    {
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerConnected It is not CDIPCVNetProcess");
        return;
    }
    if (m_vnet == NULL)
    {
        return;
    }

    rv = m_vnet->VNetAddSession(peer_address);
    if(CAW_FAILED(rv))
    {
        CAW_FATAL_TRACE("CDIPCVNetProcess::OnPeerConnected CreateSession Failed");
    }else{
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerConnected CreateSession Successful");
    }
}


void CDIPCVNetProcess::OnNodeDisconnect(const CDIPCNode& node)
{
    CDIPCAddress peeraddr = node.GetNodeAddress();
    CAW_INFO_TRACE("CDIPCVNetProcess::OnNodeDisconnect");
}

void CDIPCVNetProcess::OnPeerDisconnect(const CDIPCApp& peeraddr)
{
    CDIPCAddress peer_address = peeraddr.GetDIPCAddress();
	CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerDisconnect, peer="<<peer_address.ToString());

    if (m_vnet == NULL)
    {
        return;
    }

    CAWResult rv = CAW_ERROR_FAILURE;
    rv = m_vnet->VNetRemoveSession(peer_address);
    if(CAW_FAILED(rv))
    {
        CAW_FATAL_TRACE("CDIPCVNetProcess::OnPeerConnected RemoveSesion Failed");
    }else{
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerConnected RemoveSesion Successful");
    }
}
void CDIPCVNetProcess::OnDataRcvFromPeer(CAWMessageBlock& msg, const CDIPCAddress& fromaddr)
{
    size_t datasize = msg.GetChainedLength();
    const char* pdata = msg.GetTopLevelReadPtr();
    IDIPCVnetSession* pVnet_Session = m_vnet->VNetGetSession(fromaddr);
    if (pVnet_Session)
    {
        pVnet_Session->OnRecvPacket(pdata, datasize);
    }
}

void CDIPCVNetProcess::OnPacketRcv(CAWMessageBlock& msg, const CDIPCAddress& fromaddr)
{
    size_t datasize = msg.GetChainedLength();
    const char* pdata = msg.GetTopLevelReadPtr();
    IDIPCVnetSession* pVnet_Session = m_vnet->VNetGetSession(fromaddr);
    if (pVnet_Session)
    {
        pVnet_Session->OnRecvPacket(pdata, datasize);
    }
}


void CDIPCVNetProcess::OnPacketRcvFromPeerNode(CAWMessageBlock& msg, const CDIPCAddress& fromaddr)
{
    CAW_INFO_TRACE("CDIPCVNetProcess::OnPacketRcvFromPeerNode");
}


void CDIPCVNetProcess::OnPersistentdataChange(const CAWString& strurl)
{

}

CAWResult CDIPCVNetProcess::SendToPeer(const char *pdata, size_t datasize, const CDIPCAddress peeraddr)
{
    if (m_process==NULL)
    {
        CAW_ERROR_TRACE("CStar5GCore::SendToPeer,m_process==NULL");
        return CAW_ERROR_FAILURE;
    }
    
    CDIPCAddress localdipcaddr;
    m_process->GetLocalDIPCAddr(localdipcaddr);
    CAW_INFO_TRACE("CDIPCVNetProcess::SendToPeer,localaddr "<<localdipcaddr.ToString()
        <<" >>> to peeraddr "<<peeraddr.ToString()
        <<" ,datasize="<<datasize);

    CAWMessageBlock msgblock(
         datasize,
         (char *)pdata,
         CAWMessageBlock::DONT_DELETE,
         datasize
    );
    CAWResult rv = m_process->SendPacket(msgblock,peeraddr);
    if(CAW_FAILED(rv))
    {
        CAW_FATAL_TRACE("CDIPCVNetProcess::SendToPeer Failed");
        return rv;
    }

    return CAW_OK;
}

