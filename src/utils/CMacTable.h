/***********************************************************************
    Copyright (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef CMACTABLE_H
#define CMACTABLE_H
#include <starbase/CAWACEInclude.h>
#include <wface/CAWACEWrapper.h>
#define MACTABLE_MAC_LEN 6
using namespace starbase;
using namespace wface;

class CAW_OS_EXPORT CMacTable
{
public:
    CMacTable(uint64_t dpid,
            uint32_t port,
            char mac[MACTABLE_MAC_LEN],
            uint16_t vlanid);
    CMacTable();
    ~CMacTable();
    CMacTable(const CMacTable& right);
    
    CMacTable& operator=(const CMacTable& right);
    
    bool operator==(const CMacTable& right) const;
    bool operator!=(const CMacTable& right);

    void Print();
    void GetMac(char mac[MACTABLE_MAC_LEN]);
public:
    long m_currenttime;
    char m_mac[MACTABLE_MAC_LEN];
    uint16_t m_vlanid;
    uint64_t m_dpid;
    uint32_t m_port;
};

#endif//CMACTABLE_H