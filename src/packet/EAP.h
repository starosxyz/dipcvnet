/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _EAP_H
#define _EAP_H
#include <packet/IPacketParser.h>


static const uint32_t EAP_HEADER_LENGTH = 4;

static const uint16_t EAP_MIN_LEN = 0x4;
static const uint16_t EAP_HDR_LEN_REQ_RESP = 5;
static const uint16_t EAP_HDR_LEN_SUC_FAIL = 4;

// EAP Code
static const uint8_t EAP_REQUEST  = 0x1;
static const uint8_t EAP_RESPONSE = 0x2;
static const uint8_t EAP_SUCCESS  = 0x3;
static const uint8_t EAP_FAILURE  = 0x4;

// EAP Attribute Type
static const uint8_t EAP_ATTR_IDENTITY = 0x1;
static const uint8_t EAP_ATTR_NOTIFICATION = 0x2;
static const uint8_t EAP_ATTR_NAK = 0x3;
static const uint8_t EAP_ATTR_MD5 = 0x4;
static const uint8_t EAP_ATTR_OTP = 0x5;
static const uint8_t EAP_ATTR_GTC = 0x6;
static const uint8_t EAP_ATTR_TLS = 0xd;

#define EAP_JSON_CODE "code"
#define EAP_JSON_IDENTIFIER "identifier"
#define EAP_JSON_LENGTH "length"
#define EAP_JSON_TYPE "type"
#define EAP_JSON_DATA "data"


/*
  @类名:EAP
  @功能:对EAP协议的管理
*/

class CAW_OS_EXPORT EAP : public IPacketParser {
    /**
     * Default EAP constructor that sets the EAP code to 0.
     */
    EAP();
    virtual ~EAP();
    EAP(uint8_t code, uint8_t identifier, uint8_t type, char* data, size_t datasize);

    uint8_t getCode();

    void setCode(uint8_t code);

    /**
     * Gets the EAP identifier.
     *
     * @return EAP identifier
     */
    uint8_t getIdentifier();

    /**
     * Sets the EAP identifier.
     *
     * @param identifier EAP identifier
     * @return this
     */
    void setIdentifier(uint8_t identifier);

    /**
     * Gets the get packet length.
     *
     * @return packet length
     */
    uint16_t getLength();

    /**
     * Sets the packet length.
     *
     * @param length packet length
     * @return this
     */
    void setLength(uint16_t length);

    /**
     * Gets the data type.
     *
     * @return data type
     */
    uint8_t getDataType();

    /**
     * Sets the data type.
     *
     * @param type data type
     * @return this
     */
    void setDataType(uint8_t type);

    /**
     * Gets the EAP data.
     *
     * @return EAP data
     */
    char* getData();

    void setData(char *data, size_t datasize);

    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const ;
    virtual void Dump() const;
    EAP(const EAP &right);
    EAP& operator=(const EAP& right);
    bool operator==(const EAP& right) const;
    bool operator!=(const EAP& right) const;

private:
    uint8_t m_code;
    uint8_t m_identifier;
    uint16_t m_length;
    uint8_t m_type;
    char* m_data;
    size_t m_datasize;
};


#endif//
