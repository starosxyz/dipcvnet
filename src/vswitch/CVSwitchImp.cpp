/***********************************************************************
 * Copyright (C) 2016-2021, Nanjing StarOS, Inc. 
**********************************************************************/
#include "CVSwitchImp.h"
#include <starbase/CAWUtils.h>
#include <starbase/CAWMacAddress.h>
#include "CVSwitchPort.h"
#include "utils/MACAddress.h"

#include "packet/STPPacket.h"
#define SWITCH_PORT_MAX 1000
#define SWITCH_TABLE_TIMEOUT 1000*20
#define SWITCH_TICK     1000*3
SwitchTableItem::SwitchTableItem()
    :m_psink(NULL)
    , m_vswitch(NULL)
    , m_outputid(0)
{
    ::memset(m_macaddr, 0, sizeof(m_macaddr));
    m_currenttv = CAWTimeValue::GetTimeOfDay();
}

SwitchTableItem::SwitchTableItem(CVSwitchImp *pvswitch)
    :m_psink(NULL)
	,m_vswitch(pvswitch)
    , m_outputid(0)
{
    ::memset(m_macaddr,0,sizeof(m_macaddr));
    m_currenttv=CAWTimeValue::GetTimeOfDay();
}

SwitchTableItem::SwitchTableItem(CVSwitchImp* pvswitch,
    char macaddr[ETHERNET_MAC_LENGTH],
    uint32_t portid,
    IDIPCVSwitchSink* psink)
        :m_psink(psink)
        ,m_outputid(portid)
		,m_vswitch(pvswitch)
{
    ::memcpy(m_macaddr,macaddr,ETHERNET_MAC_LENGTH);
     m_currenttv=CAWTimeValue::GetTimeOfDay();
}
SwitchTableItem::SwitchTableItem(const SwitchTableItem &other)
{
    *this = other;
}
SwitchTableItem& SwitchTableItem::operator=(const SwitchTableItem& other)
{
    if (this != &other) {
        ::memcpy(m_macaddr,other.m_macaddr,ETHERNET_MAC_LENGTH);
        m_currenttv=other.m_currenttv;
        m_psink=other.m_psink;
        m_vswitch = other.m_vswitch;
        m_outputid = other.m_outputid;
    }
    return *this;
}
bool SwitchTableItem::operator==(const SwitchTableItem& other) const
{
    return ((memcmp(m_macaddr, other.m_macaddr, ETHERNET_MAC_LENGTH) == 0)
        && (m_psink==other.m_psink)
        && (m_outputid==other.m_outputid));
}
bool SwitchTableItem::IsTimeOut()
{
    long timeout = (long)m_vswitch->GetTimeOut();
    CAWTimeValue tvCur = CAWTimeValue::GetTimeOfDay();
    long currenttimer=tvCur.GetTotalInMsec();
    long prextimer=m_currenttv.GetTotalInMsec();
    if (currenttimer-prextimer>=timeout)
    {
        return true;
    }
    else 
    {
        return false;
    }
}
void SwitchTableItem::UpdateTime()
{
    m_currenttv=CAWTimeValue::GetTimeOfDay();
}
void SwitchTableItem::GetMac(char mac[ETHERNET_MAC_LENGTH])
{
    memcpy(mac,m_macaddr,sizeof(m_macaddr));
}


CVSwitchImp::CVSwitchImp()
	:m_timeoutms(SWITCH_TABLE_TIMEOUT)
    ,m_stp(this,"vswitch", GetCurrentTimerInMSec(),NULL)
    , m_TimeoutInterval(SWITCH_TICK)
{
}
CVSwitchImp::~CVSwitchImp()
{
    std::unordered_map<uint32_t, CVSwitchPort*>::iterator it = m_sinklist.begin();
    while (it != m_sinklist.end())
    {
        CVSwitchPort* pport = it->second;
        uint32_t id = pport->GetId();
        IDIPCVSwitchSink* ptemp = pport->GetSink();
        it = m_sinklist.erase(it);
        m_idbase.FreeID(id);
        delete pport;
    }
    m_macTable.clear();
}
#if 0
IVSwitch &IVSwitch::Instance()
{
    return *CAWSingletonT<CVSwitchImp>::Instance();
}
#endif
CAWResult CVSwitchImp::Init()
{
    CAWTimeValue aInterval((long long )m_TimeoutInterval /1000,0);
    CAW_INFO_TRACE("CVSwitchProcess::Init");

    m_timeout.Schedule(this,aInterval);
    m_idbase.Init(1, SWITCH_PORT_MAX);
    return CAW_OK;
}

CAWResult CVSwitchImp::Close()
{
    return CAW_OK;
}
void CVSwitchImp::SetTickInterval(long ms)
{
    if (ms < SWITCH_TICK)
    {
        m_TimeoutInterval = SWITCH_TICK;
    }
    else
    {
        m_TimeoutInterval = ms;
    }
    m_timeout.Cancel();
    CAWTimeValue aInterval((long long )m_TimeoutInterval / 1000, 0);
    m_timeout.Schedule(this, aInterval);
}
void CVSwitchImp::SetBridgeId(uint64_t bridge_id)
{
    m_stp.SetBridgeId(bridge_id);
}
void CVSwitchImp::SetBridgePriority(uint16_t new_priority)
{
    m_stp.SetBridgePriority(new_priority);
}
void CVSwitchImp::SetHelloTime(long ms)
{
    m_stp.SetHelloTime(ms);
}
void CVSwitchImp::SetMaxAge(long ms) 
{
    m_stp.SetMaxAge(ms);
}
void CVSwitchImp::SetForwardDelay(long ms)
{
    m_stp.SetForwardDelay(ms);
}
uint64_t CVSwitchImp::GetBridgeId() 
{
    return m_stp.GetBridgeId();
}
uint64_t CVSwitchImp::GetDesignatedRoot() 
{
    return m_stp.GetDesignatedRoot();
}
bool CVSwitchImp::IsRootBridge() 
{
    return m_stp.IsRootBridge();
}

long CVSwitchImp::GetHelloTime() 
{
    return m_stp.GetHelloTime();
}
long CVSwitchImp::GetMaxAge()
{
    return m_stp.GetMaxAge();
}
long CVSwitchImp::GetForwardDelay()
{
    return m_stp.GetForwardDelay();
}

CAWResult CVSwitchImp::SetMacItemTimeOutMs(long timeoutms)
{
	m_timeoutms=timeoutms;
	return CAW_OK;
}
CAWResult CVSwitchImp::AddSink(IDIPCVSwitchSink *psink,
    uint32_t path_cost)
{
    bool ishave=false;
    CVSwitchPort* pport = IsHaveSink(psink);
    if (pport)
    {
        CAW_ASSERTE(0);
        return CAW_OK;
    }
    uint32_t id=m_idbase.AllocID();
    if (id == 0)
    {
        return CAW_ERROR_FAILURE;
    }
    pport = new CVSwitchPort(this, psink, id);
    if (pport == NULL)
    {
        return CAW_ERROR_FAILURE;
    }

    if (m_stp.CreatePort(id, path_cost, STP_DISABLED) != CAW_OK)
    {
        delete pport;
        CAW_ASSERTE(0);
        return CAW_ERROR_FAILURE;
    }
    CVSwitchSTPPort* pstpport = m_stp.FindPort(id);

    if (pstpport == NULL)
    {
        delete pport;
        CAW_ASSERTE(0);
        return CAW_ERROR_FAILURE;
    }
    m_stp.PortEnable(pstpport);

    pport->SetSTPPort(pstpport);
    m_stp.PortSetAux(pstpport,pport);
    m_sinklist[id] = pport;
    return CAW_OK;
}

CAWResult CVSwitchImp::RemoveSink(IDIPCVSwitchSink *psink)
{
    std::unordered_map<uint32_t, CVSwitchPort*>::iterator it= m_sinklist.begin();
    while(it != m_sinklist.end())
    {
        CVSwitchPort* pport = it->second;
        uint32_t id = pport->GetId();
        IDIPCVSwitchSink* ptemp = pport->GetSink();
        if (ptemp==psink)
        {
            it=m_sinklist.erase(it);
            m_idbase.FreeID(id);
            delete pport;
            m_stp.DestroyPort(id);
        }
        else 
        {
            it++;
        }
    }

    std::unordered_map<MacKey, SwitchTableItem , MacKeyHashFun>::iterator it2= m_macTable.begin();
    while(it2 != m_macTable.end())
    {
        SwitchTableItem &item=it2->second;
        if (item.GetSink()==psink)
        {
            it2=m_macTable.erase(it2);
        }
        else 
        {
            it2++;
        }
    }

    return CAW_OK;
}

CVSwitchPort *CVSwitchImp::IsHaveSink(IDIPCVSwitchSink *psink)
{
    std::unordered_map<uint32_t, CVSwitchPort*>::iterator it = m_sinklist.begin();
    while (it != m_sinklist.end())
    {
        CVSwitchPort* pport = it->second;
        uint32_t id = pport->GetId();
        IDIPCVSwitchSink* ptemp = pport->GetSink();
        if (ptemp == psink)
        {
            return pport;
        }
        it++;
    }
    
    return NULL; 
}
void CVSwitchImp::OutputAllSink(IDIPCVSwitchSink *insink,
    const char *rcvdata,
    size_t datasize)
{
    std::unordered_map<uint32_t, CVSwitchPort*>::iterator it = m_sinklist.begin();
    while (it != m_sinklist.end())
    {
        CVSwitchPort* pport = it->second;
        uint32_t id = pport->GetId();
        IDIPCVSwitchSink* ptemp = pport->GetSink();
        CVSwitchSTPPort* pstpport = m_stp.FindPort(pport->GetId());
        if (pstpport)
        {
            if (stp_forward_in_state(m_stp.PortGetState(pstpport)))
            {
                if (ptemp != insink)
                {
                    ptemp->OnOutput(insink, rcvdata, datasize, true);
                }
            }
        }
        it++;
    } 
}

CAWResult CVSwitchImp::AddMacTable(const char mac[ETHERNET_MAC_LENGTH],SwitchTableItem &initem)
{

    MacKey key;
    ::memcpy(key.dmac,(uint8_t*)mac,ETHERNET_MAC_LENGTH);
    key.vlanid=0;
    std::unordered_map<MacKey, SwitchTableItem , MacKeyHashFun>::iterator it = m_macTable.find(key);
    if (it != m_macTable.end())
    {
        return CAW_ERROR_FAILURE;
    }
    m_macTable[key]=initem;
    
    return CAW_OK;
}

CAWResult CVSwitchImp::UpdateMacTable(const char mac[ETHERNET_MAC_LENGTH],
    SwitchTableItem &initem)
{

    MacKey key;
    ::memcpy(key.dmac,(uint8_t*)mac,ETHERNET_MAC_LENGTH);
    key.vlanid=0;
    std::unordered_map<MacKey, SwitchTableItem , MacKeyHashFun>::iterator it = m_macTable.find(key);
    if (it == m_macTable.end())
    {
        return CAW_ERROR_FAILURE;
    }
    m_macTable.erase(it);
    initem.UpdateTime();
    m_macTable[key]=initem;

    return CAW_ERROR_NOT_FOUND;
  
}


CAWResult CVSwitchImp::FindMacTable(const char mac[ETHERNET_MAC_LENGTH],SwitchTableItem &outitem)
{
    MacKey key;
    ::memcpy(key.dmac,(uint8_t*)mac,6);
    key.vlanid=0;
    std::unordered_map<MacKey, SwitchTableItem , MacKeyHashFun>::iterator it = m_macTable.find(key);
    if (it == m_macTable.end())
    {
        return CAW_ERROR_NOT_FOUND;
    }
    outitem=it->second;

    return CAW_OK;
}

CAWResult CVSwitchImp::RemoveMacTable(const char mac[ETHERNET_MAC_LENGTH])
{
    MacKey key;
    ::memcpy(key.dmac,(uint8_t*)mac,6);
    key.vlanid=0;
    std::unordered_map<MacKey, SwitchTableItem , MacKeyHashFun>::iterator it = m_macTable.find(key);
    if (it == m_macTable.end())
    {
        return CAW_ERROR_NOT_FOUND;
    }
    m_macTable.erase(it);
    return CAW_OK;
}

CAWResult CVSwitchImp::Input(IDIPCVSwitchSink *psink,
        const char *rcvdata,
        size_t datasize)
{
    
    CVSwitchPort* pport = NULL;
    if ((datasize ==0)|| (rcvdata == NULL))
    {
        CAW_ERROR_TRACE("CVSwitchImp::Input can not find the sink, please add sink first");
        return CAW_ERROR_FAILURE;
    }
    pport = IsHaveSink(psink);
    if (pport == NULL)
    {
        return CAW_ERROR_FAILURE;
    }
    CVSwitchSTPPort* pstpport = pport->GetSTPPort();
    if (pstpport==NULL)
    {
        return CAW_ERROR_FAILURE;
    }
    CAW_INFO_TRACE("CVSwitchImp::Input port state=" << (int)m_stp.PortGetState(pstpport)
        <<",id="<< pport->GetId());
    CAWAutoPtr<CEthernet> pethernet;

    if (CEthernet::DecodeEthernet((char*)rcvdata,
        datasize,
        pethernet) != CAW_OK)
    {
        CAW_ASSERTE(0);
        return CAW_ERROR_FAILURE;
    }

    WORD16 space = pethernet->GetEncodeSpace();

    CAWMessageBlock msgblk1(space);

    CAWByteStreamNetwork os(msgblk1);

    if (pethernet->StreamTo(os) != CAW_OK)
    {
        CAW_ASSERTE(0);
        return CAW_ERROR_FAILURE;
    }
    CAW_INFO_TRACE("CVSwitchImp::Input GetEthType =" << pethernet->GetEthType());
    if (pethernet->GetEthType() == 0x9946)
    {
        STPPacket* pstp = dynamic_cast<STPPacket*>(pethernet.Get());
        if (pstp)
        {
            char* pstpdata = NULL;
            size_t pstpadasize = 0;
            pstp->GetPlayload(pstpdata, pstpadasize);
            m_stp.ReceivedBpdu(pstpport, pstpdata, pstpadasize);
        }
        return CAW_OK;
    }
    if (stp_learn_in_state(m_stp.PortGetState(pstpport)))
    {
        InsertMac(psink, rcvdata, datasize);
    }
    IDIPCVSwitchSink* poutsink =NULL;
    char dstmac[ETHERNET_MAC_LENGTH] = { 0 };
    char srcmac[ETHERNET_MAC_LENGTH] = { 0 };
    memcpy(dstmac, rcvdata, ETHERNET_MAC_LENGTH);
    memcpy(srcmac, rcvdata + ETHERNET_MAC_LENGTH, ETHERNET_MAC_LENGTH);
    if (!stp_forward_in_state(m_stp.PortGetState(pstpport)))
    {
        CAW_INFO_TRACE("CVSwitchImp::Input drop");
        return CAW_OK;
    }
    if(MACAddress::isMulticast(dstmac))
    {
        OutputAllSink(psink,rcvdata, datasize);
        return CAW_OK;
    }
    if (MACAddress::isBroadcast(dstmac))
    {
        OutputAllSink(psink, rcvdata, datasize);
        return CAW_OK;
    }
    bool result = LookUp(rcvdata, datasize, poutsink);
    if (result==false)
    {
        //std::cout << "OutputAllSink" << std::endl;
        OutputAllSink(psink, rcvdata, datasize);
        return CAW_OK;
    }
    else
    {
        if (poutsink == NULL)
        {
            return CAW_OK;
        }
        if (poutsink == psink)
        {
            return CAW_OK;
        }
        CVSwitchPort* poutport = NULL;
        poutport = IsHaveSink(poutsink);
        if (poutport == NULL)
        {
            return CAW_ERROR_FAILURE;
        }
        CVSwitchSTPPort* pstpport = poutport->GetSTPPort();
        if (pstpport==NULL)
        {
            return CAW_ERROR_FAILURE;
        }
        if (!stp_forward_in_state(m_stp.PortGetState(pstpport)))
        {
            return CAW_OK;
        }
        poutsink->OnOutput(psink, rcvdata, datasize, false);
        return CAW_OK;
    }
    
    return CAW_OK;
}
CVSwitchPort* CVSwitchImp::FindPort(uint32_t id)
{
    std::unordered_map<uint32_t, CVSwitchPort*>::iterator it = m_sinklist.find(id);
    if (it == m_sinklist.end())
    {
        return NULL;
    }
    return it->second;
}
CAWResult CVSwitchImp::ClearTable()
{
    m_macTable.clear();
    return CAW_OK;
}
void CVSwitchImp::OnTimer(CAWTimerWrapperID* aId)
{
    std::unordered_map<MacKey, SwitchTableItem , MacKeyHashFun>::iterator it= m_macTable.begin();
    while(it != m_macTable.end())
    {
        SwitchTableItem &item=it->second;
        if (item.IsTimeOut()==true)
        {
            it=m_macTable.erase(it);
        }
        else 
        {
            it++;
        }
    }
    
    m_stp.Tick(SWITCH_TICK);

    //CheckAndUpdateLinkState();

    std::list<CVSwitchSTPPort*> portplist;
    m_stp.GetChangedPort(portplist);
    for (CVSwitchSTPPort* pport : portplist)
    {
        CVSwitchPort* ofport = (CVSwitchPort *)m_stp.PortGetAux(pport);
        enum stp_state tempstate = m_stp.PortGetState(pport);
        if (ofport->GetSTPState() != tempstate)
        {
            if (stp_learn_in_state(ofport->GetSTPState())
                != stp_learn_in_state(tempstate)) {
                /* xxx Learning action flows should also be flushed. */
                m_macTable.clear();
            }

            ofport->SetSTPState(tempstate);
            if (ofport->GetSink())
            {
                ofport->GetSink()->OnStateChange(tempstate);
            }
        }
    }
    if (m_stp.CheckAndResetFdbFlush())
    {
        m_macTable.clear();
    }
}
void CVSwitchImp::CheckAndUpdateLinkState()
{
    CVSwitchPort* ofport=NULL;

    std::unordered_map<uint32_t, CVSwitchPort*>::iterator it = m_sinklist.begin();
    while (it != m_sinklist.end())
    {
        ofport = it->second;
        CVSwitchSTPPort* pstpport = m_stp.FindPort(ofport->GetId());
        enum stp_state tempstate = m_stp.PortGetState(pstpport);
        if (ofport->GetSTPState() != tempstate)
        {
            ofport->SetSTPState(tempstate);
            if (ofport->GetSink())
            {
                ofport->GetSink()->OnStateChange(tempstate);
            }
        }
    }
}
void CVSwitchImp::GetAllItem(std::list<CMacTable> &list)
{

    std::unordered_map<MacKey, SwitchTableItem , MacKeyHashFun>::iterator it= m_macTable.begin();
    while(it != m_macTable.end())
    {
        SwitchTableItem &item=it->second;
        CMacTable macitem;

        item.GetMac(macitem.m_mac);

        list.push_back(macitem);
                
        it++;
    }

}
bool CVSwitchImp::LookUp(const char* eth, size_t ethsize, IDIPCVSwitchSink*& psink)
{
    char dstmac[ETHERNET_MAC_LENGTH] = { 0 };
    char srcmac[ETHERNET_MAC_LENGTH] = { 0 };
    memcpy(dstmac, eth, ETHERNET_MAC_LENGTH);
    memcpy(srcmac, eth + ETHERNET_MAC_LENGTH, ETHERNET_MAC_LENGTH);
    if (MACAddress::isMulticast(dstmac))
    {
        return false;
    }
    if (MACAddress::isBroadcast(dstmac))
    {
        return false;
    }
    SwitchTableItem item1;
    if (FindMacTable(dstmac, item1) != CAW_OK)
    {
        return false;
    }
    else
    {
        psink = item1.GetSink();
        return true;
    }
    return false;
}
bool CVSwitchImp::IsPortBlocking(IDIPCVSwitchSink* psink)
{
    CVSwitchPort* pport = NULL;
    pport = IsHaveSink(psink);
    if (pport == NULL)
    {
        return false;
    }
    CVSwitchSTPPort* pstpport = pport->GetSTPPort();
    if (pstpport == NULL)
    {
        return false;
    }
    if (!stp_blocking_in_state(m_stp.PortGetState(pstpport)))
    {
        return false;
    }

    return true;
}
bool CVSwitchImp::IsPortForward(IDIPCVSwitchSink* psink)
{
    CVSwitchPort* pport = NULL;
    pport = IsHaveSink(psink);
    if (pport == NULL)
    {
        return false;
    }
    CVSwitchSTPPort* pstpport = pport->GetSTPPort();
    if (pstpport == NULL)
    {
        return false;
    }
    if (!stp_forward_in_state(m_stp.PortGetState(pstpport)))
    {
        return false;
    }

    return true;
}
CAWResult CVSwitchImp::InsertMac(IDIPCVSwitchSink* psink, 
    const char* eth, 
    size_t ethsize)
{
    CVSwitchPort* pport = NULL;
    pport = IsHaveSink(psink);
    if (pport == NULL)
    {
        return CAW_ERROR_FAILURE;
    }
    char dstmac[ETHERNET_MAC_LENGTH] = { 0 };
    char srcmac[ETHERNET_MAC_LENGTH] = { 0 };
    memcpy(dstmac, eth, ETHERNET_MAC_LENGTH);
    memcpy(srcmac, eth + ETHERNET_MAC_LENGTH, ETHERNET_MAC_LENGTH);

    if (MACAddress::isBroadcast(srcmac))
    {
        return CAW_OK;
    }
    if (MACAddress::isMulticast(srcmac))
    {
        return CAW_OK;
    }
    SwitchTableItem itemfind;
    SwitchTableItem item(this, srcmac, pport->GetId(),psink);
    if (FindMacTable(srcmac, itemfind) != CAW_OK)
    {
        return AddMacTable(srcmac, item);
    }
    else
    {
        if (itemfind.GetSink() == psink)
        {
            return UpdateMacTable(srcmac, item);
        }
        else
        {
            RemoveMacTable(srcmac);
            return AddMacTable(srcmac, item);
        }
    }
    return CAW_OK;
}
IDIPCVSwitch* CreateMacLearningSwitch()
{
    CVSwitchImp* pvswitch = new CVSwitchImp();
    if (pvswitch)
    {
        pvswitch->Init();
    }
    return pvswitch;
}
CAWResult DestroyMacLearningSwitch(IDIPCVSwitch* pswitch)
{
    if (pswitch)
    {
        delete pswitch;
    }
    return CAW_OK;
}
void CVSwitchImp::SendSTPPacket(uint32_t id,
    const char* pkt,
    size_t pktsize)
{
    CVSwitchPort * pport = FindPort(id);
    if (pport)
    {
        CVSwitchSTPPort* pstpport = pport->GetSTPPort();
        if (pstpport == NULL)
    {
            return;
        }
        if (stp_blocking_in_state(m_stp.PortGetState(pstpport)))
        {
            return;
        }
        IDIPCVSwitchSink *psink = pport->GetSink();
        if (psink)
        {
            psink->OnOutput(NULL, pkt, pktsize,true);
        }
    }
}
