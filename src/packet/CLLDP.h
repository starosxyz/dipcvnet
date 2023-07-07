/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef CLLDP_H_
#define CLLDP_H_
#include <packet/Ethernet.h>
#include <wface/CAWACEWrapper.h>
#include <packet/LLDPTLV.h>
using namespace starbase;
using namespace wface;

class CAW_OS_EXPORT CLLDP : public CEthernet
{
public:
    CLLDP();
    virtual ~CLLDP();

    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;

    virtual void Dump() const;
    CLLDP(const CLLDP& right);
    CLLDP& operator=(const CLLDP& right);
    inline LLDPTLV &GetChassisId(){return chassisId;}
    void SetChassisId(const LLDPTLV  &   strchassisId);
    inline  LLDPTLV &GetPortId(){return portId;}
    void SetPortId(const LLDPTLV    &strportId);
    inline LLDPTLV &GetTtl(){return ttl;}
    void SetTtl(const LLDPTLV  &strttl);
    size_t GetLLDPTLVSize();
    void addOptionalTLV(LLDPTLV &lldptlv);
    CAWString GetValueByType(uint8_t &strtype);
    std::list<LLDPTLV> GetTlvList();
void GetTlvList(std::list<LLDPTLV> &list);
protected:
    LLDPTLV  chassisId;
    LLDPTLV  portId;
    LLDPTLV  ttl;
    std::list<LLDPTLV> optionalTLVList;
    uint16_t m_len;
};

#endif//CLLDP_H_