/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _LLDPTLV_H
#define _LLDPTLV_H
#include <packet/IPacketParser.h>


class CAW_OS_EXPORT LLDPTLV : public IPacketParser
{
public:
	LLDPTLV();
	LLDPTLV(uint8_t type, uint16_t length);
	 ~LLDPTLV();
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;
    virtual void Dump() const;
    void SetValue(uint16_t length, char* pdata);
    void GetValue(uint16_t &length, uint8_t pdata[255]);
    uint8_t GetLldpType();
    uint16_t GetLldpLength();
    CAWString GetMvalue();
    LLDPTLV& operator=(const LLDPTLV &right);
    LLDPTLV(const LLDPTLV &right);
protected:
	uint8_t m_type;
	uint16_t m_length;
	uint8_t m_value[255];
};


#endif //_LLDPTLV_H