/***********************************************************************
 *   版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include "CVSwitchSTP.h"
#include "vswitch.h"
#include "CVSwitchSTPPort.h"
#include "packet/STPPacket.h"
static int clamp(int x, int min, int max);
static int ms_to_timer(int ms);
static int timer_to_ms(int timer);

CVSwitchSTP::~CVSwitchSTP()
{
    CAW_INFO_TRACE("CVSwitchSTP::~CVSwitchSTP");
}
CAWResult CVSwitchSTP::AddPort(uint32_t id, CVSwitchSTPPort* pport)
{
    CAW_INFO_TRACE("CVSwitchSTP::AddPort id="<<id);
    InitializePort(pport, STP_DISABLED);
    if (ports.size() == 0)
    {
        first_changed_port = pport;
    }
    std::unordered_map<uint32_t, CVSwitchSTPPort*>::iterator it= ports.find(id);
    if (it != ports.end())
    {
        return CAW_ERROR_FAILURE;
    }
    ports[id] = pport;
    return CAW_OK;
}
CVSwitchSTPPort* CVSwitchSTP::FindPort(uint32_t id)
{
    std::unordered_map<uint32_t, CVSwitchSTPPort*>::iterator it = ports.find(id);
    if (it != ports.end())
    {
        return it->second;
    }
    return NULL;
}
CAWResult CVSwitchSTP::RemovePort(uint32_t id)
{
    CAW_INFO_TRACE("CVSwitchSTP::RemovePort id=" << id);
    std::unordered_map<uint32_t, CVSwitchSTPPort*>::iterator it = ports.find(id);
    if (it == ports.end())
    {
        return CAW_ERROR_FAILURE;
    }
    ports.erase(it);
    return CAW_OK;
}
CAWResult CVSwitchSTP::CreatePort(uint32_t id,
    uint32_t path_cost,
    enum stp_state state)
{
    CAW_INFO_TRACE("CVSwitchSTP::CreatePort id=" << id);
    CVSwitchSTPPort* pport = new CVSwitchSTPPort(this, id, path_cost, state);
    if (pport == NULL)
    {
        return CAW_ERROR_FAILURE;
    }
    if (AddPort(id, pport) != CAW_OK)
    {
        delete pport;
        return CAW_ERROR_FAILURE;
    }
    return CAW_OK;
}
CAWResult CVSwitchSTP::DestroyPort(uint32_t id)
{
    CAW_INFO_TRACE("CVSwitchSTP::DestroyPort id=" << id);
    CVSwitchSTPPort* p = FindPort(id);
    if (p)
    {
        RemovePort(id);
        delete p;
    }
    return CAW_OK;
}
void CVSwitchSTP::GetEnablePortList(std::list< CVSwitchSTPPort*>& list)
{
    std::unordered_map<uint32_t, CVSwitchSTPPort*>::iterator it = ports.begin();
    while (it != ports.end())
    {
        CVSwitchSTPPort * port = it->second;
        if (port->state != STP_DISABLED)
        {
            list.push_back(port);
        }
        it++;
    }
}
/* Creates and returns a new STP instance that initially has no ports enabled.
 *
 * 'bridge_id' should be a 48-bit MAC address as returned by
 * eth_addr_to_uint64().  'bridge_id' may also have a priority value in its top
 * 16 bits; if those bits are set to 0, STP_DEFAULT_BRIDGE_PRIORITY is used.
 * (This priority may be changed with stp_set_bridge_priority().)
 *
 * When the bridge needs to send out a BPDU, it calls 'send_bpdu'.  This
 * callback may be called from stp_tick() or stp_received_bpdu().  The
 * arguments to 'send_bpdu' are an STP BPDU encapsulated in 'bpdu',
 * the spanning tree port number 'port_no' that should transmit the
 * packet, and auxiliary data to be passed to the callback in 'aux'.
 */
CVSwitchSTP::CVSwitchSTP(IVSwitchSTPSink* psink,
                        const CAWString &pname, 
                        stp_identifier nbridge_id,
                        void *paux)
{
    name = pname;


    bridge_id = nbridge_id;
    if (!(bridge_id >> 48)) {
        bridge_id |= (uint64_t) STP_DEFAULT_BRIDGE_PRIORITY << 48;
    }
    CAW_INFO_TRACE("CVSwitchSTP::CVSwitchSTP");

    max_age = 0;
    hello_time = 0;
    forward_delay = 0;
    bridge_max_age = 0;
    bridge_hello_time = 0;
    bridge_forward_delay = 0;

    rq_max_age = STP_DEFAULT_MAX_AGE;
    rq_hello_time = STP_DEFAULT_HELLO_TIME;
    rq_forward_delay = STP_DEFAULT_FWD_DELAY;
    UpdateBridgeTimers();
    max_age = bridge_max_age;
    hello_time = bridge_hello_time;
    forward_delay = bridge_forward_delay;

    designated_root = bridge_id;
    root_path_cost = 0;
    root_port = NULL;
    topology_change_detected = false;
    topology_change = false;

    StopTimer(&tcn_timer);
    StopTimer(&topology_change_timer);
    StartTimer(&hello_timer, hello_time);
    fdb_needs_flush = false;
    m_psink = psink;
    aux = paux;
}

/* Runs 'stp' given that 'ms' milliseconds have passed. */
void
CVSwitchSTP::Tick(int ms)
{
    CVSwitchSTPPort *p=NULL;
    int elapsed=0;
    CAW_INFO_TRACE("CVSwitchSTP::Tick ms="<<ms);
    /* Convert 'ms' to STP timer ticks.  Preserve any leftover milliseconds
     * from previous stp_tick() calls so that we don't lose STP ticks when we
     * are called too frequently. */
    ms = clamp(ms, 0, INT_MAX - 1000) + elapsed_remainder;
    elapsed = ms_to_timer(ms);
    elapsed_remainder = ms - timer_to_ms(elapsed);
    if (!elapsed) {
        CAW_INFO_TRACE("CVSwitchSTP::Tick elapsed=" << elapsed);
        return;
    }
    CAW_INFO_TRACE("CVSwitchSTP::Tick2");
    if (TimerExpired(&hello_timer, elapsed, hello_time)) {
        HelloTimerExpiry();
    }
    if (TimerExpired(&tcn_timer, elapsed, bridge_hello_time)) {
        TcnTimerExpiry();
    }
    if (TimerExpired(&topology_change_timer, elapsed,
                          max_age + forward_delay)) {
        TopologyChangeTimerExpiry();
    }
    std::list<CVSwitchSTPPort*> portlist;
    GetEnablePortList(portlist);
    for(CVSwitchSTPPort *p:portlist)
    {
        if (TimerExpired(&p->message_age_timer, elapsed, max_age)) {
            MessageAgeTimerExpiry(p);
        }
    }
    for (CVSwitchSTPPort* p : portlist)
    {
        if (TimerExpired(&p->forward_delay_timer, elapsed,
                              forward_delay)) {
            ForwardDelayTimerExpiry(p);
        }
        if (TimerExpired(&p->hold_timer, elapsed, ms_to_timer(1000))) {
            HoldTimerExpiry(p);
        }
    }
    CAW_INFO_TRACE("CVSwitchSTP::Tick end");
}

void
CVSwitchSTP::BridgeId(stp_identifier new_bridge_id)
{
    if (new_bridge_id != bridge_id) {
        bool root;
        root = IsRootBridge();
        std::list<CVSwitchSTPPort*> portlist;
        GetEnablePortList(portlist);
        for (CVSwitchSTPPort* p : portlist)
        {
            if (IsDesignatedPort(p)) {
                p->designated_bridge = new_bridge_id;
            }
        }
        bridge_id = new_bridge_id;
        ConfigurationUpdate();
        PortStateSelection();
        if (IsRootBridge() && !root) {
            BecomeRootBridge();
        }
    }
}

void
CVSwitchSTP::SetBridgeId(stp_identifier bridge_id)
{
    const uint64_t mac_bits = (uint64_t(1) << 48) - 1;
    const uint64_t pri_bits = ~mac_bits;
    BridgeId((bridge_id & pri_bits) | (bridge_id & mac_bits));
}

void
CVSwitchSTP::SetBridgePriority(uint16_t new_priority)
{
    const uint64_t mac_bits = (uint64_t(1) << 48) - 1;
    BridgeId(((bridge_id & mac_bits)
                        | ((uint64_t) new_priority << 48)));
}

/* Sets the desired hello time for 'stp' to 'ms', in milliseconds.  The actual
 * hello time is clamped to the range of 1 to 10 seconds and subject to the
 * relationship (bridge_max_age >= 2 * (bridge_hello_time + 1 s)).  The bridge
 * hello time is only used when 'stp' is the root bridge. */
void
CVSwitchSTP::SetHelloTime(int ms)
{
    rq_hello_time = ms;
    UpdateBridgeTimers();
}

/* Sets the desired max age for 'stp' to 'ms', in milliseconds.  The actual max
 * age is clamped to the range of 6 to 40 seconds and subject to the
 * relationships (2 * (bridge_forward_delay - 1 s) >= bridge_max_age) and
 * (bridge_max_age >= 2 * (bridge_hello_time + 1 s)).  The bridge max age is
 * only used when 'stp' is the root bridge. */
void
CVSwitchSTP::SetMaxAge(int ms)
{
    rq_max_age = ms;
    UpdateBridgeTimers();
}

/* Sets the desired forward delay for 'stp' to 'ms', in milliseconds.  The
 * actual forward delay is clamped to the range of 4 to 30 seconds and subject
 * to the relationship (2 * (bridge_forward_delay - 1 s) >= bridge_max_age).
 * The bridge forward delay is only used when 'stp' is the root bridge. */
void
CVSwitchSTP::SetForwardDelay(int ms)
{
    rq_forward_delay = ms;
    UpdateBridgeTimers();
}

/* Returns the name given to 'stp' in the call to stp_create(). */
const char *
CVSwitchSTP::GetName()
{
    return name;
}

/* Returns the bridge ID for 'stp'. */
stp_identifier
CVSwitchSTP::GetBridgeId()
{
    return bridge_id;
}

/* Returns the bridge ID of the bridge currently believed to be the root. */
stp_identifier
CVSwitchSTP::GetDesignatedRoot()
{
    return designated_root;
}

/* Returns the cost of the path from 'stp' to the root of the spanning tree. */
int
CVSwitchSTP::GetRootPathCost()
{
    int cost;
    cost = root_path_cost;
    return cost;
}

/* Returns the bridge hello time, in ms.  The returned value is not necessarily
 * the value passed to stp_set_hello_time(): it is clamped to the valid range
 * and quantized to the STP timer resolution.  */
int
CVSwitchSTP::GetHelloTime()
{
    int time;

    time = timer_to_ms(bridge_hello_time);
    return time;
}

/* Returns the bridge max age, in ms.  The returned value is not necessarily
 * the value passed to stp_set_max_age(): it is clamped to the valid range,
 * quantized to the STP timer resolution, and adjusted to match the constraints
 * due to the hello time.  */
int
CVSwitchSTP::GetMaxAge()
{
    int time;

    time = timer_to_ms(bridge_max_age);
    return time;
}

/* Returns the bridge forward delay, in ms.  The returned value is not
 * necessarily the value passed to stp_set_forward_delay(): it is clamped to
 * the valid range, quantized to the STP timer resolution, and adjusted to
 * match the constraints due to the forward delay.  */
int
CVSwitchSTP::GetForwardDelay()
{
    int time;

    time = timer_to_ms(bridge_forward_delay);
    return time;
}

/* Returns true if something has happened to 'stp' which necessitates flushing
 * the client's MAC learning table.  Calling this function resets 'stp' so that
 * future calls will return false until flushing is required again. */
bool
CVSwitchSTP::CheckAndResetFdbFlush()
{
    bool needs_flush=false;

    needs_flush = fdb_needs_flush;
    fdb_needs_flush = false;
    return needs_flush;
}

/* Returns the port in 'stp' with index 'port_no', which must be between 0 and
 * STP_MAX_PORTS. */
CVSwitchSTPPort *CVSwitchSTP::GetPort(int port_no)
{
    CVSwitchSTPPort *port;

    port = ports[port_no];
    return port;
}

/* Returns the port connecting 'stp' to the root bridge, or a null pointer if
 * there is no such port. */
CVSwitchSTPPort *CVSwitchSTP::GetRootPort()
{
    return root_port;
}

/* Finds a port whose state has changed.  If successful, stores the port whose
 * state changed in '*portp' and returns true.  If no port has changed, stores
 * NULL in '*portp' and returns false. */
bool
CVSwitchSTP::GetChangedPort(std::list<CVSwitchSTPPort*> &portplist)
{
    bool changed = false;

    std::unordered_map<uint32_t, CVSwitchSTPPort*>::iterator it = ports.begin();
    while (it != ports.end())
    {
        CVSwitchSTPPort* p = it->second;
        if (p->state_changed) {
            p->state_changed = false;
            portplist.push_back(p);
            changed = true;
        }
        it++;
    }
    return changed;
}

/* Returns the name for the given 'state' (for use in debugging and log
 * messages). */
const char *
stp_state_name(enum stp_state state)
{
    switch (state) {
    case STP_DISABLED:
        return "disabled";
    case STP_LISTENING:
        return "listening";
    case STP_LEARNING:
        return "learning";
    case STP_FORWARDING:
        return "forwarding";
    case STP_BLOCKING:
        return "blocking";
    default:
        return "null";
    }
}
bool
stp_blocking_in_state(enum stp_state state)
{
    return (state & STP_BLOCKING) != 0;
}
/* Returns true if 'state' is one in which packets received on a port should
 * be forwarded, false otherwise.
 */
bool
stp_forward_in_state(enum stp_state state)
{
    return (state & STP_FORWARDING) != 0;
}

/* Returns true if 'state' is one in which MAC learning should be done on
 * packets received on a port, false otherwise.
 */
bool
stp_learn_in_state(enum stp_state state)
{
    return (state & (STP_LEARNING | STP_FORWARDING)) != 0;
}

/* Returns true if 'state' is one in which bpdus should be forwarded on a
 * port, false otherwise.
 *
 * Returns true if 'state' is STP_DISABLED, since in that case the port does
 * not generate the bpdu and should just forward it (e.g. patch port on pif
 * bridge). */
bool
stp_should_forward_bpdu(enum stp_state state)
{
    return (state &
            ( STP_DISABLED | STP_LISTENING | STP_LEARNING
              | STP_FORWARDING)) != 0;
}

/* Returns the name for the given 'role' (for use in debugging and log
 * messages). */
const char *RoleName(enum stp_role role)
{
    switch (role) {
    case STP_ROLE_ROOT:
        return "root";
    case STP_ROLE_DESIGNATED:
        return "designated";
    case STP_ROLE_ALTERNATE:
        return "alternate";
    case STP_ROLE_DISABLED:
        return "disabled";
    default:
        return "null";
    }
}

/* Notifies the STP entity that bridge protocol data unit 'bpdu', which is
 * 'bpdu_size' bytes in length, was received on port 'p'.
 *
 * This function may call the 'send_bpdu' function provided to stp_create(). */
void
CVSwitchSTP::ReceivedBpdu(CVSwitchSTPPort *p, 
    const void *bpdu, 
    size_t bpdu_size)
{
    CVSwitchSTP *stp = p->stp;
    const struct stp_bpdu_header *header;
    CAW_INFO_TRACE("CVSwitchSTP::ReceivedBpdu p->state="<< p->state);
    if (p->state == STP_DISABLED) {
        return;
    }

    if (bpdu_size < sizeof(struct stp_bpdu_header)) {
        p->error_count++;
        CAW_ERROR_TRACE("CVSwitchSTP::ReceivedBpdu bpdu_size" << bpdu_size);
        return;
    }

    header = (const struct stp_bpdu_header*)bpdu;
    if (header->protocol_id != htons(STP_PROTOCOL_ID)) {
        p->error_count++;
        CAW_ERROR_TRACE("CVSwitchSTP::ReceivedBpdu header->protocol_id" << header->protocol_id);
        return;
    }
    if (header->protocol_version != STP_PROTOCOL_VERSION) {
        CAW_ERROR_TRACE("CVSwitchSTP::ReceivedBpdu header->protocol_version" << header->protocol_version);
    }
    CAW_INFO_TRACE("CVSwitchSTP::ReceivedBpdu bpdu_type=" << header->bpdu_type);
    switch (header->bpdu_type) {
    case STP_TYPE_CONFIG:
        if (bpdu_size < sizeof(struct stp_config_bpdu)) {
            p->error_count++;
            return;
        }
        ReceivedConfigBpdu(p, (const struct stp_config_bpdu*)bpdu);
        break;

    case STP_TYPE_TCN:
        if (bpdu_size != sizeof(struct stp_tcn_bpdu)) {
            p->error_count++;
            return;
        }
        ReceivedTcnBpdu(p);
        break;

    default:
        p->error_count++;
        return;
    }
    p->rx_count++;
}

/* Returns the STP entity in which 'p' is nested. */
CVSwitchSTP* CVSwitchSTP::PortGetStp(CVSwitchSTPPort *p)
{
    return p->stp;
}

void
CVSwitchSTP::PortSetName(CVSwitchSTPPort *p, const CAWString &name)
{
    p->port_name = name;
}

/* Sets the 'aux' member of 'p'.
 *
 * The 'aux' member will be reset to NULL when stp_port_disable() is
 * called or stp_port_enable() is called when the port is in a Disabled
 * state. */
void
CVSwitchSTP::PortSetAux(CVSwitchSTPPort *p, void *aux)
{
    p->aux = aux;
}

/* Returns the 'aux' member of 'p'. */
void *
CVSwitchSTP::PortGetAux(CVSwitchSTPPort *p)
{
    return p->aux;
}

/* Returns the state of port 'p'. */
enum stp_state
CVSwitchSTP::PortGetState(const CVSwitchSTPPort *p)
{
    return p->state;
}

/* Returns the role of port 'p'. */
enum stp_role
CVSwitchSTP::PortGetRole(const CVSwitchSTPPort *p)
{
    CVSwitchSTPPort *root_port;
    enum stp_role role;

    root_port = p->stp->root_port;
    if (root_port && root_port->port_id == p->port_id) {
        role = STP_ROLE_ROOT;
    } else if (IsDesignatedPort(p)) {
        role = STP_ROLE_DESIGNATED;
    } else if (p->state == STP_DISABLED) {
        role = STP_ROLE_DISABLED;
    } else {
        role = STP_ROLE_ALTERNATE;
    }
    return role;
}

/* Retrieves BPDU transmit and receive counts for 'p'. */
void
CVSwitchSTP::PortGetCounts(const CVSwitchSTPPort *p,
                    int *tx_count, int *rx_count, int *error_count)
{
    *tx_count = p->tx_count;
    *rx_count = p->rx_count;
    *error_count = p->error_count;
}

void
CVSwitchSTP::PortGetStatus(CVSwitchSTPPort *p,
                    int *port_id, enum stp_state *state, enum stp_role *role)
{
    *port_id = p->port_id;
    *state = p->state;
    *role = PortGetRole(p);
}

/* Disables STP on port 'p'. */
void
CVSwitchSTP::PortDisable(CVSwitchSTPPort *p)
{
    if (p->state != STP_DISABLED) {
        bool root = IsRootBridge();
        BecomeDesignatedPort(p);
        SetPortState(p, STP_DISABLED);
        p->topology_change_ack = false;
        p->config_pending = false;
        StopTimer(&p->message_age_timer);
        StopTimer(&p->forward_delay_timer);
        ConfigurationUpdate();
        PortStateSelection();
        if (IsRootBridge() && !root) {
            BecomeRootBridge();
        }
        p->aux = NULL;
    }
}

/* Enables STP on port 'p'.  The port will initially be in "blocking" state. */
void
CVSwitchSTP::PortEnable(CVSwitchSTPPort *p)
{
    if (p->state == STP_DISABLED) {
        InitializePort(p, STP_BLOCKING);
        p->stp->PortStateSelection();
    }
}

/* Sets the priority of port 'p' to 'new_priority'.  Lower numerical values
 * are interpreted as higher priorities. */
void
CVSwitchSTP::PortSetPriority(CVSwitchSTPPort *p, uint8_t new_priority)
{
    uint16_t new_port_id;

    new_port_id  = (p->port_id & 0xff) | (new_priority << 8);
    if (p->port_id != new_port_id) {
        CVSwitchSTP *stp = p->stp;
        if (IsDesignatedPort(p)) {
            p->designated_port = new_port_id;
        }
        p->port_id = new_port_id;
        if (stp->bridge_id == p->designated_bridge
            && p->port_id < p->designated_port) {
            BecomeDesignatedPort(p);
            stp->PortStateSelection();
        }
    }
}

/* Convert 'speed' (measured in Mb/s) into the path cost. */
uint16_t
CVSwitchSTP::ConvertSpeedToCost(unsigned int speed)
{
    uint16_t ret;

    ret = speed >= 10000 ? 2  /* 10 Gb/s. */
        : speed >= 1000 ? 4 /* 1 Gb/s. */
        : speed >= 100 ? 19 /* 100 Mb/s. */
        : speed >= 16 ? 62  /* 16 Mb/s. */
        : speed >= 10 ? 100 /* 10 Mb/s. */
        : speed >= 4 ? 250  /* 4 Mb/s. */
        : 19;             /* 100 Mb/s (guess). */
    return ret;
}

/* Sets the path cost of port 'p' to 'path_cost'.  Lower values are generally
 * used to indicate faster links.  Use stp_port_set_speed() to automatically
 * generate a default path cost from a link speed. */
void
CVSwitchSTP::PortSetPathCost(CVSwitchSTPPort *p, uint16_t path_cost)
{
    if (p->path_cost != path_cost) {
        CVSwitchSTP *stp = p->stp;
        p->path_cost = path_cost;
        stp->ConfigurationUpdate();
        stp->PortStateSelection();
    }
}

/* Sets the path cost of port 'p' based on 'speed' (measured in Mb/s). */
void
CVSwitchSTP::PortSetSpeed(CVSwitchSTPPort *p, unsigned int speed)
{
    PortSetPathCost(p, ConvertSpeedToCost(speed));
}

/* Enables topology change detection on port 'p'. */
void
CVSwitchSTP::PortEnableChangeDetection(CVSwitchSTPPort *p)
{
    p->change_detection_enabled = true;
}

/* Disables topology change detection on port 'p'. */
void
CVSwitchSTP::PortDisableChangeDetection(CVSwitchSTPPort *p)
{
    p->change_detection_enabled = false;
}
bool
CVSwitchSTP::SupersedesPortInfo(CVSwitchSTPPort *p,
                         const struct stp_config_bpdu *config)
{
    if (caw_ntohll(config->root_id) != p->designated_root) {
        return caw_ntohll(config->root_id) < p->designated_root;
    } else if (ntohl(config->root_path_cost) != p->designated_cost) {
        return ntohl(config->root_path_cost) < p->designated_cost;
    } else if (caw_ntohll(config->bridge_id) != p->designated_bridge) {
        return caw_ntohll(config->bridge_id) < p->designated_bridge;
    } else {
        return (caw_ntohll(config->bridge_id) != p->stp->bridge_id
                || ntohs(config->port_id) <= p->designated_port);
    }
}

void
CVSwitchSTP::RecordConfigInformation(CVSwitchSTPPort *p,
                              const struct stp_config_bpdu *config)
{
    p->designated_root = caw_ntohll(config->root_id);
    p->designated_cost = ntohl(config->root_path_cost);
    p->designated_bridge = caw_ntohll(config->bridge_id);
    p->designated_port = ntohs(config->port_id);
    StartTimer(&p->message_age_timer, ntohs(config->message_age));
}

void
CVSwitchSTP::RecordConfigTimeoutValues(const struct stp_config_bpdu  *config)
{
    max_age = ntohs(config->max_age);
    hello_time = ntohs(config->hello_time);
    forward_delay = ntohs(config->forward_delay);
    topology_change = config->flags & STP_CONFIG_TOPOLOGY_CHANGE;
}

bool
CVSwitchSTP::IsDesignatedPort(const CVSwitchSTPPort *p)
{
    return (p->designated_bridge == p->stp->bridge_id
            && p->designated_port == p->port_id);
}

void
CVSwitchSTP::ConfigBpduGeneration()
{
    std::list<CVSwitchSTPPort*> portlist;
    GetEnablePortList(portlist);
    for (CVSwitchSTPPort* p : portlist)
    {
        if (IsDesignatedPort(p)) {
            TransmitConfig(p);
        }
    }
}

void
CVSwitchSTP::TransmitTcn()
{
    CVSwitchSTPPort *p = root_port;
    struct stp_tcn_bpdu tcn_bpdu;
    CAW_INFO_TRACE("CVSwitchSTP::TransmitTcn");
    if (!p) {
        return;
    }

    tcn_bpdu.header.protocol_id = htons(STP_PROTOCOL_ID);
    tcn_bpdu.header.protocol_version = STP_PROTOCOL_VERSION;
    tcn_bpdu.header.bpdu_type = STP_TYPE_TCN;
    SendBPDU(p, &tcn_bpdu, sizeof tcn_bpdu);
}

void
CVSwitchSTP::ConfigurationUpdate()
{
    RootSelection();
    DesignatedPortSelection();
    //seq_change(connectivity_seq_get());
}

bool
CVSwitchSTP::SupersedesRoot(const CVSwitchSTPPort *root, CVSwitchSTPPort *p)
{
    int p_cost = p->designated_cost + p->path_cost;
    int root_cost = root->designated_cost + root->path_cost;

    if (p->designated_root != root->designated_root) {
        return p->designated_root < root->designated_root;
    } else if (p_cost != root_cost) {
        return p_cost < root_cost;
    } else if (p->designated_bridge != root->designated_bridge) {
        return p->designated_bridge < root->designated_bridge;
    } else if (p->designated_port != root->designated_port) {
        return p->designated_port < root->designated_port;
    } else {
        return p->port_id < root->port_id;
    }
}

void
CVSwitchSTP::RootSelection()
{
    CVSwitchSTPPort *root=NULL;
    CAW_INFO_TRACE("CVSwitchSTP::RootSelection");
    root = NULL;
    std::list<CVSwitchSTPPort*> portlist;
    GetEnablePortList(portlist);
    for (CVSwitchSTPPort* p : portlist)
    {
        if (IsDesignatedPort(p)
            || p->designated_root >= bridge_id) {
            continue;
        }
        if (root && !SupersedesRoot(root, p)) {
            continue;
        }
        root = p;
    }
    root_port = root;
    if (!root) {
        designated_root = bridge_id;
        root_path_cost = 0;
    } else {
        designated_root = root->designated_root;
        root_path_cost = root->designated_cost + root->path_cost;
    }

    if (root_port)
    {
        CAW_INFO_TRACE("CVSwitchSTP::RootSelection root_port="<< root_port->port_id);
    }
}

void
CVSwitchSTP::DesignatedPortSelection()
{
    CAW_INFO_TRACE("CVSwitchSTP::DesignatedPortSelection");
    std::list<CVSwitchSTPPort*> portlist;
    GetEnablePortList(portlist);
    for (CVSwitchSTPPort* p : portlist)
    {
        if (IsDesignatedPort(p)
            || p->designated_root != designated_root
            || root_path_cost < p->designated_cost
            || (root_path_cost == p->designated_cost
                && (bridge_id < p->designated_bridge
                    || (bridge_id == p->designated_bridge
                        && p->port_id <= p->designated_port))))
        {
            BecomeDesignatedPort(p);
        }
    }
}

void
CVSwitchSTP::BecomeDesignatedPort(CVSwitchSTPPort *p)
{
    CVSwitchSTP *stp = p->stp;
    p->designated_root = stp->designated_root;
    p->designated_cost = stp->root_path_cost;
    p->designated_bridge = stp->bridge_id;
    p->designated_port = p->port_id;
}

void
CVSwitchSTP::PortStateSelection()
{
    CVSwitchSTPPort *p;

    std::list<CVSwitchSTPPort*> portlist;
    GetEnablePortList(portlist);
    for (CVSwitchSTPPort* p : portlist)
    {
        if (p == root_port) {
            p->config_pending = false;
            p->topology_change_ack = false;
            MakeForwarding(p);
        } else if (IsDesignatedPort(p)) {
            StopTimer(&p->message_age_timer);
            MakeForwarding(p);
        } else {
            p->config_pending = false;
            p->topology_change_ack = false;
            MakeBlocking(p);
        }
    }
}

void
CVSwitchSTP::MakeForwarding(CVSwitchSTPPort *p)
{
    if (p->state == STP_BLOCKING) {
        SetPortState(p, STP_LISTENING);
        StartTimer(&p->forward_delay_timer, 0);
    }
}

void
CVSwitchSTP::MakeBlocking(CVSwitchSTPPort *p)
{
    if (!(p->state & (STP_DISABLED | STP_BLOCKING))) {
        if (p->state & (STP_FORWARDING | STP_LEARNING)) {
            if (p->change_detection_enabled) {
                p->stp->TopologyChangeDetection();
            }
        }
        SetPortState(p, STP_BLOCKING);
        StopTimer(&p->forward_delay_timer);
    }
}

void
CVSwitchSTP::SetPortState(CVSwitchSTPPort *p, enum stp_state state)
{
    if (state != p->state && !p->state_changed) {
        p->state_changed = true;
        if (p < p->stp->first_changed_port) {
            p->stp->first_changed_port = p;
        }
        //seq_change(connectivity_seq_get());
    }
    p->state = state;
}

void
CVSwitchSTP::TopologyChangeDetection()
{
    if (IsRootBridge()) {
        topology_change = true;
        StartTimer(&topology_change_timer, 0);
    } else if (!topology_change_detected) {
        TransmitTcn();
        StartTimer(&tcn_timer, 0);
    }
    fdb_needs_flush = true;
    topology_change_detected = true;
}

void
CVSwitchSTP::TopologyChangeAcknowledged()
{
    topology_change_detected = false;
    StopTimer(&tcn_timer);
}

void
CVSwitchSTP::AcknowledgeTopologyChange(CVSwitchSTPPort *p)
{
    p->topology_change_ack = true;
    TransmitConfig(p);
}

void
CVSwitchSTP::ReceivedConfigBpdu(CVSwitchSTPPort *p,
                         const struct stp_config_bpdu *config)
{
    if (ntohs(config->message_age) >= ntohs(config->max_age)) {
        return;
    }
    CAW_INFO_TRACE("CVSwitchSTP::ReceivedConfigBpdu port_id =" << p->port_id);
    if (p->state != STP_DISABLED) {
        bool root = IsRootBridge();
        CAW_INFO_TRACE("CVSwitchSTP::ReceivedConfigBpdu IsRootBridge="<< root<<", port_id = " << p->port_id);
        if (SupersedesPortInfo(p, config)) {
            RecordConfigInformation(p, config);
            ConfigurationUpdate();
            PortStateSelection();
            if (!IsRootBridge() && root) {
                StopTimer(&hello_timer);
                if (topology_change_detected) {
                    StopTimer(&topology_change_timer);
                    TransmitTcn();
                    StartTimer(&tcn_timer, 0);
                }
            }
            if (p == root_port) {
                RecordConfigTimeoutValues(config);
                ConfigBpduGeneration();
                if (config->flags & STP_CONFIG_TOPOLOGY_CHANGE_ACK) {
                    TopologyChangeAcknowledged();
                }
                if (config->flags & STP_CONFIG_TOPOLOGY_CHANGE) {
                    fdb_needs_flush = true;
                }
            }
        } else if (IsDesignatedPort(p)) {
            TransmitConfig(p);
        }
    }
}

void
CVSwitchSTP::ReceivedTcnBpdu(CVSwitchSTPPort *p)
{
    CAW_INFO_TRACE("CVSwitchSTP::ReceivedTcnBpdu port_id =" << p->port_id);
    if (p->state != STP_DISABLED) {
        if (IsDesignatedPort(p)) {
            CAW_INFO_TRACE("CVSwitchSTP::ReceivedTcnBpdu IsDesignatedPort port_id =" << p->port_id);
            TopologyChangeDetection();
            AcknowledgeTopologyChange(p);
        }
    }
}

void
CVSwitchSTP::HelloTimerExpiry()
{
    ConfigBpduGeneration();
    StartTimer(&hello_timer, 0);
}

void
CVSwitchSTP::MessageAgeTimerExpiry(CVSwitchSTPPort *p)
{
    CVSwitchSTP *stp = p->stp;
    bool root = stp->IsRootBridge();

    BecomeDesignatedPort(p);
    ConfigurationUpdate();
    PortStateSelection();
    if (IsRootBridge() && !root) {
        max_age = bridge_max_age;
        hello_time = bridge_hello_time;
        forward_delay = bridge_forward_delay;
        TopologyChangeDetection();
        StopTimer(&tcn_timer);
        ConfigBpduGeneration();
        StartTimer(&hello_timer, 0);
    }
}

bool
CVSwitchSTP::IsDesignatedForSomePort()
{

    std::list<CVSwitchSTPPort*> portlist;
    GetEnablePortList(portlist);
    for (CVSwitchSTPPort* p : portlist)
    {
        if (p->designated_bridge == bridge_id) {
            return true;
        }
    }
    return false;
}

void
CVSwitchSTP::ForwardDelayTimerExpiry(CVSwitchSTPPort *p)
{
    if (p->state == STP_LISTENING) {
        SetPortState(p, STP_LEARNING);
        StartTimer(&p->forward_delay_timer, 0);
    } else if (p->state == STP_LEARNING) {
        SetPortState(p, STP_FORWARDING);
        if (p->stp->IsDesignatedForSomePort()) {
            if (p->change_detection_enabled) {
                p->stp->TopologyChangeDetection();
            }
        }
    }
}

void
CVSwitchSTP::TcnTimerExpiry()
{
    TransmitTcn();
    StartTimer(&tcn_timer, 0);
}

void
CVSwitchSTP::TopologyChangeTimerExpiry()
{
    topology_change_detected = false;
    topology_change = false;
}

void
CVSwitchSTP::HoldTimerExpiry(CVSwitchSTPPort *p)
{
    if (p->config_pending) {
        TransmitConfig(p);
    }
}

void
CVSwitchSTP::InitializePort(CVSwitchSTPPort* p,
    enum stp_state state)
{
    BecomeDesignatedPort(p);

    if (!p->state && state == STP_DISABLED) {
        p->state = state; /* Do not trigger state change when initializing. */
    } else {
        SetPortState(p, state);
    }
    p->topology_change_ack = false;
    p->config_pending = false;
    p->change_detection_enabled = true;
    p->aux = NULL;
    StopTimer(&p->message_age_timer);
    StopTimer(&p->forward_delay_timer);
    StopTimer(&p->hold_timer);
    p->tx_count = p->rx_count = p->error_count = 0;
}

void
CVSwitchSTP::BecomeRootBridge()
{
    max_age = bridge_max_age;
    hello_time = bridge_hello_time;
    forward_delay = bridge_forward_delay;
    TopologyChangeDetection();
    StopTimer(&tcn_timer);
    ConfigBpduGeneration();
    StartTimer(&hello_timer, 0);
}

void
CVSwitchSTP::StopTimer(struct stp_timer *timer)
{
    timer->active = false;
}

bool
CVSwitchSTP::TimerExpired(struct stp_timer *timer, int elapsed, int timeout)
{
    if (timer->active) {
        timer->value += elapsed;
        if (timer->value >= timeout) {
            timer->active = false;
            return true;
        }
    }
    return false;
}

/* Returns the number of whole STP timer ticks in 'ms' milliseconds.  There
 * are 256 STP timer ticks per second. */
static int
ms_to_timer(int ms)
{
    return ms * 0x100 / 1000;
}

/* Returns the number of whole milliseconds in 'timer' STP timer ticks.  There
 * are 256 STP timer ticks per second. */
static int
timer_to_ms(int timer)
{
    return timer * 1000 / 0x100;
}

static int
clamp(int x, int min, int max)
{
    return x < min ? min : x > max ? max : x;
}

void
CVSwitchSTP::UpdateBridgeTimers()
{
    int ht, ma, fd;

    ht = clamp(rq_hello_time, 1000, 10000);
    ma = clamp(rq_max_age, caw_max(2 * (ht + 1000), 6000), 40000);
    fd = clamp(rq_forward_delay, ma / 2 + 1000, 30000);

    bridge_hello_time = ms_to_timer(ht);
    bridge_max_age = ms_to_timer(ma);
    bridge_forward_delay = ms_to_timer(fd);

    if (IsRootBridge()) {
        max_age = bridge_max_age;
        hello_time = bridge_hello_time;
        forward_delay = bridge_forward_delay;
    }
}

void
CVSwitchSTP::SendBPDU(CVSwitchSTPPort *p, const void *bpdu, size_t bpdu_size)
{
    CAW_INFO_TRACE("CVSwitchSTP::SendBPDU");
    STPPacket packet;
    packet.SetPayload((char*)bpdu, bpdu_size);
    DWORD dwLength = packet.GetEncodeSpace();

    CAWMessageBlock msgblock(dwLength);

    CAWByteStreamNetwork os(msgblock);
    CAWResult ret = packet.StreamTo(os);
    if (CAW_FAILED(ret))
    {
        CAW_ASSERTE(0);
        return;
    }
    m_psink->SendSTPPacket(p->port_id,
        msgblock.GetTopLevelReadPtr(),
        msgblock.GetTopLevelLength());
}
bool CVSwitchSTP::IsRootBridge()
{
    bool is_root;
    is_root = bridge_id == designated_root;
    return is_root;
}
void
CVSwitchSTP::StartTimer(struct stp_timer* timer, int value)
{
    timer->value = value;
    timer->active = true;
}
void
CVSwitchSTP::TransmitConfig(CVSwitchSTPPort* p)
{
    bool root = IsRootBridge();
    CAW_INFO_TRACE("CVSwitchSTP::TransmitConfig IsRootBridge="<<root
        <<",port_id="<<p->port_id);
    if (!root && !root_port) {
        return;
    }
    if (p->hold_timer.active) {
        p->config_pending = true;
    }
    else {
        struct stp_config_bpdu config;
        memset(&config, 0, sizeof config);
        config.header.protocol_id = htons(STP_PROTOCOL_ID);
        config.header.protocol_version = STP_PROTOCOL_VERSION;
        config.header.bpdu_type = STP_TYPE_CONFIG;
        config.flags = 0;
        if (p->topology_change_ack) {
            config.flags |= STP_CONFIG_TOPOLOGY_CHANGE_ACK;
        }
        if (topology_change) {
            config.flags |= STP_CONFIG_TOPOLOGY_CHANGE;
        }
        config.root_id = caw_htonll(designated_root);
        config.root_path_cost = htonl(root_path_cost);
        config.bridge_id = caw_htonll(bridge_id);
        config.port_id = htons(p->port_id);
        if (root) {
            config.message_age = htons(0);
        }
        else {
            config.message_age = htons(root_port->message_age_timer.value+ MESSAGE_AGE_INCREMENT);
        }
        config.max_age = htons(max_age);
        config.hello_time = htons(hello_time);
        config.forward_delay = htons(forward_delay);
        if (ntohs(config.message_age) < max_age) {
            p->topology_change_ack = false;
            p->config_pending = false;
            SendBPDU(p, &config, sizeof config);
            StartTimer(&p->hold_timer, 0);
        }
    }
}
bool
CVSwitchSTP::MayReceive(CVSwitchSTPPort* pport)
{
    /* Only drop packets here if both forwarding and learning are
     * disabled.  If just learning is enabled, we need to have
     * OFPP_NORMAL and the learning action have a look at the packet
     * before we can drop it. */
    if (!stp_forward_in_state(PortGetState(pport)) && !stp_learn_in_state(PortGetState(pport)))
    {
        return false;
    }

    return true;
}
