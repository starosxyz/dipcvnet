/***********************************************************************
 * Copyright (C) 2016-2021, Nanjing StarOS, Inc. 
**********************************************************************/
#ifndef CVSWITCHIMP_H
#define CVSWITCHIMP_H
#include <starbase/CAWACEInclude.h>
#include "IDIPCVSwitch.h"
#include "MacHashFun.h"
#include <starbase/CAWIDAllocator.h>
#include "CVSwitchSTP.h"

    class CVSwitchImp;
    class CVSwitchPort;
class SwitchTableItem 
{
public:
    SwitchTableItem();
    SwitchTableItem(CVSwitchImp *pvswitch);
    SwitchTableItem(CVSwitchImp *pvswitch,
        char macaddr[ETHERNET_MAC_LENGTH],
        uint32_t portid,
        IDIPCVSwitchSink *psink);
    SwitchTableItem(const SwitchTableItem &other);
    SwitchTableItem& operator=(const SwitchTableItem& other);
    bool operator==(const SwitchTableItem& other) const;
    bool IsTimeOut();
    void UpdateTime();
    inline IDIPCVSwitchSink *GetSink(){return m_psink;}
    void GetMac(char mac[ETHERNET_MAC_LENGTH]);
    inline uint32_t GetPortId() { return m_outputid; }
private:
    char m_macaddr[ETHERNET_MAC_LENGTH];
    CAWTimeValue m_currenttv;
    IDIPCVSwitchSink *m_psink;
	CVSwitchImp *m_vswitch;
    uint32_t m_outputid;
};

class CVSwitchImp : public IDIPCVSwitch
    ,public CAWTimerWrapperIDSink
    ,public IVSwitchSTPSink
{
public:
    CVSwitchImp();
    virtual ~CVSwitchImp();
    CAWResult Init();
    CAWResult Close();
public:
    virtual void SetTickInterval(long ms);
    virtual void SetBridgeId(uint64_t bridge_id);
    virtual void SetBridgePriority(uint16_t new_priority);
    virtual void SetHelloTime(long ms);
    virtual void SetMaxAge(long ms);
    virtual void SetForwardDelay(long ms);
    virtual uint64_t GetBridgeId();
    virtual uint64_t GetDesignatedRoot();
    virtual bool IsRootBridge();
    virtual long GetHelloTime();
    virtual long GetMaxAge();
    virtual long GetForwardDelay();
    virtual CAWResult AddSink(IDIPCVSwitchSink* psink,
        uint32_t path_cost = 50);
    virtual CAWResult RemoveSink(IDIPCVSwitchSink *psink);
    virtual CAWResult ClearTable();
    CVSwitchPort* FindPort(uint32_t id);
	virtual CAWResult SetMacItemTimeOutMs(long timeoutms);
    virtual CAWResult Input(IDIPCVSwitchSink *psink,
        const char *rcvdata,
        size_t datasize);
    CAWResult AddMacTable(const char mac[ETHERNET_MAC_LENGTH],SwitchTableItem &initem);
    CAWResult UpdateMacTable(const char mac[ETHERNET_MAC_LENGTH],SwitchTableItem &initem);
    CAWResult FindMacTable(const char mac[ETHERNET_MAC_LENGTH],SwitchTableItem &outitem);
    CAWResult RemoveMacTable(const char mac[ETHERNET_MAC_LENGTH]);
    CVSwitchPort *IsHaveSink(IDIPCVSwitchSink *psink);
    void OutputAllSink(IDIPCVSwitchSink *insink,const char *rcvdata,
        size_t datasize);
    virtual void GetAllItem(std::list<CMacTable> &list);
    virtual bool LookUp(const char* eth, size_t ethsize, IDIPCVSwitchSink*& psink);
    virtual CAWResult InsertMac(IDIPCVSwitchSink* psink,const char* eth, size_t ethsize);
    virtual bool IsPortForward(IDIPCVSwitchSink* psink);
    virtual bool IsPortBlocking(IDIPCVSwitchSink* psink);
    void CheckAndUpdateLinkState();
public:
    //CAWTimerWrapperIDSink
    virtual void OnTimer(CAWTimerWrapperID* aId);
public:
	inline long GetTimeOut(){return m_timeoutms;}
public:
    virtual void SendSTPPacket(uint32_t id, const char* pkt, size_t pktsize);
private:      
    CAWIDAllocator m_idbase;
    std::unordered_map<uint32_t, CVSwitchPort*> m_sinklist;
    std::unordered_map<MacKey, SwitchTableItem , MacKeyHashFun> m_macTable;
    CAWTimerWrapperID m_timeout;
	long m_timeoutms;
    CVSwitchSTP m_stp;
    long m_TimeoutInterval;
};

#endif //CVSWITCHLIB_H

