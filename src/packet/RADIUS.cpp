/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/RADIUS.h>


RADIUS::RADIUS() {
}
RADIUS::~RADIUS() {
}

/**
 * Constructs a RADIUS packet with the given code and identifier.
 *
 * @param code       code
 * @param identifier identifier
 */
RADIUS::RADIUS(uint8_t code, uint8_t identifier) {
    m_code = code;
    m_identifier = identifier;
}

/**
 * Gets the code.
 *
 * @return code
 */
uint8_t RADIUS::getCode() {
    return m_code;
}

/**
 * Sets the code.
 *
 * @param code code
 */
void RADIUS::setCode(uint8_t code) {
    m_code = code;
}

/**
 * Gets the identifier.
 *
 * @return identifier
 */
uint8_t RADIUS::getIdentifier() {
    return m_identifier;
}

/**
 * Sets the identifier.
 *
 * @param identifier identifier
 */
void RADIUS::setIdentifier(uint8_t identifier) {
    m_identifier = identifier;
}

/**
 * Gets the authenticator.
 *
 * @return authenticator
 */
void RADIUS::getAuthenticator(char authenticator[16]) {
    ::memcpy(authenticator, m_authenticator, 16);
}

/**
 * Sets the authenticator.
 *
 * @param authenticator authenticator
 */
void RADIUS::setAuthenticator(char authenticator[16]) {
    ::memcpy(m_authenticator, authenticator, 16);
}


/**
 * Checks if the packet's code field is valid.
 *
 * @return whether the code is valid
 */
bool RADIUS::isValidCode() {
    return m_code == RADIUS_CODE_ACCESS_REQUEST ||
            m_code == RADIUS_CODE_ACCESS_ACCEPT ||
            m_code == RADIUS_CODE_ACCESS_REJECT ||
            m_code == RADIUS_CODE_ACCOUNTING_REQUEST ||
            m_code == RADIUS_CODE_ACCOUNTING_RESPONSE ||
            m_code == RADIUS_CODE_ACCESS_CHALLENGE;
}

/**
 * Gets a list of attributes from the RADIUS packet.
 *
 * @param attrType the type field of the required attributes
 * @return List of the attributes that matches the type or an empty list if there is none
 */
void RADIUS::getAttributeList(uint8_t attrType, std::list<RADIUSAttribute> &listattr)
{
    for (RADIUSAttribute &attr : m_attributes) {
        listattr.push_back(attr);
    }
}

void RADIUS::GetAttributeList2(RADIUSAttribute *&parray, size_t &arraysize)
{
    arraysize = m_attributes.size();
    parray = new RADIUSAttribute[arraysize];
    size_t index = 0;
    for (RADIUSAttribute &iter : m_attributes) 
    {
        parray[index] = iter;
        index++;
    } 
}

void RADIUS::ReturnAttributeList(RADIUSAttribute *&parray)
{
    if (parray != NULL)
    {
        delete[] parray;
    }
}

/**
 * Gets an attribute from the RADIUS packet.
 *
 * @param attrType the type field of the required attribute
 * @return the first attribute that matches the type or null if does not exist
 */
RADIUSAttribute RADIUS::getAttribute(uint8_t attrType) {
    RADIUSAttribute nullarr;

    for (RADIUSAttribute &attr : m_attributes) {
        if (attr.getType() == attrType) {
            return attr;
        }
    }
    return nullarr;
}

/**
 * Sets an attribute in the RADIUS packet.
 *
 * @param attrType the type field of the attribute to set
 * @param value    value to be set
 * @return reference to the attribute object
 */
void RADIUS::setAttribute(RADIUSAttribute &attr) {
    m_attributes.push_back(attr);
}

CAWResult RADIUS::StreamFrom(CAWByteStreamNetwork& is)
{
    UDPPacket::StreamFrom(is);
    is>>m_code;
    is>>m_identifier;
    is>>m_length;
    is.Read(m_authenticator,16);
    int remainingLength = m_length - RADIUS_MIN_LENGTH;
    while(remainingLength > 0)
    {
        RADIUSAttribute attr;
        attr.StreamFrom(is);
        m_attributes.push_back(attr);
        remainingLength -= attr.GetStreamLen();
    }
    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return CAW_OK;
}

CAWResult RADIUS::StreamTo(CAWByteStreamNetwork& os)
{
    UDPPacket::StreamTo(os);

    os<<m_code;
    os<<m_identifier;
    os<<m_length;
    os.Write(m_authenticator,16);
    for (RADIUSAttribute &attr:m_attributes) {
        attr.StreamTo(os);
    }
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return CAW_OK;
}
CAWResult RADIUS::FromJson(Json::Value& jsonin)
{
    UDPPacket::FromJson(jsonin);
    ValueFromJson(jsonin, RADIUS_JSON_CODE, m_code);
    ValueFromJson(jsonin, RADIUS_JSON_IDENTIFIER, m_identifier);
    ValueFromJson(jsonin, RADIUS_JSON_LENGTH, m_length);
    ByteArrayFromJson(jsonin, RADIUS_JSON_AUTH, m_authenticator, 16);

    if (!jsonin[RADIUS_JSON_ATTRS].isNull())
    {
        if (jsonin[RADIUS_JSON_ATTRS].isArray())
        {
            Json::Value arrayjson=jsonin[RADIUS_JSON_ATTRS];
            for(uint32_t i=0;i<arrayjson.size();i++)
            {
                Json::Value onejson = arrayjson[i];
                RADIUSAttribute attr;
                attr.FromJson(onejson);
                m_attributes.push_back(attr);
            }
        }
    }
    
    return CAW_OK;

}

CAWResult RADIUS::ToJson(Json::Value& jsonout)
{
    UDPPacket::ToJson(jsonout);
    jsonout[RADIUS_JSON_CODE]=ValueToJson(m_code);
    jsonout[RADIUS_JSON_IDENTIFIER]=ValueToJson(m_identifier);
    jsonout[RADIUS_JSON_LENGTH]=ValueToJson(m_length);
    jsonout[RADIUS_JSON_AUTH]=ByteArrayToJson(m_authenticator, 16);

    Json::Value arrayjson;

    for(RADIUSAttribute &attr:m_attributes)
    {
        Json::Value onejson;
        attr.ToJson(onejson);
        arrayjson.append(onejson);
    }

    
    jsonout[RADIUS_JSON_ATTRS]=arrayjson;

    return CAW_OK;

}

uint16_t RADIUS::GetStreamLen() const
{
    size_t size = 20+UDPPacket::GetStreamLen();
    for(const RADIUSAttribute &attr:m_attributes)
    {
        size+=attr.GetStreamLen();
    }
    return size;
}

uint16_t RADIUS::GetEncodeSpace()   const
{
    size_t size = 20+UDPPacket::GetStreamLen();
    for(const RADIUSAttribute &attr:m_attributes)
    {
        size+=attr.GetStreamLen();
    }
    return size;
}


