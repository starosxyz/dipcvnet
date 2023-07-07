/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/RADIUSAttribute.h>


RADIUSAttribute::RADIUSAttribute() 
    :m_type(0)
    ,m_length(0)
    ,m_value(NULL)
{
}

RADIUSAttribute::~RADIUSAttribute()
{
    if (m_value)
    {
        delete[] m_value;
    }
}

RADIUSAttribute::RADIUSAttribute(uint8_t type, uint8_t length, char *value)
{
    m_type = type;
    m_length = length;
    if (m_value)
    {
        delete[] m_value;
        m_value=NULL;
    }
    m_value = new char[length];
    ::memcpy(m_value, value, length);
}

RADIUSAttribute::RADIUSAttribute(const RADIUSAttribute &right)
{
    m_type = right.m_type;
    m_length = right.m_length;
    if (m_value)
    {
        delete[] m_value;
        m_value=NULL;
    }
    m_value = new char[m_length];
    ::memcpy(m_value, right.m_value, m_length); 
}
RADIUSAttribute& RADIUSAttribute::operator=(const RADIUSAttribute& right)
{
    if (&right != this)
    {
        m_type = right.m_type;
        m_length = right.m_length;
        if (m_value)
        {
            delete[] m_value;
            m_value=NULL;
        }
        m_value = new char[m_length];
        ::memcpy(m_value, right.m_value, m_length); 
    }
    return *this;

}
bool RADIUSAttribute::operator==(const RADIUSAttribute& right) const
{
    return ((m_type == right.m_type) && 
        (m_length == right.m_length) && 
        (memcmp(m_value, right.m_value, m_length) ==0));
}

bool RADIUSAttribute::operator!=(const RADIUSAttribute& right) const
{
    return !(*this == right);
}

/**
 * Checks if the attribute type is valid.
 *
 * @return whether the type is valid or not
 */
bool RADIUSAttribute::isValidType() {
    return m_type== RADIUS_ATTR_USERNAME ||
            m_type == RADIUS_ATTR_NAS_IP ||
            m_type == RADIUS_ATTR_NAS_PORT ||
            m_type == RADIUS_ATTR_VENDOR_SPECIFIC ||
            m_type == RADIUS_ATTR_CALLING_STATION_ID ||
            m_type == RADIUS_ATTR_NAS_ID ||
            m_type == RADIUS_ATTR_ACCT_SESSION_ID ||
            m_type == RADIUS_ATTR_NAS_PORT_TYPE ||
            m_type == RADIUS_ATTR_EAP_MESSAGE ||
            m_type == RADIUS_ATTR_MESSAGE_AUTH ||
            m_type == RADIUS_ATTR_NAS_PORT_ID;
}

/**
 * Gets the attribute type.
 *
 * @return the type
 */
uint8_t RADIUSAttribute::getType() {
    return m_type;
}

/**
 * Sets the attribute type.
 *
 * @param type the code to set
 * @return this
 */
void RADIUSAttribute::setType(uint8_t type) {
    m_type = type;
}

/**
 * Gets the attribute length.
 *
 * @return the length
 */
uint8_t RADIUSAttribute::getLength() {
    return m_length;
}

/**
 * Gets the attribute value.
 *
 * @return the value
 */
char *RADIUSAttribute::getValue() {
    return m_value;
}

/**
 * Sets the attribute value.
 *
 * @param value the data to set
 * @return this
 */
void RADIUSAttribute::setValue(uint8_t length, char *value) {
    if (m_value)
    {
        delete[] m_value;
        m_value=NULL;
    }
    m_value = new char[length];
    ::memcpy(m_value, value, length);
}


/**********************************************************************
* 函数名称：StreamFrom
* 功能描述：读取流
* 访问的表：无
* 修改的表：无
* 输入参数：CAWByteStreamNetwork& is：输入流对象
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/6/29  V1.0    夏章辉     创建
************************************************************************/
CAWResult RADIUSAttribute::StreamFrom(CAWByteStreamNetwork& is)
{
    is>>m_type;
    is>>m_length;
    if (m_value)
    {
        delete[] m_value;
        m_value=NULL;
    }
    m_value = new char[m_length-2];
    if (m_value)
    {
        is.Read(m_value, m_length-2);
    }
    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return CAW_OK;
}

/**********************************************************************
* 函数名称：StreamTo
* 功能描述：读取流
* 访问的表：无
* 修改的表：无
* 输入参数：CAWByteStreamNetwork& os：输出流对象
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/6/29  V1.0    夏章辉     创建
************************************************************************/
CAWResult RADIUSAttribute::StreamTo(CAWByteStreamNetwork& os)
{
    os<<m_type;
    os<<m_length;
    if (m_value)
    {
        os.Write(m_value, m_length-2);
    }
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return CAW_OK;
}

CAWResult RADIUSAttribute::FromJson(Json::Value& jsonin)
{
    //以json形式读取数据
    ValueFromJson(jsonin, RADIUS_ATTRIBUTE_JSON_TYPE, m_type);
    ValueFromJson(jsonin, RADIUS_ATTRIBUTE_JSON_LENGTH, m_length);
    ByteArrayFromJson(jsonin, RADIUS_ATTRIBUTE_JSON_VALUE, m_value, m_length-2);
    return CAW_OK;
}

CAWResult RADIUSAttribute::ToJson(Json::Value& jsonout)
{
    //以json形式写出数据
    jsonout[RADIUS_ATTRIBUTE_JSON_TYPE]=ValueToJson(m_type);
    jsonout[RADIUS_ATTRIBUTE_JSON_LENGTH]=ValueToJson(m_length);
    jsonout[RADIUS_ATTRIBUTE_JSON_VALUE]=ByteArrayToJson(m_value, m_length-2);

    return CAW_OK;
}
/**********************************************************************
* 函数名称：GetStreamLen
* 功能描述：获取流长度
* 访问的表：无
* 修改的表：无
* 输入参数：无
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/
uint16_t RADIUSAttribute::GetStreamLen() const
{
    return m_length;
}
/**********************************************************************
* 函数名称：GetEncodeSpace
* 功能描述：获取编码空间
* 访问的表：无
* 修改的表：无
* 输入参数：无
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/
uint16_t RADIUSAttribute::GetEncodeSpace()   const
{
    return m_length;
}

void RADIUSAttribute::Dump() const
{
    //std::string strdump = toString();
    //printf("%s\n",strdump.c_str());
}



