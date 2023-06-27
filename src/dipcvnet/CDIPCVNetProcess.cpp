/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/

#include "CDIPCVNetProcess.h"
#include "CDIPCVNetConfig.h"


CDIPCVNetProcess::CDIPCVNetProcess()
    :m_process(NULL)
	,m_vnetmgr(this)
	,m_lanmanager(this)
	,m_mode(TT_UNKNOWN)
{

}
CDIPCVNetProcess::~CDIPCVNetProcess()
{

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
    CAWString mode = CDipcVnetConfig::Instance()->GetDipcMode();
    if( (mode=="lan")||(mode=="LAN") )
    {
        m_mode = CDIPCVNetProcess::TT_LAN;
        CAWResult rv = m_lanmanager.Init();
        if(CAW_FAILED(rv))
        {
            CAW_ERROR_TRACE("--------CDIPCVNetProcess::OnProcessRun--number Mode TT_LAN Failure");
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
}


void CDIPCVNetProcess::OnNodeConnected(const CDIPCNode& node)
{
    CAW_INFO_TRACE("CDIPCVNetProcess::OnNodeConnected");
}


void CDIPCVNetProcess::OnPeerConnected(const CDIPCApp& peeraddr)
{
    CDIPCAddress peer_address = peeraddr.GetDIPCAddress();
    CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerConnected, peer="<<peer_address.ToString());

    if(peer_address.GetJno()!=DIPCVNet_JNO)
    {
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerConnected It is not CDIPCVNetProcess");
        return;
    }
    CAWResult rv = CAW_ERROR_FAILURE;
    if(m_mode == CDIPCVNetProcess::TT_LAN)
        rv = m_lanmanager.CreateSwitchPort(peer_address);
    else if(m_mode == CDIPCVNetProcess::TT_P2P)
        rv = m_vnetmgr.CreateVnetTap(peer_address);
    if(CAW_FAILED(rv))
    {
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerConnected CreateVnet Failed");
    }else{
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerConnected CreateVnet Successful");
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

    if(peer_address.GetJno()!=DIPCVNet_JNO)
    {
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerDisconnect It is not CDIPCVNetProcess");
        return;
    }

    CAWResult rv = CAW_ERROR_FAILURE;
    if(m_mode == CDIPCVNetProcess::TT_LAN)
    {
        IVSwitchSink *pnew=m_lanmanager.GetSwitchPort(peer_address);
        rv = m_lanmanager.RemoveSwitchPort(peer_address);
        delete pnew;
    }
    else if(m_mode == CDIPCVNetProcess::TT_P2P)
    {
        CDIPCVnetTap *pnew=m_vnetmgr.GetVnet(peer_address);
        rv = m_vnetmgr.RemoveVnet(peer_address);
        delete pnew;
    }
    if(CAW_FAILED(rv))
    {
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerDisconnect RemoveVnet Failed");
    }else{
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPeerDisconnect RemoveVnet Successful");
    }
}


void CDIPCVNetProcess::OnPacketRcv(CAWMessageBlock& msg, const CDIPCAddress& fromaddr)
{
    size_t datasize = msg.GetChainedLength();
    const char *pdata=msg.GetTopLevelReadPtr();
    CAW_INFO_TRACE("CDIPCVNetProcess::OnPacketRcv,datasize="<<datasize<<"<<< from "<<fromaddr.ToString());

    if(fromaddr.GetJno()!=DIPCVNet_JNO)
    {
        CAW_INFO_TRACE("CDIPCVNetProcess::OnPacketRcv It is not CDIPCVNetProcess");
        return;
    }

    if(m_mode == CDIPCVNetProcess::TT_LAN)
        m_lanmanager.OnPacketRcv(pdata, datasize,fromaddr);
    else if(m_mode == CDIPCVNetProcess::TT_P2P)
        m_vnetmgr.OnPacketRcv(pdata, datasize,fromaddr);
}


void CDIPCVNetProcess::OnPacketRcvFromPeerNode(CAWMessageBlock& msg, const CDIPCAddress& fromaddr)
{
    CAW_INFO_TRACE("CDIPCVNetProcess::OnPacketRcvFromPeerNode");
}


void CDIPCVNetProcess::OnPersistentdataChange(const CAWString& strurl)
{

}

void CDIPCVNetProcess::SendToPeer(const char *pdata, size_t datasize, const CDIPCAddress peeraddr)
{
    CAW_INFO_TRACE("CDIPCVNetProcess::SendToPeer,datasize="<<datasize);
    if (m_process==NULL)
    {
        CAW_ERROR_TRACE("CStar5GCore::SendToPeer,m_process==NULL");
        return;
    }
    if(peeraddr.GetJno()!=DIPCVNet_JNO)
    {
        CAW_INFO_TRACE("CDIPCVNetProcess::SendToPeer It is not CDIPCVNetProcess");
        return;
    }
    CAWMessageBlock msgblock(
         datasize,
         (char *)pdata,
         CAWMessageBlock::DONT_DELETE,
         datasize
    );
    CDIPCAddress localdipcaddr;
    m_process->GetLocalDIPCAddr(localdipcaddr);
    CAW_INFO_TRACE("CDIPCVNetProcess::SendToPeer,localaddr "
        <<localdipcaddr.ToString()<<">>> to peeraddr "<<peeraddr.ToString());
    m_process->SendPacket(msgblock,peeraddr);
}

