/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/UnknownTCPPacket.h>

UnknownTCPPacket::UnknownTCPPacket()
{
    ::memset(m_etherpayload, 0, sizeof(m_etherpayload));
}
UnknownTCPPacket::UnknownTCPPacket(const UnknownTCPPacket &right)
    :TCPPacket(right)
{
    m_etherpayloadsize=right.m_etherpayloadsize;

    ::memcpy(m_etherpayload,right.m_etherpayload,m_etherpayloadsize);

}

UnknownTCPPacket &UnknownTCPPacket::operator=(const UnknownTCPPacket &right)
{
    TCPPacket::operator=(right);
    m_etherpayloadsize=right.m_etherpayloadsize;

    ::memcpy(m_etherpayload,right.m_etherpayload,m_etherpayloadsize);

    return *this;
}

UnknownTCPPacket::~UnknownTCPPacket()
{

}

CAWResult UnknownTCPPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    size_t datasize = is.GetChainedLength();

    CAW_INFO_TRACE("UnknownTCPPacket::StreamFrom, datasize1="<<datasize);
    
    CAWResult result = CAW_OK;
    result = TCPPacket::StreamFrom(is);
    datasize = is.GetChainedLength();
    CAW_INFO_TRACE("UnknownTCPPacket::StreamFrom, datasize2="<<datasize);

    uint16_t totallength = getTotalLength();
    size_t optionsize = GetOptionSize();
    size_t tcpoptionsize = GetTCPOptionSize();

    if (totallength<40)
    {
        return CAW_ERROR_FAILURE;
    }
    if (totallength-40<optionsize+tcpoptionsize)  
    {
        return CAW_ERROR_FAILURE;
    }
    size_t tcpdata=totallength-40-optionsize-tcpoptionsize;
    
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("UnknownTCPPacket::StreamFrom fail");
        return CAW_ERROR_FAILURE;
    }
    else 
    {   
        //size_t remain=m_totalLength-m_headerLength*4-32;
        size_t remain =tcpdata;
        //std::cout << "totalLength = " << m_totalLength << " headerLength = " << (int)m_headerLength << std::endl;
        if (remain>ETHERNET_MAX_SIZE)
        {
            return CAW_ERROR_FAILURE;
        }
        if (remain != 0)  
        {
            is.Read(m_etherpayload, remain);
            m_etherpayloadsize=remain;
        }
        else 
        {
            m_etherpayloadsize=0;
        }
    }
    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return result;
}


CAWResult UnknownTCPPacket::StreamTo(CAWByteStreamNetwork& os)
{
    CAWResult result = CAW_OK;
    result = TCPPacket::StreamTo(os);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("CEthernet::StreamTo fail");
        result = CAW_ERROR_FAILURE;
    }
    else
    {
        if(m_etherpayloadsize != 0)
            os.Write(m_etherpayload, m_etherpayloadsize);
    }
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return result;
}
CAWResult UnknownTCPPacket::FromJson(Json::Value& jsonin)
{
    TCPPacket::FromJson(jsonin);

    
    return CAW_OK;
}
CAWResult UnknownTCPPacket::ToJson(Json::Value& jsonout)
{
    TCPPacket::ToJson(jsonout);

    return CAW_OK;
}

uint16_t UnknownTCPPacket::GetStreamLen() const
{
    return TCPPacket::GetStreamLen()+m_etherpayloadsize;

}

uint16_t UnknownTCPPacket::GetEncodeSpace()   const
{
    return TCPPacket::GetEncodeSpace()+m_etherpayloadsize;
}
void UnknownTCPPacket::Dump() const
{
    CAW_INFO_TRACE("UnknownTCPPacket::Dump()");

}

size_t  UnknownTCPPacket::SetAppPayload(char *data, size_t datasize)
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

void UnknownTCPPacket::GetAppPayload(char *&payload, size_t &payloadsize)
{
    payload=m_etherpayload;
    payloadsize=m_etherpayloadsize;
}
uint16_t UnknownTCPPacket::TCPCheckSum()
{
   // std::cout << "UnknownUDPPacket::TCPCheckSum" << std::endl;
    uint16_t tochecksum=0;
    uint16_t length = 20+m_etherpayloadsize;
    
    CAWMessageBlock msgblk(length);
    
    CAWByteStreamNetwork checksumos(msgblk);

    checksumos<<(uint16_t) (m_tcpsourcePort & 0xffff);
    checksumos<<(uint16_t) (m_tcpdestinationPort & 0xffff);
    checksumos<<m_tcpsequence;
    checksumos<<m_tcpacknowledge;
    checksumos<<(uint16_t)(m_tcpflags | m_tcpdataOffset << 12);
    checksumos<<m_tcpwindowSize;
    checksumos<<m_tcpchecksum;
    checksumos<<m_tcpurgentPointer;
    
    if (m_tcpdataOffset > 5) {
       if ((m_tcpoptionssize>0) && (m_tcpoptions != NULL))
        {
           checksumos.Write(m_tcpoptions, m_tcpoptionssize);
        }
        int padding=0;
        padding = (m_tcpdataOffset << 2) - 20 - m_tcpoptionssize;
        for (int i = 0; i < padding; i++) {
            uint8_t data=0;
            checksumos<<data;
        }
    }

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


uint16_t UnknownTCPPacket::TCPCheckSumv6()
{
    return 0;
}


