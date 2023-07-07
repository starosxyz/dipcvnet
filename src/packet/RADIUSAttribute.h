/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef TPPORT_H
#define TPPORT_H
#include <packet/IPacketParser.h>
#include <json/jsoncpp.h>


// RADIUS attribute types
static const uint8_t RADIUS_ATTR_USERNAME = 1;
static const uint8_t RADIUS_ATTR_NAS_IP = 4;
static const uint8_t RADIUS_ATTR_NAS_PORT = 5;
static const uint8_t RADIUS_ATTR_FRAMED_MTU = 12;
static const uint8_t RADIUS_ATTR_STATE = 24;
static const uint8_t RADIUS_ATTR_VENDOR_SPECIFIC = 26;
static const uint8_t RADIUS_ATTR_CALLING_STATION_ID = 31;
static const uint8_t RADIUS_ATTR_NAS_ID = 32;
static const uint8_t RADIUS_ATTR_ACCT_SESSION_ID = 44;
static const uint8_t RADIUS_ATTR_NAS_PORT_TYPE = 61;
static const uint8_t RADIUS_ATTR_EAP_MESSAGE = 79;
static const uint8_t RADIUS_ATTR_MESSAGE_AUTH = 80;
static const uint8_t RADIUS_ATTR_NAS_PORT_ID = 87;

#define RADIUS_ATTRIBUTE_JSON_LENGTH "length"
#define RADIUS_ATTRIBUTE_JSON_TYPE "type"
#define RADIUS_ATTRIBUTE_JSON_VALUE "value"

/*
  @类名:RADIUSAttribute
  @功能:对Ip数据格式的管理
*/

class CAW_OS_EXPORT RADIUSAttribute : public IPacketParser
{
public:
    RADIUSAttribute();
    RADIUSAttribute(uint8_t type, uint8_t length, char *value);
    RADIUSAttribute(const RADIUSAttribute &right);
    RADIUSAttribute& operator=(const RADIUSAttribute& right);
    bool operator==(const RADIUSAttribute& right) const;
    bool operator!=(const RADIUSAttribute& right) const;

    virtual ~RADIUSAttribute();
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;
    virtual void Dump() const;

    bool isValidType();
    uint8_t getType();
    void setType(uint8_t type);

    uint8_t getLength();
    char *getValue();
    void setValue(uint8_t length,  char *value);
private:
    uint8_t m_type;
    uint8_t m_length;
    char* m_value;
};


#endif

