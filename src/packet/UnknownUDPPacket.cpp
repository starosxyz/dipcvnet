/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/UnknownUDPPacket.h>

UnknownUDPPacket::UnknownUDPPacket()
{
    ::memset(m_etherpayload, 0, sizeof(m_etherpayload));
}
UnknownUDPPacket::UnknownUDPPacket(const UnknownUDPPacket &right)
    :UDPPacket(right)
{
    m_etherpayloadsize=right.m_etherpayloadsize;

    ::memcpy(m_etherpayload,right.m_etherpayload,m_etherpayloadsize);

}

UnknownUDPPacket &UnknownUDPPacket::operator=(const UnknownUDPPacket &right)
{
    UDPPacket::operator=(right);
    m_etherpayloadsize=right.m_etherpayloadsize;

    ::memcpy(m_etherpayload,right.m_etherpayload,m_etherpayloadsize);

    return *this;
}

UnknownUDPPacket::~UnknownUDPPacket()
{

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
CAWResult UnknownUDPPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    //定义结果集  并使用上一层网络为其赋输入流对象
    CAWResult result = CAW_OK;
    result = UDPPacket::StreamFrom(is);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("CEthernet::StreamFrom fail");
        result = CAW_ERROR_FAILURE;
    }
    else 
    {
        size_t remain=m_udplength-8;
        
        if (remain>ETHERNET_MAX_SIZE)
        {
            return CAW_ERROR_FAILURE;
        }
        
        is.Read(m_etherpayload, remain);
        m_etherpayloadsize=remain;
    }
    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return result;
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
CAWResult UnknownUDPPacket::StreamTo(CAWByteStreamNetwork& os)
{
    CAWResult result = CAW_OK;
    result = UDPPacket::StreamTo(os);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("CEthernet::StreamTo fail");
        result = CAW_ERROR_FAILURE;
    }
    else
    {
        os.Write(m_etherpayload, m_etherpayloadsize);
    }
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return result;
}
CAWResult UnknownUDPPacket::FromJson(Json::Value& jsonin)
{
    UDPPacket::FromJson(jsonin);

    
    return CAW_OK;
}
CAWResult UnknownUDPPacket::ToJson(Json::Value& jsonout)
{
    UDPPacket::ToJson(jsonout);

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
uint16_t UnknownUDPPacket::GetStreamLen() const
{
    return UDPPacket::GetStreamLen()+m_etherpayloadsize;

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

uint16_t UnknownUDPPacket::GetEncodeSpace()   const
{
    return UDPPacket::GetEncodeSpace()+m_etherpayloadsize;
}
void UnknownUDPPacket::Dump() const
{
    CAW_INFO_TRACE("UnknownUDPPacket::Dump()");

}

size_t  UnknownUDPPacket::SetAppPayload(char *data, size_t datasize)
{
    if ((data == NULL) || (datasize==0))
    {
        return 0;
    }
    if (datasize>ETHERNET_MAX_SIZE)
    {
        m_etherpayloadsize=ETHERNET_MAX_SIZE;
    }
    else 
    {
        m_etherpayloadsize=datasize;
    }
    ::memcpy(m_etherpayload, data, m_etherpayloadsize);
    return m_etherpayloadsize;
}

void UnknownUDPPacket::GetAppPayload(char *&payload, size_t &payloadsize)
{
    payload=m_etherpayload;
    payloadsize=m_etherpayloadsize;
}
uint16_t UnknownUDPPacket::UDPCheckSum()
{
   // std::cout << "UnknownUDPPacket::UDPCheckSum" << std::endl;
    uint16_t tochecksum=0;
    uint16_t length = GetStreamLen();
    
    CAWMessageBlock msgblk(length);
    
    CAWByteStreamNetwork checksumos(msgblk);

    checksumos<<m_sourcePort;
    checksumos<<m_destinationPort;
    checksumos<<length;
    checksumos<<m_udpchecksum;
    if (m_etherpayloadsize != 0)
    {
        checksumos.Write(m_etherpayload, m_etherpayloadsize);
    }
    
    uint16_t accumulation = 0;
    accumulation += (m_sourceAddress>> 16 & 0xffff)
            + (m_sourceAddress & 0xffff);
    accumulation += (m_destinationAddress >> 16 & 0xffff)
            + (m_destinationAddress & 0xffff);
    accumulation += m_protocol & 0xff;
    accumulation += length & 0xffff;

    for (int i = 0; i < length / 2; ++i) {
        uint16_t short16=0;
        checksumos>>short16;
        accumulation += 0xffff & short16;
    }
    // pad to an even number of shorts
    if (length % 2 > 0) {
        uint8_t bit8=0;
        checksumos>>bit8;
        accumulation += (bit8 & 0xff) << 8;
    }

    accumulation = (accumulation >> 16 & 0xffff)
            + (accumulation & 0xffff);
    tochecksum = (uint16_t) (~accumulation & 0xffff);
    return tochecksum;
}


