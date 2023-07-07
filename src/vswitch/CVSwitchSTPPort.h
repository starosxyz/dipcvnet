/***********************************************************************
 *   版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef CVSWITCHSTPPORT_H
#define CVSWITCHSTPPORT_H
#include <starbase/CAWACEInclude.h>
#include "IDIPCVSwitch.h"
#include "CVSwitchSTP.h"

class CVSwitchSTPPort
{
public:
	CVSwitchSTPPort(CVSwitchSTP *pstp,
        uint32_t id,
        uint32_t path_cost,
        enum stp_state estate);
	~CVSwitchSTPPort();
public:
    CVSwitchSTP* stp;
    CAWString port_name;                /* Human-readable name for log messages. */
    void* aux;                      /* Auxiliary data the user may retrieve. */
    int port_id;                    /* 8.5.5.1: Unique port identifier. */
    enum stp_state state;           /* 8.5.5.2: Current state. */
    int path_cost;                  /* 8.5.5.3: Cost of tx/rx on this port. */
    stp_identifier designated_root; /* 8.5.5.4. */
    int designated_cost;            /* 8.5.5.5: Path cost to root on port. */
    stp_identifier designated_bridge; /* 8.5.5.6. */
    int designated_port;            /* 8.5.5.7: Port to send config msgs on. */
    bool topology_change_ack;       /* 8.5.5.8: Flag for next config BPDU. */
    bool config_pending;            /* 8.5.5.9: Send BPDU when hold expires? */
    bool change_detection_enabled;  /* 8.5.5.10: Detect topology changes? */

    struct stp_timer message_age_timer; /* 8.5.6.1: Age of received info. */
    struct stp_timer forward_delay_timer; /* 8.5.6.2: State change timer. */
    struct stp_timer hold_timer;        /* 8.5.6.3: BPDU rate limit timer. */

    int tx_count;                   /* Number of BPDUs transmitted. */
    int rx_count;                   /* Number of valid BPDUs received. */
    int error_count;                /* Number of bad BPDUs received. */

    bool state_changed;
};

#endif//CVSWITCHSTPPORT_H