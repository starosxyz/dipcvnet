/***********************************************************************
 *   版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef CVSWITCHPORT_H
#define CVSWITCHPORT_H
#include <starbase/CAWACEInclude.h>
#include "IDIPCVSwitch.h"
#include "CVSwitchSTPPort.h"

    class CVSwitchImp;
class CVSwitchPort
{
public:
    CVSwitchPort(CVSwitchImp *pvswitch, 
        IDIPCVSwitchSink *psink,
        uint32_t id);
	virtual ~CVSwitchPort();
    inline IDIPCVSwitchSink* GetSink() { return m_psink; }
    inline uint32_t  GetId() { return m_id; }
    inline void SetSTPPort(CVSwitchSTPPort* pport) { m_stpport = pport; }
    inline CVSwitchSTPPort* GetSTPPort() { return m_stpport; }
    enum stp_state GetSTPState() { return m_stp_state; }
    void SetSTPState(enum stp_state stp_state) { m_stp_state = stp_state; }
public:
    CVSwitchImp* m_pswitch;
    IDIPCVSwitchSink* m_psink;
    uint32_t m_id;
    enum stp_state m_stp_state;   /* Always STP_DISABLED if STP not in use. */
    CVSwitchSTPPort* m_stpport;
};

#endif//CVSWITCHSTPPORT_H