/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/DIPCPacket.h>

DIPCPacket::DIPCPacket()
    :CEthernet(0x9946)
{
	::memset(&m_dstaddr,0,sizeof(m_dstaddr));
	::memset(&m_srcaddr,0,sizeof(m_srcaddr));
    m_payloadsize = 0;
    m_dipctype = 0;
    m_payload = NULL;
}
DIPCPacket::~DIPCPacket()
{
    if (m_payload)
    {
        delete[] m_payload;
        m_payload = NULL;
    }

}

DIPCPacket::DIPCPacket(const DIPCPacket &right)
    :CEthernet(right)
{
    (*this) = right;
}

DIPCPacket &DIPCPacket::operator=(const DIPCPacket &right)
{
    if (this != &right)
    {
        CEthernet::operator=(right);
		::memcpy(&m_dstaddr,&(right.m_dstaddr),sizeof(m_dstaddr));
		::memcpy(&m_srcaddr,&(right.m_srcaddr),sizeof(m_srcaddr));

        m_dipctype = right.m_dipctype;
        m_payloadsize = right.m_payloadsize;
        if (m_payload)
        {
            delete[] m_payload;
            m_payload = NULL;
        }
        if (m_payloadsize > 0)
        {
            m_payload = new char[m_payloadsize];
            ::memcpy(m_payload, right.m_payload, m_payloadsize);
        }
    }


    return *this;
}

CAWResult DIPCPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    CEthernet::StreamFrom(is);
	is>>m_dstaddr.jno;
	is>>m_dstaddr.pno;
	is>>m_dstaddr.nodeid;
	is>>m_dstaddr.clusterid;
	is>>m_dstaddr.datacenterid;
	is>>m_srcaddr.jno;
	is>>m_srcaddr.pno;
	is>>m_srcaddr.nodeid;
	is>>m_srcaddr.clusterid;
	is>>m_srcaddr.datacenterid;
    is >> m_dipctype;
    is>> m_payloadsize;
    if (m_payloadsize > 0)
    {
        m_payload = new char[m_payloadsize];
        if (m_payload)
        {
            is.Read(m_payload, m_payloadsize);
        }
        else
        {
            m_payloadsize = 0;
        }
    }
    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;
}

CAWResult DIPCPacket::StreamTo(CAWByteStreamNetwork& os)
{
    CEthernet::StreamTo(os);
	os << m_dstaddr.jno;
	os << m_dstaddr.pno;
	os << m_dstaddr.nodeid;
	os << m_dstaddr.clusterid;
	os << m_dstaddr.datacenterid;
	os << m_srcaddr.jno;
	os << m_srcaddr.pno;
	os << m_srcaddr.nodeid;
	os << m_srcaddr.clusterid;
	os << m_srcaddr.datacenterid;
    os << m_dipctype;
    os << m_payloadsize;
    if (m_payloadsize > 0)
    {
        if (m_payload)
        {
            os << m_payloadsize;
            os.Write(m_payload, m_payloadsize);
        }
        else
        {
            m_payloadsize = 0;
            os << m_payloadsize;
        }
    }
    else
    {
        os << m_payloadsize;
    }
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return CAW_OK;
}
CAWResult DIPCPacket::FromJson(Json::Value& jsonin)
{
    CEthernet::FromJson(jsonin);
    return CAW_OK;

}

CAWResult DIPCPacket::ToJson(Json::Value& jsonout)
{
    CEthernet::ToJson(jsonout);
    return CAW_OK;
}


uint16_t DIPCPacket::GetStreamLen() const
{
    return CEthernet::GetStreamLen() + sizeof(m_srcaddr)
        + sizeof(m_dstaddr)
        + sizeof(m_dipctype)
        + sizeof(m_payloadsize)
        + m_payloadsize;

}

uint16_t DIPCPacket::GetEncodeSpace() const
{
    return CEthernet::GetStreamLen() + sizeof(m_srcaddr)
        + sizeof(m_dstaddr)
        + sizeof(m_dipctype)
        + sizeof(m_payloadsize)
        + m_payloadsize;
}
void DIPCPacket::Dump() const
{

}

void DIPCPacket::SetPayload(const char* pdata, size_t datasize)
{
    if (m_payload)
    {
        delete[] m_payload;
        m_payload = NULL;
    }
    if (datasize == 0)
    {
        m_payloadsize = 0;
        return;
    }
    if (pdata == NULL)
    {
        m_payloadsize = 0;
        return;
    }

    m_payload = new char[datasize];
    if (m_payload)
    {
        ::memcpy(m_payload, pdata, datasize);
        m_payloadsize = (uint16_t)datasize;
    }
}
void DIPCPacket::GetPlayload(char*& pdata, size_t& datasize)
{
    pdata = m_payload;
    datasize = (size_t)m_payloadsize;
}

void DIPCPacket::DestroyClone()
{
    delete this;
}
DIPCPacket* DIPCPacket::ClonePdu()
{
    DIPCPacket* pbase = new DIPCPacket();
    *pbase = *this;
    return pbase;
}

