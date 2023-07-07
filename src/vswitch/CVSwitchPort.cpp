/***********************************************************************
 *   版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include "CVSwitchPort.h"
#include "CVSwitchImp.h"

CVSwitchPort::CVSwitchPort(CVSwitchImp* pvswitch,
	IDIPCVSwitchSink* psink,
	uint32_t id)
	:m_pswitch(pvswitch)
	,m_psink(psink)
	,m_id(id)
	, m_stpport(NULL)
{
	m_stp_state = STP_DISABLED;
}
CVSwitchPort::~CVSwitchPort()
{

}
