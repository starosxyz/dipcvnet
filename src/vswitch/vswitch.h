/***********************************************************************
 *   版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef VSW_H
#define VSW_H
/* This is an implementation of Spanning Tree Protocol as described in IEEE
 * 802.1D-1998, clauses 8 and 9.  Section numbers refer to this standard.  */

#include <stdbool.h>
#include <stdint.h>

#define STP_PROTOCOL_ID 0x0000
#define STP_PROTOCOL_VERSION 0x00
#define STP_TYPE_CONFIG 0x00
#define STP_TYPE_TCN 0x80

struct stp_bpdu_header {
    uint16_t protocol_id;       /* STP_PROTOCOL_ID. */
    uint8_t protocol_version;   /* STP_PROTOCOL_VERSION. */
    uint8_t bpdu_type;          /* One of STP_TYPE_*. */
};

enum stp_config_bpdu_flags {
    STP_CONFIG_TOPOLOGY_CHANGE_ACK = 0x80,
    STP_CONFIG_TOPOLOGY_CHANGE = 0x01
};
struct stp_config_bpdu {
    struct stp_bpdu_header header; /* Type STP_TYPE_CONFIG. */
    uint8_t flags;                 /* STP_CONFIG_* flags. */
    uint64_t root_id;              /* 8.5.1.1: Bridge believed to be root. */
    uint32_t root_path_cost;       /* 8.5.1.2: Cost of path to root. */
    uint64_t bridge_id;            /* 8.5.1.3: ID of transmitting bridge. */
    uint16_t port_id;              /* 8.5.1.4: Port transmitting the BPDU. */
    uint16_t message_age;          /* 8.5.1.5: Age of BPDU at tx time. */
    uint16_t max_age;              /* 8.5.1.6: Timeout for received data. */
    uint16_t hello_time;           /* 8.5.1.7: Time between BPDU generation. */
    uint16_t forward_delay;        /* 8.5.1.8: State progression delay. */
};

struct stp_tcn_bpdu {
    struct stp_bpdu_header header; /* Type STP_TYPE_TCN. */
};

struct stp_timer {
    bool active;                 /* Timer in use? */
    int value;                   /* Current value of timer, counting up. */
};

/* Bridge and port priorities that should be used by default. */
#define STP_DEFAULT_BRIDGE_PRIORITY 32768
#define STP_DEFAULT_PORT_PRIORITY 128

/* Default time values. */
#define STP_DEFAULT_MAX_AGE    20000
#define STP_DEFAULT_HELLO_TIME 2000
#define STP_DEFAULT_FWD_DELAY  15000

/* Bridge identifier.  Top 16 bits are a priority value (numerically lower
 * values are higher priorities).  Bottom 48 bits are MAC address of bridge. */
typedef uint64_t stp_identifier;


#define STP_ID_FMT "%04" PRIx16 ".%012" PRIx64
#define STP_ID_ARGS(stp_id) \
    (uint16_t)((stp_id) >> 48), \
    (uint64_t)((stp_id) & 0xffffffffffffULL)

#define STP_PORT_ID_FMT "%04" PRIx16

/* Basic STP functionality. */
#define STP_MAX_PORTS 255
#define MESSAGE_AGE_INCREMENT 1

    /* State of an STP port.
     *
     * A port is in exactly one state at any given time, but distinct bits are used
     * for states to allow testing for more than one state with a bit mask.
     *
     * The STP_DISABLED state means that the port is disabled by management.
     * In our implementation, this state means that the port does not
     * participate in the spanning tree, but it still forwards traffic as if
     * it were in the STP_FORWARDING state.  This may be different from
     * other implementations.
     *
     * The following diagram describes the various states and what they are
     * allowed to do in OVS:
     *
     *                     FWD  LRN  TX_BPDU RX_BPDU FWD_BPDU
     *                     ---  ---  ------- ------- --------
     *        Disabled      Y    -      -       -        Y
     *        Blocking      -    -      -       Y        -
     *        Listening     -    -      Y       Y        Y
     *        Learning      -    Y      Y       Y        Y
     *        Forwarding    Y    Y      Y       Y        Y
     *
     *
     * FWD:              the port should forward any incoming non-stp-BPDU
     *                   packets.
     *
     * LRN:              the port should conduct MAC learning on packets received.
     *
     * TX_BPDU/RX_BPDU:  the port could generate/consume bpdus.
     *
     * FWD_BPDU:         the port should should always forward the BPDUS,
     *                   whether they are generated by the port or received
     *                   as incoming packets.
     *
     * Once again, note that the disabled state forwards traffic, which is
     * likely different than the spec would indicate.
     */
enum stp_state {
    STP_DISABLED = 1 << 0,       /* 8.4.5: See note above. */
    STP_LISTENING = 1 << 1,      /* 8.4.2: Not learning or relaying frames. */
    STP_LEARNING = 1 << 2,       /* 8.4.3: Learning but not relaying frames. */
    STP_FORWARDING = 1 << 3,     /* 8.4.4: Learning and relaying frames. */
    STP_BLOCKING = 1 << 4        /* 8.4.1: Initial boot state. */
};
/* Role of an STP port. */
enum stp_role {
    STP_ROLE_ROOT,               /* Path to root bridge. */
    STP_ROLE_DESIGNATED,         /* Path to LAN segments. */
    STP_ROLE_ALTERNATE,          /* Backup path to root bridge. */
    STP_ROLE_DISABLED            /* Port does not participate in STP. */
};

#endif