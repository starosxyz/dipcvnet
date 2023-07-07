/***********************************************************************
 *   版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef STP_H
#define STP_H
#include <starbase/CAWACEInclude.h>
#include <wface/CAWACEWrapper.h>
#include "IDIPCVSwitch.h"
#include "vswitch.h"

const char* stp_state_name(enum stp_state state);
bool stp_forward_in_state(enum stp_state state);
bool stp_blocking_in_state(enum stp_state state);
bool stp_should_forward_bpdu(enum stp_state state);
bool stp_learn_in_state(enum stp_state state);
class CVSwitchSTPPort;
class IVSwitchSTPSink
{
public:
    virtual ~IVSwitchSTPSink() {}
public:
    virtual void SendSTPPacket(uint32_t id, const char* pkt, size_t pktsize) = 0;
};

class CVSwitchSTP
{
public:
    CVSwitchSTP(IVSwitchSTPSink *p,
        const CAWString& name, 
        stp_identifier bridge_id,
        void* aux);
    virtual ~CVSwitchSTP();
public:
    CAWResult AddPort(uint32_t id,CVSwitchSTPPort* pport);
    CVSwitchSTPPort* FindPort(uint32_t id);
    CAWResult RemovePort(uint32_t id);
    CAWResult CreatePort(uint32_t id, uint32_t path_cost, enum stp_state state);
    CAWResult DestroyPort(uint32_t id);
    void GetEnablePortList(std::list< CVSwitchSTPPort*>& list);
public:
    void Tick(int ms);
    void BridgeId(stp_identifier new_bridge_id);
    void SetBridgeId(stp_identifier bridge_id);
    void SetBridgePriority(uint16_t new_priority);
    void SetHelloTime(int ms);
    void SetMaxAge(int ms);
    void SetForwardDelay(int ms);
    const char* GetName();
    stp_identifier GetBridgeId();
    stp_identifier GetDesignatedRoot();
    bool IsRootBridge();
    int GetRootPathCost();
    int GetHelloTime();
    int GetMaxAge();
    int GetForwardDelay();
    bool CheckAndResetFdbFlush();
    CVSwitchSTPPort* GetPort(int port_no);
    CVSwitchSTPPort* GetRootPort();
    bool GetChangedPort(std::list<CVSwitchSTPPort*>& portplist);
    void ReceivedBpdu(CVSwitchSTPPort* p, const void* bpdu, size_t bpdu_size);
    CVSwitchSTP* PortGetStp(CVSwitchSTPPort* p);
    void PortSetName(CVSwitchSTPPort* p, const CAWString& name);
    void PortSetAux(CVSwitchSTPPort* p, void* aux);
    void* PortGetAux(CVSwitchSTPPort* p);
    int PortNo(const CVSwitchSTPPort* p);
    enum stp_state PortGetState(const CVSwitchSTPPort* p);
    enum stp_role PortGetRole(const CVSwitchSTPPort* p);
    void PortGetCounts(const CVSwitchSTPPort* p,
        int* tx_count, int* rx_count, int* error_count);
    void PortGetStatus(CVSwitchSTPPort* p,
        int* port_id, enum stp_state* state, enum stp_role* role);
    void PortDisable(CVSwitchSTPPort* p);
    void PortEnable(CVSwitchSTPPort* p);
    void PortSetPriority(CVSwitchSTPPort* p, uint8_t new_priority);
    uint16_t ConvertSpeedToCost(unsigned int speed);
    void PortSetPathCost(CVSwitchSTPPort* p, uint16_t path_cost);
    void PortSetSpeed(CVSwitchSTPPort* p, unsigned int speed);
    void PortEnableChangeDetection(CVSwitchSTPPort* p);
    void PortDisableChangeDetection(CVSwitchSTPPort* p);
    bool SupersedesPortInfo(CVSwitchSTPPort* p,
            const struct stp_config_bpdu* config);
    void RecordConfigInformation(CVSwitchSTPPort* p,
        const struct stp_config_bpdu* config);
    void RecordConfigTimeoutValues(const struct stp_config_bpdu* config);
    bool IsDesignatedPort(const CVSwitchSTPPort* p);
    void ConfigBpduGeneration();
    void TransmitTcn();
    void ConfigurationUpdate();
    bool SupersedesRoot(const CVSwitchSTPPort* root, CVSwitchSTPPort* p);
    void RootSelection();
    void DesignatedPortSelection();
    void PortStateSelection();
    void BecomeDesignatedPort(CVSwitchSTPPort* p);
    void MakeForwarding(CVSwitchSTPPort* p);
    void MakeBlocking(CVSwitchSTPPort* p);
    void SetPortState(CVSwitchSTPPort* p, enum stp_state state);
    void TopologyChangeDetection();
    void TopologyChangeAcknowledged();
    void AcknowledgeTopologyChange(CVSwitchSTPPort* p);
    void ReceivedConfigBpdu(CVSwitchSTPPort* p,const struct stp_config_bpdu* config);
    void ReceivedTcnBpdu(CVSwitchSTPPort* p);
    void MessageAgeTimerExpiry(CVSwitchSTPPort* p);
    void HelloTimerExpiry();
    bool IsDesignatedForSomePort();
    void ForwardDelayTimerExpiry(CVSwitchSTPPort* p);
    void TcnTimerExpiry();
    void TopologyChangeTimerExpiry();
    void HoldTimerExpiry(CVSwitchSTPPort* p);
    void InitializePort(CVSwitchSTPPort* p, enum stp_state state);
    void BecomeRootBridge();
    void StartTimer(struct stp_timer* timer, int value);
    void StopTimer(struct stp_timer* timer);
    bool TimerExpired(struct stp_timer* timer, int elapsed, int timeout);
    void UpdateBridgeTimers();
    void TransmitConfig(CVSwitchSTPPort* p);
    void SendBPDU(CVSwitchSTPPort* p, const void* bpdu, size_t bpdu_size);
    bool MayReceive(CVSwitchSTPPort* pport);
public:

public:
    /* Static bridge data. */
    CAWString name;                     /* Human-readable name for log messages. */
    stp_identifier bridge_id;       /* 8.5.3.7: This bridge. */
    int max_age;                    /* 8.5.3.4: Time to drop received data. */
    int hello_time;                 /* 8.5.3.5: Time between sending BPDUs. */
    int forward_delay;              /* 8.5.3.6: Delay between state changes. */
    int bridge_max_age;             /* 8.5.3.8: max_age when we're root. */
    int bridge_hello_time;          /* 8.5.3.9: hello_time as root. */
    int bridge_forward_delay;       /* 8.5.3.10: forward_delay as root. */
    int rq_max_age;                 /* User-requested max age, in ms. */
    int rq_hello_time;              /* User-requested hello time, in ms. */
    int rq_forward_delay;           /* User-requested forward delay, in ms. */
    int elapsed_remainder;          /* Left-over msecs from last stp_tick(). */

    /* Dynamic bridge data. */
    stp_identifier designated_root; /* 8.5.3.1: Bridge believed to be root. */
    unsigned int root_path_cost;    /* 8.5.3.2: Cost of path to root. */
    CVSwitchSTPPort* root_port;     /* 8.5.3.3: Lowest cost port to root. */
    bool topology_change_detected;  /* 8.5.3.11: Detected a topology change? */
    bool topology_change;           /* 8.5.3.12: Received topology change? */

    /* Bridge timers. */
    struct stp_timer hello_timer;   /* 8.5.4.1: Hello timer. */
    struct stp_timer tcn_timer;     /* 8.5.4.2: Topology change timer. */
    struct stp_timer topology_change_timer; /* 8.5.4.3. */

    /* Ports. */
    std::unordered_map<uint32_t, CVSwitchSTPPort*> ports;
    /* Interface to client. */
    bool fdb_needs_flush;          /* MAC learning tables needs flushing. */
    CVSwitchSTPPort* first_changed_port;

    void* aux;

    IVSwitchSTPSink* m_psink;
};
#endif