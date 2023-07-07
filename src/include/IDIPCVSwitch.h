/***********************************************************************
 * Copyright (C) 2016-2021, Nanjing StarOS, Inc. 
**********************************************************************/
#ifndef __IDIPCVSWITCH_H__
#define __IDIPCVSWITCH_H__
#include <dipc/dipc.h>
#include "utils/CMacTable.h"
using namespace starbase;
#define ETHERNET_MAC_LENGTH         6
#define DEFAULT_COST                100
class CAW_OS_EXPORT IDIPCVSwitchSink
{
public:
    virtual ~IDIPCVSwitchSink(){}
    virtual void OnOutput(IDIPCVSwitchSink *from,
        const char *rcvdata,
        size_t datasize,
        bool flood) = 0;
    virtual void OnStateChange(int state) = 0;
};

class CAW_OS_EXPORT IDIPCVSwitch
{
public:
    virtual ~IDIPCVSwitch(){};
    virtual void SetTickInterval(long ms) = 0;
    virtual void SetBridgeId(uint64_t bridge_id) = 0;
    virtual void SetBridgePriority(uint16_t new_priority) = 0;
    virtual void SetHelloTime(long ms) = 0;
    virtual void SetMaxAge(long ms) = 0;
    virtual void SetForwardDelay(long ms) = 0;
    virtual uint64_t GetBridgeId() = 0;
    virtual uint64_t GetDesignatedRoot() = 0;
    virtual bool IsRootBridge() = 0;
    virtual long GetHelloTime() = 0;
    virtual long GetMaxAge() = 0;
    virtual long GetForwardDelay() = 0;
    virtual CAWResult ClearTable() = 0;
	virtual CAWResult SetMacItemTimeOutMs(long timeoutms) = 0;
    virtual CAWResult AddSink(IDIPCVSwitchSink* psink,
        uint32_t path_cost=50) = 0;
    virtual CAWResult RemoveSink(IDIPCVSwitchSink *psink) = 0;
    virtual CAWResult Input(IDIPCVSwitchSink *psink,
        const char *rcvdata,
        size_t datasize) = 0;
    virtual void GetAllItem(std::list<CMacTable> &list) = 0;
	virtual bool LookUp(const char* eth, size_t ethsize, IDIPCVSwitchSink*& psink)=0;
	virtual CAWResult InsertMac(IDIPCVSwitchSink* psink, const char* eth, size_t ethsize)=0;
    virtual bool IsPortForward(IDIPCVSwitchSink* psink) = 0;
    virtual bool IsPortBlocking(IDIPCVSwitchSink* psink) = 0;
};

extern "C"
{
	CAW_OS_EXPORT IDIPCVSwitch* CreateMacLearningSwitch();
	CAW_OS_EXPORT CAWResult DestroyMacLearningSwitch(IDIPCVSwitch *pswitch);
}

#endif//__IDIPCVSWITCH_H__

