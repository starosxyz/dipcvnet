/***********************************************************************
 *   版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include "CVSwitchSTPPort.h"
#include "CVSwitchSTP.h"
CVSwitchSTPPort::CVSwitchSTPPort(CVSwitchSTP* pstp,
    uint32_t id,
    uint32_t path_cost,
    enum stp_state estate)
    :stp(pstp)
    , port_id(id)
    ,path_cost(path_cost)
    , state(estate)
{
    aux = NULL;
    designated_root = 0;
    designated_cost = 0;
    designated_bridge = 0;
    designated_port = 0;
    topology_change_ack = false;
    config_pending = false;
    change_detection_enabled = false;

    tx_count = 0;
    rx_count = 0;
    error_count = 0;

    state_changed=false;
}
CVSwitchSTPPort::~CVSwitchSTPPort()
{

}
