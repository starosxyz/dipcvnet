/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/LinkDataPacket.h>



LinkDataPacket::LinkDataPacket()
    :CEthernet(0x9945)
{
    m_dstport = 0;
    m_srcport = 0;
    m_dstdpid = 0;
    m_srcdpid = 0;
    m_datasize = 0;
    m_xid = 0;
    m_pdata = NULL;
    m_flags = 0;
}
LinkDataPacket::~LinkDataPacket()
{
    if (m_pdata)
    {
        delete[] m_pdata;
        m_pdata = NULL;
    }

}

LinkDataPacket::LinkDataPacket(const LinkDataPacket &right)
    :CEthernet(right)
{
    (*this) = right;
}

LinkDataPacket &LinkDataPacket::operator=(const LinkDataPacket &right)
{
    if (this != &right)
    {
        CEthernet::operator=(right);

        m_dstport = right.m_dstport;
        m_srcport = right.m_srcport;
        m_dstdpid = right.m_dstdpid;
        m_srcdpid = right.m_srcdpid;
        m_xid = right.m_xid;
        m_flags = right.m_flags;
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

CAWResult LinkDataPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    CEthernet::StreamFrom(is);
    is>> m_dstport;
    is>> m_srcport;
    is>> m_dstdpid;
    is>> m_srcdpid;
    is >> m_xid;
    is >> m_flags;
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

CAWResult LinkDataPacket::StreamTo(CAWByteStreamNetwork& os)
{
    CEthernet::StreamTo(os);
    os<< m_dstport;
    os<< m_srcport;
    os<< m_dstdpid;
    os<< m_srcdpid;
    os << m_xid;
    os << m_flags;
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
CAWResult LinkDataPacket::FromJson(Json::Value& jsonin)
{
    CEthernet::FromJson(jsonin);
    return CAW_OK;

}

CAWResult LinkDataPacket::ToJson(Json::Value& jsonout)
{
    CEthernet::ToJson(jsonout);
    return CAW_OK;
}


uint16_t LinkDataPacket::GetStreamLen() const
{
    return CEthernet::GetStreamLen() + sizeof(m_dstport)
        + sizeof(m_srcport)
        + sizeof(m_dstdpid)
        + sizeof(m_srcdpid)
        +sizeof(m_xid)
        + sizeof(m_datasize)
        +sizeof(m_flags)
        + m_datasize;

}

uint16_t LinkDataPacket::GetEncodeSpace() const
{
    return CEthernet::GetStreamLen() + sizeof(m_dstport)
        + sizeof(m_srcport)
        + sizeof(m_dstdpid)
        + sizeof(m_srcdpid)
        + sizeof(m_xid)
        + sizeof(m_datasize)
        + sizeof(m_flags)
        + m_datasize;
}
void LinkDataPacket::Dump() const
{

}

void LinkDataPacket::SetPayload(const char* pdata, size_t datasize)
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
void LinkDataPacket::GetPlayload(char*& pdata, size_t& datasize)
{
    pdata = m_pdata;
    datasize = (size_t)m_datasize;
}

void LinkDataPacket::DestroyClone()
{
    delete this;
}
LinkDataPacket* LinkDataPacket::ClonePdu()
{
    LinkDataPacket* pbase = new LinkDataPacket();
    *pbase = *this;
    return pbase;
}

