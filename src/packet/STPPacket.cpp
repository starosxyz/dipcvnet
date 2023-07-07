/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/STPPacket.h>


STPPacket::STPPacket()
    :CEthernet(0x9946)
{
    m_datasize = 0;
    m_pdata = NULL;
    char tempdmac[6] = { (char)0x01,(char)0x80,(char)0xC2,(char)0x01,(char)0x02,(char)0x03 };
    ::memcpy(m_dmac, tempdmac, 6);
}
STPPacket::~STPPacket()
{
    if (m_pdata)
    {
        delete[] m_pdata;
        m_pdata = NULL;
    }

}

STPPacket::STPPacket(const STPPacket &right)
    :CEthernet(right)
{
    (*this) = right;
}

STPPacket &STPPacket::operator=(const STPPacket &right)
{
    if (this != &right)
    {
        CEthernet::operator=(right);
        m_datasize = right.m_datasize;
        if (m_pdata)
        {
            delete[] m_pdata;
            m_pdata = NULL;
        }
        if (m_datasize > 0)
        {
            m_pdata = new char[m_datasize];
            ::memcpy(m_pdata, right.m_pdata, m_datasize);
        }
    }


    return *this;
}

CAWResult STPPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    CEthernet::StreamFrom(is);
    is>> m_datasize;
    if (m_datasize > 0)
    {
        m_pdata = new char[m_datasize];
        if (m_pdata)
        {
            is.Read(m_pdata, m_datasize);
        }
        else
        {
            m_datasize = 0;
        }
    }
    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;
}

CAWResult STPPacket::StreamTo(CAWByteStreamNetwork& os)
{
    CEthernet::StreamTo(os);
    if (m_datasize > 0)
    {
        if (m_pdata)
        {
            os << m_datasize;
            os.Write(m_pdata, m_datasize);
        }
        else
        {
            m_datasize = 0;
            os << m_datasize;
        }
    }
    else
    {
        os << m_datasize;
    }
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;
}
CAWResult STPPacket::FromJson(Json::Value& jsonin)
{
    CEthernet::FromJson(jsonin);
    return CAW_OK;

}

CAWResult STPPacket::ToJson(Json::Value& jsonout)
{
    CEthernet::ToJson(jsonout);
    return CAW_OK;
}


uint16_t STPPacket::GetStreamLen() const
{
    return CEthernet::GetStreamLen()
        + sizeof(m_datasize)
        + m_datasize;

}

uint16_t STPPacket::GetEncodeSpace() const
{
    return CEthernet::GetStreamLen()
        + sizeof(m_datasize)
        + m_datasize;
}
void STPPacket::Dump() const
{

}

void STPPacket::SetPayload(const char* pdata, size_t datasize)
{
    if (m_pdata)
    {
        delete[] m_pdata;
        m_pdata = NULL;
    }
    if (datasize == 0)
    {
        m_datasize = 0;
        return;
    }
    if (pdata == NULL)
    {
        m_datasize = 0;
        return;
    }

    m_pdata = new char[datasize];
    if (m_pdata)
    {
        ::memcpy(m_pdata, pdata, datasize);
        m_datasize = (uint16_t)datasize;
    }
}
void STPPacket::GetPlayload(char*& pdata, size_t& datasize)
{
    pdata = m_pdata;
    datasize = (size_t)m_datasize;
}

void STPPacket::DestroyClone()
{
    delete this;
}
STPPacket* STPPacket::ClonePdu()
{
    STPPacket* pbase = new STPPacket();
    if (pbase == NULL)
    {
        return NULL;
    }
    *pbase = *this;
    return pbase;
}

