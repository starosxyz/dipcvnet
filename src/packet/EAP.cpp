/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/EAP.h>


/**
 * Default EAP constructor that sets the EAP code to 0.
 */
EAP::EAP() {
    m_code=0;
    m_identifier=0;
    m_length = 0;
    m_type =0;
    m_data = NULL;
    m_datasize=0;
}
EAP::~EAP()
{
    if (m_data)
    {
        delete[] m_data;
    }
}

/**
 * EAP constructor that initially sets all fields.
 *
 * @param code EAP code
 * @param identifier EAP identifier
 * @param type packet type
 * @param data EAP data
 */
EAP::EAP(uint8_t code, uint8_t identifier, uint8_t type, char *data, size_t datasize) {
    m_code = code;
    m_identifier = identifier;
    if (m_code == EAP_REQUEST || m_code == EAP_RESPONSE) {
        m_length = (uint16_t) (5 + m_datasize);
        m_type = type;
    } else {
        m_length = (uint16_t) (4 + m_datasize);
    }
    m_datasize=datasize;
    if (m_data)
    {
        delete[] m_data;
        m_data=NULL;
    }
    m_data = new char[m_datasize];
    ::memcpy(m_data, data, datasize);
}

/**
 * Gets the EAP code.
 *
 * @return EAP code
 */
uint8_t EAP::getCode() {
    return m_code;
}


/**
 * Sets the EAP code.
 *
 * @param code EAP code
 * @return this
 */
void EAP::setCode(uint8_t code) {
    m_code = code;
}

/**
 * Gets the EAP identifier.
 *
 * @return EAP identifier
 */
uint8_t EAP::getIdentifier() {
    return m_identifier;
}

/**
 * Sets the EAP identifier.
 *
 * @param identifier EAP identifier
 * @return this
 */
void EAP::setIdentifier(uint8_t identifier) {
    m_identifier = identifier;
}

/**
 * Gets the get packet length.
 *
 * @return packet length
 */
uint16_t EAP::getLength() {
    return m_length;
}

/**
 * Sets the packet length.
 *
 * @param length packet length
 * @return this
 */
void EAP::setLength(uint16_t length) {
    m_length = length;
}

/**
 * Gets the data type.
 *
 * @return data type
 */
uint8_t EAP::getDataType() {
    return m_type;
}

/**
 * Sets the data type.
 *
 * @param type data type
 * @return this
 */
void EAP::setDataType(uint8_t type) {
    m_type = type;
}

/**
 * Gets the EAP data.
 *
 * @return EAP data
 */
char *EAP::getData() {
    return m_data;
}

/**
 * Sets the EAP data.
 *
 * @param data EAP data to be set
 * @return this
 */
void EAP::setData(char *data, size_t datasize) {
    m_datasize=datasize;
    if (m_data)
    {
        delete[] m_data;
        m_data=NULL;
    }
    m_data = new char[m_datasize];
    ::memcpy(m_data, data, datasize);
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
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/
CAWResult EAP::StreamFrom(CAWByteStreamNetwork& is)
{
    is>>m_code;
    is>>m_identifier;
    is>>m_length;
    int dataLength;
    if (m_code == EAP_REQUEST || m_code == EAP_RESPONSE) {
        is>>m_type;
        dataLength = m_length - 5;
    } else {
        dataLength = m_length - 4;
    }

    if (dataLength>0)
    {
        if (m_data)
        {
            delete[] m_data;
        }
        m_data = new char[dataLength];
        if (m_data)
        {
            is.Read(m_data, dataLength);
        }
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
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/
CAWResult EAP::StreamTo(CAWByteStreamNetwork& os)
{
    m_length=GetStreamLen();
    os<<m_code;
    os<<m_identifier;
    os<<m_length;
    if (m_code == EAP_REQUEST || m_code == EAP_RESPONSE) {
        os<<m_type;
    }
    if ((m_data != NULL) && (m_datasize != 0))
    {
        os.Write(m_data, m_datasize);
    }
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return CAW_OK;
}
CAWResult EAP::FromJson(Json::Value& jsonin)
{
    //以JSON形式读取数据
    ValueFromJson(jsonin, EAP_JSON_CODE, m_code);
    ValueFromJson(jsonin, EAP_JSON_IDENTIFIER, m_identifier);
    ValueFromJson(jsonin, EAP_JSON_LENGTH, m_length);
    ValueFromJson(jsonin, EAP_JSON_TYPE, m_type);
    ByteArrayFromJson(jsonin, EAP_JSON_DATA, m_data, m_datasize);
    return CAW_OK;
}

CAWResult EAP::ToJson(Json::Value& jsonout)
{
    //以JSON形式写出数据
    jsonout[EAP_JSON_CODE]=ValueToJson(m_code);
    jsonout[EAP_JSON_IDENTIFIER]=ValueToJson(m_identifier);
    jsonout[EAP_JSON_LENGTH]=ValueToJson(m_length);
    jsonout[EAP_JSON_TYPE]=ValueToJson(m_type);
    jsonout[EAP_JSON_DATA]=ByteArrayToJson(m_data, m_datasize);

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
uint16_t EAP::GetStreamLen() const
{
    if (m_code == EAP_REQUEST || m_code == EAP_RESPONSE) {
        return 5+m_datasize;
    } else {
        return 4+m_datasize;
    } 
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
uint16_t EAP::GetEncodeSpace()   const 
{
    if (m_code == EAP_REQUEST || m_code == EAP_RESPONSE) {
        return 5+m_datasize;
    } else {
        return 4+m_datasize;
    } 
}
void EAP::Dump() const
{
}

EAP::EAP(const EAP &right)
{
    m_code=right.m_code;
    m_identifier=right.m_identifier;
    m_length=right.m_length;
    m_type=right.m_type;
    m_datasize = right.m_datasize;
    if (m_data)
    {
        delete[] m_data;
        m_data = NULL;
    }
    m_data = new char[m_datasize];
    ::memcpy(m_data, right.m_data, m_datasize);
}

EAP& EAP::operator=(const EAP& right)
{
    if (&right != this)
    {
        m_code=right.m_code;
        m_identifier=right.m_identifier;
        m_length=right.m_length;
        m_type=right.m_type;
        m_datasize = right.m_datasize;
        if (m_data)
        {
            delete[] m_data;
            m_data = NULL;
        }
        m_data = new char[m_datasize];
        ::memcpy(m_data, right.m_data, m_datasize); 
    }
    return *this;

}

bool EAP::operator==(const EAP& right) const
{
    return ((m_code == right.m_code) && (m_identifier==right.m_identifier)&&
        (m_length==right.m_length)&&
        (m_type==right.m_type)&&
        (m_datasize==right.m_datasize)&&
        (memcmp(m_data, right.m_data, m_datasize)));
}

bool EAP::operator!=(const EAP& right) const
{
    return !(*this == right);
}




