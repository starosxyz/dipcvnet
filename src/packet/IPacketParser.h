/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef IOFP_MSG_PARSER_H
#define IOFP_MSG_PARSER_H
#include <starbase/CAWACEInclude.h>
#include <wface/CAWACEWrapper.h>
#include <json/jsoncpp.h>
using namespace starbase;
using namespace wface;

class CAW_OS_EXPORT IPacketParser: public CAWReferenceControlSingleThread 
{
public:
    IPacketParser();
    virtual ~IPacketParser(){};
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is) = 0;
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os) = 0;
    virtual CAWResult FromJson(Json::Value& jsonin) =0;
    virtual CAWResult ToJson(Json::Value& jsonout) = 0;
    virtual uint16_t GetStreamLen() const = 0;
    virtual uint16_t GetEncodeSpace()   const = 0;
    virtual void Dump() const = 0;
    Json::Value ValueToJson(uint8_t value);
    Json::Value ValueToJson(uint16_t value);
    Json::Value ValueToJson(uint32_t value);
    Json::Value ValueToJson(uint64_t value);
    Json::Value BufferToJson(uint8_t *buffer, size_t buffersize);
    Json::Value ByteArrayToJson(char *byte, size_t bytesize);
    Json::Value ValueToJson(const CAWString &value);
    Json::Value ValueToJson(bool boolvalue);
    Json::Value MacAddressToJson(uint8_t mac[6]);
    Json::Value IPv6ToJson(uint8_t ipv6[128]);
    Json::Value IPv4ToJson(uint32_t ipv4addr);

    void ValueFromJson(Json::Value &jsonvalue, const CAWString &name, uint8_t &outvalue);
    void ValueFromJson(Json::Value &jsonvalue, const CAWString &name, uint16_t &outvalue);
    void ValueFromJson(Json::Value &jsonvalue, const CAWString &name, uint32_t &outvalue);
    void ValueFromJson(Json::Value &jsonvalue, const CAWString &name, uint64_t &outvalue);
    void ValueFromJson(Json::Value &jsonvalue, const CAWString &name, CAWString &outvalue);
    void ValueFromJson(Json::Value &jsonvalue, const CAWString &name, bool &output);

    void IPv4FromJson(Json::Value &jsonvalue, const CAWString &name, uint32_t &ipv4output);
    void IPv6FromJson(Json::Value &jsonvalue, const CAWString &name, uint8_t ipv6output[128]);
    void MacAddressFromJson(Json::Value &jsonvalue, const CAWString &name, uint8_t macaddr[6]);
    void BufferFromJson(Json::Value &jsonvalue, const CAWString &name, uint8_t *buffer, size_t buffersize);
    void ByteArrayFromJson(Json::Value &jsonvalue, const CAWString &name, char *buffer, size_t buffersize);
    static void ValueFromJson_S(Json::Value &jsonvalue, const CAWString &name, uint16_t &outvalue);
    static void ValueFromJson_S(Json::Value &jsonvalue, const CAWString &name, uint8_t &outvalue);
};

#endif//IOFP_MSG_PARSER_H



