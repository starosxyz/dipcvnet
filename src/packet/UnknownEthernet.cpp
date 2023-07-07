/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/UnknownEthernet.h>

UnknownEthernet::UnknownEthernet()
{
    ::memset(m_etherpayload, 0, sizeof(m_etherpayload));
}
UnknownEthernet::UnknownEthernet(uint16_t type)
    :CEthernet(type)
{
    
}
UnknownEthernet::UnknownEthernet(const UnknownEthernet &right)
    :CEthernet(right)
{
    m_etherpayloadsize=right.m_etherpayloadsize;

    ::memcpy(m_etherpayload,right.m_etherpayload,m_etherpayloadsize);

}

UnknownEthernet &UnknownEthernet::operator=(const UnknownEthernet &right)
{
    CEthernet::operator=(right);
    m_etherpayloadsize=right.m_etherpayloadsize;

    ::memcpy(m_etherpayload,right.m_etherpayload,m_etherpayloadsize);

    return *this;
}

UnknownEthernet::~UnknownEthernet()
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

CAWResult UnknownEthernet::StreamFrom(CAWByteStreamNetwork& is)
{
    //定义结果集  并使用上一层网络为其赋输入流对象
    CAWResult result = CAW_OK;
    result = CEthernet::StreamFrom(is);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("CEthernet::StreamFrom fail");
        result = CAW_ERROR_FAILURE;
    }
    else 
    {
        size_t remain=is.GetChainedLength();
        
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

CAWResult UnknownEthernet::StreamTo(CAWByteStreamNetwork& os)
{
    //定义结果集  并使用上一层网络为其赋输出流对象
    CAWResult result = CAW_OK;
    result = CEthernet::StreamTo(os);
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
CAWResult UnknownEthernet::FromJson(Json::Value& jsonin)
{
    CEthernet::FromJson(jsonin);

    
    return CAW_OK;
}
CAWResult UnknownEthernet::ToJson(Json::Value& jsonout)
{
    CEthernet::ToJson(jsonout);

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

uint16_t UnknownEthernet::GetStreamLen() const
{
    return CEthernet::GetStreamLen()+m_etherpayloadsize;

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

uint16_t UnknownEthernet::GetEncodeSpace()   const
{
    return CEthernet::GetEncodeSpace()+m_etherpayloadsize;
}
void UnknownEthernet::Dump() const
{
    CAW_INFO_TRACE("UnknownEthernet::Dump()");

}

size_t  UnknownEthernet::SetAppPayload(char *data, size_t datasize)
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

void UnknownEthernet::GetAppPayload(char *&payload, size_t &payloadsize)
{
    payload=m_etherpayload;
    payloadsize=m_etherpayloadsize;
}


