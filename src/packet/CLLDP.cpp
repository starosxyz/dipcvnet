/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/CLLDP.h>

CLLDP::CLLDP()
            :CEthernet(0x88CC)
{
    m_dmac[0] = 0x01;
    m_dmac[1] = 0x80;
    m_dmac[2] = 0xc2;
    m_dmac[3] = 0x00;
    m_dmac[4] = 0x00;
    m_dmac[5] = 0x0e;

    m_len = 0/*chassisId.GetStreamLen() + portId.GetStreamLen() + ttl.GetStreamLen()*/;
}

CLLDP:: ~CLLDP()
{
    //optionalTLVList.clear();
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
CAWResult CLLDP::StreamFrom(CAWByteStreamNetwork& is)
{
    CAW_INFO_TRACE("CLLDP::StreamFrom begin");
    if (CEthernet::StreamFrom(is) != CAW_OK)
    {
        return CAW_ERROR_FAILURE;
    }
    //读取LLDP协议,判断前三种type是否为chassidid portid ttl
    if (chassisId.StreamFrom(is) != CAW_OK)
    {
        CAW_INFO_TRACE("CLLDP::StreamFrom chassisId.StreamFrom error");
        return CAW_ERROR_FAILURE;
    }
    if (chassisId.GetLldpType() != 1)
    {
        CAW_INFO_TRACE("CLLDP::StreamFrom chassisId type is not 1");
        return CAW_ERROR_FAILURE;
    }
    if (portId.StreamFrom(is) != CAW_OK)
    {
        CAW_INFO_TRACE("CLLDP::StreamFrom portId.StreamFrom error");
        return CAW_ERROR_FAILURE;
    }
    if (portId.GetLldpType() != 2)
    {
        CAW_INFO_TRACE("CLLDP::StreamFrom portId type is not 2");
        return CAW_ERROR_FAILURE;
    }
    if (ttl.StreamFrom(is) != CAW_OK)
    {
        CAW_INFO_TRACE("CLLDP::StreamFrom ttl.StreamFrom error");
        return CAW_ERROR_FAILURE;
    }
    if (ttl.GetLldpType() != 3)
    {
        CAW_INFO_TRACE("CLLDP::StreamFrom ttl type is not 3");
        return CAW_ERROR_FAILURE;  
    }
    //定义协议结尾长度 并读取
    uint16_t endofLen = 0;
    //计算tlv剩余长度
    size_t remainsize = is.GetChainedLength();
    uint16_t length = 0;
	while(remainsize>0)
	{
        if (remainsize == 2 || remainsize == 9)
        {
            break;
        }
        uint16_t temp=0;
        is.PreRead(&temp, sizeof(temp));
        temp=ntohs(temp);
        if (temp==0)
        {
            break;
        }
		LLDPTLV tlv;
		if (tlv.StreamFrom(is) != CAW_OK)
		{
            return CAW_ERROR_FAILURE;
		}
        if (tlv.GetLldpType() == 0)
        {
            break;
	}
        optionalTLVList.push_back(tlv);

        remainsize=remainsize-tlv.GetStreamLen();
    }
    
    size_t syuSize = is.GetChainedLength();
    CAW_INFO_TRACE("CLLDP::StreamFrom syuSize="<<syuSize);
   
    is>>endofLen;
    bool bGood = is.IsGood();
    
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    
    for (LLDPTLV& lldptlv : optionalTLVList)
    {
        length += lldptlv.GetStreamLen();
    }
    //计算lldp 中tlv总长度
    m_len = length + chassisId.GetStreamLen() + portId.GetStreamLen() + ttl.GetStreamLen();
    CAW_INFO_TRACE("CLLDP::StreamFrom end");
   // std::cout << "CLLDP::StreamFrom end" << std::endl;
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
* 2019/12/18  V1.0    夏章辉     创建
************************************************************************/
CAWResult CLLDP::StreamTo(CAWByteStreamNetwork& os)
{
    if (CEthernet::StreamTo(os) != CAW_OK)
    {
        return CAW_ERROR_FAILURE;
    }
    //分别将协议中固定的三种tlv写出去
    CAW_INFO_TRACE("CLLDP::StreamTo chassisid");
    chassisId.StreamTo(os);
    CAW_INFO_TRACE("CLLDP::StreamTo portid");
	portId.StreamTo(os);
    CAW_INFO_TRACE("CLLDP::StreamTo ttl");
	ttl.StreamTo(os);
    //写出集合中的tlv
    for (LLDPTLV& lldptlv : optionalTLVList)
   {
        lldptlv.StreamTo(os);
    }
    //定义结尾长度 并写出
    uint16_t endofLen = 0;
    os<<endofLen;
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return CAW_OK;
}
CAWResult CLLDP::FromJson(Json::Value& jsonin)
{
    return CAW_OK;
}
CAWResult CLLDP::ToJson(Json::Value& jsonout)
{
    return CAW_OK;
}
uint16_t CLLDP::GetStreamLen() const
{
    return CEthernet::GetStreamLen() + m_len + 2;
}
uint16_t CLLDP::GetEncodeSpace()   const
{
    return CEthernet::GetStreamLen() + m_len + 2;
}

void CLLDP::Dump() const
{
    CAW_INFO_TRACE("CLLDP::Dump()");
   /* uint8_t chassidType = chassisId.GetLldpType();
    uint16_t chassidLength = chassisId.GetLldpLength();
    
    uint8_t portIdType = portId.GetLldpType();
    uint16_t portIdLength = portId.GetLldpLength();

    uint8_t ttlType = ttl.GetLldpType();
    uint16_t ttlLength = ttl.GetLldpLength();

    size_t listsize = optionalTLVList.size();
    
    CAW_INFO_TRACE("chassidType=" << chassidType
                    << "chassidLength=" << chassidLength 
                    << "portIdType=" << portIdType 
                    << "portIdLength=" << portIdLength
                    << "ttlType=" << ttlType
                    << "ttlLength=" << ttlLength
                    << "listsize=" << listsize); */             
}

CLLDP::CLLDP(const CLLDP &right)
    :CEthernet(right)
{
    CEthernet::operator=(right);
    chassisId=right.chassisId;
    portId = right.portId;
    ttl = right.ttl;
    optionalTLVList = right.optionalTLVList;
    int length=0;
    for (LLDPTLV& lldptlv : optionalTLVList)
    {
        length += lldptlv.GetStreamLen();
    }
    m_len=length+chassisId.GetStreamLen()+portId.GetStreamLen()+ttl.GetStreamLen();

}

CLLDP &CLLDP::operator=(const CLLDP &right)
{
    CEthernet::operator=(right);
    chassisId=right.chassisId;
    portId = right.portId;
    ttl = right.ttl;
    optionalTLVList = right.optionalTLVList;
    int length=0;
    for (LLDPTLV& lldptlv : optionalTLVList)
    {
        length += lldptlv.GetStreamLen();
    }
    m_len=length;

    return *this;
}


void CLLDP::SetChassisId(const LLDPTLV  &   strchassisId)
{
    chassisId = strchassisId;
    m_len += chassisId.GetStreamLen();
}
void CLLDP::SetPortId(const LLDPTLV    &strportId)
{
    portId = strportId;
    m_len += portId.GetStreamLen();
}
void CLLDP::SetTtl(const LLDPTLV  &strttl)
{
    ttl = strttl;
    m_len += ttl.GetStreamLen();
}
size_t CLLDP::GetLLDPTLVSize()
{
     return optionalTLVList.size();
}
 void CLLDP::addOptionalTLV(LLDPTLV &lldptlv)
 {
    optionalTLVList.push_back(lldptlv);
    m_len += lldptlv.GetStreamLen();
    //std::cout << "m_len = " << m_len << std::endl;
 }
 
 CAWString CLLDP::GetValueByType(uint8_t &strtype)
 {
    CAWString svalue = "";
     for (LLDPTLV& lldptlv : optionalTLVList)
         {
             if (lldptlv.GetLldpType() == strtype)
             {
                 svalue = lldptlv.GetMvalue();
             }
         }
         return svalue;
 }
 std::list<LLDPTLV> CLLDP::GetTlvList()
 {
   return optionalTLVList;
 }
 void CLLDP::GetTlvList(std::list<LLDPTLV> &list)
 {
     list = optionalTLVList;
 }






