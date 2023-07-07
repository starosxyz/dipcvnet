/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef RADIUS_H
#define RADIUS_H
#include <packet/UDPPacket.h>
#include <packet/RADIUSAttribute.h>


// RADIUS parameters
static const uint16_t RADIUS_MIN_LENGTH = 20;
static const uint16_t MAX_ATTR_VALUE_LENGTH = 253;
static const uint16_t RADIUS_MAX_LENGTH = 4096;

// RADIUS packet types
static const uint8_t RADIUS_CODE_ACCESS_REQUEST = 0x01;
static const uint8_t RADIUS_CODE_ACCESS_ACCEPT = 0x02;
static const uint8_t RADIUS_CODE_ACCESS_REJECT = 0x03;
static const uint8_t RADIUS_CODE_ACCOUNTING_REQUEST = 0x04;
static const uint8_t RADIUS_CODE_ACCOUNTING_RESPONSE = 0x05;
static const uint8_t RADIUS_CODE_ACCESS_CHALLENGE = 0x0b;

#define RADIUS_JSON_CODE "code"
#define RADIUS_JSON_IDENTIFIER "identifier"
#define RADIUS_JSON_LENGTH "length"
#define RADIUS_JSON_AUTH "authenticator"
#define RADIUS_JSON_ATTRS "attributes"

/*
  @类名:RADIUS
  @功能:对UDP数据格式的管理
*/

class CAW_OS_EXPORT RADIUS : public UDPPacket {
public:
    /**
     * Default constructor.
     */
    RADIUS();
    virtual ~RADIUS();

    /**
     * Constructs a RADIUS packet with the given code and identifier.
     *
     * @param code       code
     * @param identifier identifier
     */
    RADIUS(uint8_t code, uint8_t identifier);

    /**
     * Gets the code.
     *
     * @return code
     */
    uint8_t getCode();

    /**
     * Sets the code.
     *
     * @param code code
     */
    void setCode(uint8_t code);

    /**
     * Gets the identifier.
     *
     * @return identifier
     */
    uint8_t getIdentifier();

    /**
     * Sets the identifier.
     *
     * @param identifier identifier
     */
    void setIdentifier(uint8_t identifier);

    /**
     * Gets the authenticator.
     *
     * @return authenticator
     */
    void getAuthenticator(char authenticator[16]);

    /**
     * Sets the authenticator.
     *
     * @param authenticator authenticator
     */
    void setAuthenticator(char authenticator[16]);

    /**
     * Checks if the packet's code field is valid.
     *
     * @return whether the code is valid
     */
    bool isValidCode();

    /**
     * Gets a list of attributes from the RADIUS packet.
     *
     * @param attrType the type field of the required attributes
     * @return List of the attributes that matches the type or an empty list if there is none
     */
    void getAttributeList(uint8_t attrType, std::list<RADIUSAttribute> &attrlist);
    void GetAttributeList2(RADIUSAttribute *&parray, size_t &arraysize);
    void ReturnAttributeList(RADIUSAttribute *&parray);
    /**
     * Gets an attribute from the RADIUS packet.
     *
     * @param attrType the type field of the required attribute
     * @return the first attribute that matches the type or null if does not exist
     */
    RADIUSAttribute getAttribute(uint8_t attrType);

    /**
     * Sets an attribute in the RADIUS packet.
     *
     * @param attrType the type field of the attribute to set
     * @param value    value to be set
     * @return reference to the attribute object
     */
    void setAttribute(RADIUSAttribute &attr);

    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;

    std::string toString();
private:
    uint8_t m_code;
    uint8_t m_identifier;
    uint16_t m_length;
    char m_authenticator[16];
    std::list<RADIUSAttribute> m_attributes;
};


#endif

