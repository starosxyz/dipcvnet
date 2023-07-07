/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/LLDPTLV.h>


LLDPTLV::LLDPTLV()
              :m_type(0)
              ,m_length(0)
{
    ::memset(m_value,0,255);
}
LLDPTLV::LLDPTLV(uint8_t type, uint16_t length)
                  :m_type(0)
                  ,m_length(0)

{
    m_type = type;
    m_length= length;
    ::memset(m_value,0,255);
}

LLDPTLV::~LLDPTLV()
{

}
LLDPTLV::LLDPTLV(const LLDPTLV &right)
{
    m_type=right.m_type;
    m_length = right.m_length;
    ::memcpy(m_value,right.m_value,m_length);
}

LLDPTLV &LLDPTLV::operator=(const LLDPTLV &right)
{
    m_type=right.m_type;
    m_length = right.m_length;
    ::memcpy(m_value,right.m_value,m_length); 
    return *this;
}

void LLDPTLV::SetValue(uint16_t length, char* pdata)
{
    if(length <= m_length)
    {        
        m_length=length;
        ::memcpy(m_value,pdata, m_length);
    }
}
void LLDPTLV::GetValue(uint16_t &length, uint8_t pdata[255])
{
    ::memcpy(pdata,m_value,255);
    length = m_length;
}
uint8_t LLDPTLV::GetLldpType()
{
    return m_type;
    
}
uint16_t LLDPTLV::GetLldpLength()
{
    return m_length;
}
CAWString LLDPTLV::GetMvalue()
{
    return CAWString((char*)m_value);
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
* 2019/12/18  V1.0    夏章辉     创建
************************************************************************/
CAWResult LLDPTLV::StreamFrom(CAWByteStreamNetwork& is)
{
    //读取每个tlv
    uint16_t typeLength=0;
    is>>typeLength;
	m_type = (uint8_t) (typeLength >> 9 & 0x7f);
	m_length = (uint16_t) (typeLength & 0x1ff);
    // std::cout << "lldp type = " << (int)m_type << std::endl;
    // std::cout << "lldp length = " << (int)m_length << std::endl;
    //读之前
   // size_t remainbefor = is.GetChainedLength();
   // std::cout << "remainbefor =" << remainbefor << std::endl;
    if(m_length > 0)
    {
        is.Read(m_value,m_length);
         //读之后
   // size_t remainafter = is.GetChainedLength();
   // std::cout << "remainafter =" << remainafter << std::endl;
   // std::cout << "m_lengthafter =" << m_length << std::endl;
    }
    // std::cout << "value = " << m_value << std::endl;
     bool bGood = is.IsGood();
   //  std::cout << "bGood = " << bGood << std::endl;
   //  std::cout << "********************Next"<< std::endl;
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
CAWResult LLDPTLV::StreamTo(CAWByteStreamNetwork& os)
{
    //写入每个tlv
	uint16_t scratch = (uint16_t) ((0x7f & m_type) << 9 | 0x1ff & m_length);
	os<<scratch;
	os.Write(m_value,m_length);
    
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return CAW_OK;
 }
CAWResult LLDPTLV::FromJson(Json::Value& jsonin)
{
    return CAW_OK;
}
CAWResult LLDPTLV::ToJson(Json::Value& jsonout)
{
    return CAW_OK;
}
uint16_t LLDPTLV::GetStreamLen() const
{ 
    return 2 + m_length;
}
uint16_t LLDPTLV::GetEncodeSpace()   const
{
    return 2 + m_length;
}

void LLDPTLV::Dump() const
{
    CAW_INFO_TRACE("LLDPTLV::Dump()");
   /* CAW_INFO_TRACE("tlvType=" << m_type
                    << "tlvlength=" << m_length 
                    << "tlvvalue=" << m_value); */                    
}





