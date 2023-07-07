/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/UnknownIPPacket.h>

UnknownIPPacket::UnknownIPPacket()
{
    ::memset(m_etherpayload, 0, sizeof(m_etherpayload));
}
UnknownIPPacket::UnknownIPPacket(uint8_t protocol)
    :IPPacket(protocol)
{
    
}
UnknownIPPacket::UnknownIPPacket(const UnknownIPPacket &right)
    :IPPacket(right)
{
    m_etherpayloadsize=right.m_etherpayloadsize;

    ::memcpy(m_etherpayload,right.m_etherpayload,m_etherpayloadsize);

}

UnknownIPPacket &UnknownIPPacket::operator=(const UnknownIPPacket &right)
{
    IPPacket::operator=(right);
    m_etherpayloadsize=right.m_etherpayloadsize;

    ::memcpy(m_etherpayload,right.m_etherpayload,m_etherpayloadsize);

    return *this;
}

UnknownIPPacket::~UnknownIPPacket()
{

}

CAWResult UnknownIPPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    CAWResult result = CAW_OK;
    result = IPPacket::StreamFrom(is);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("CEthernet::StreamFrom fail");
        result = CAW_ERROR_FAILURE;
    }
    else 
    {
        uint16_t totallength = getTotalLength();
        size_t optionsize = GetOptionSize();

        if (totallength<20)
        {
            return CAW_ERROR_FAILURE;
        }
        if (totallength-20<optionsize)  
        {
            return CAW_ERROR_FAILURE;
        }
        size_t remain=totallength-20-optionsize;

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

CAWResult UnknownIPPacket::StreamTo(CAWByteStreamNetwork& os)
{
    CAWResult result = CAW_OK;
    result = IPPacket::StreamTo(os);
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
CAWResult UnknownIPPacket::FromJson(Json::Value& jsonin)
{
    IPPacket::FromJson(jsonin);

    
    return CAW_OK;
}
CAWResult UnknownIPPacket::ToJson(Json::Value& jsonout)
{
    IPPacket::ToJson(jsonout);

    return CAW_OK;
}

uint16_t UnknownIPPacket::GetStreamLen() const
{
    return IPPacket::GetStreamLen()+m_etherpayloadsize;

}

uint16_t UnknownIPPacket::GetEncodeSpace()   const
{
    return IPPacket::GetEncodeSpace()+m_etherpayloadsize;
}
void UnknownIPPacket::Dump() const
{
    CAW_INFO_TRACE("UnknownIPPacket::Dump()");

}

size_t  UnknownIPPacket::SetAppPayload(char *data, size_t datasize)
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

void UnknownIPPacket::GetAppPayload(char *&payload, size_t &payloadsize)
{
    payload=m_etherpayload;
    payloadsize=m_etherpayloadsize;
}

