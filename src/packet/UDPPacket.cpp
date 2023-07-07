/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/UDPPacket.h>


#define UDP_MAX_SIZE 1400
UDPPacket::UDPPacket()
    :IPPacket(17)
    ,m_sourcePort(0)
     ,m_destinationPort(0)
     ,m_udplength(0)
     ,m_udpchecksum(0)
{

}

UDPPacket::~UDPPacket()
{

}

CAWResult UDPPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    CAWResult result = CAW_OK;
    result = IPPacket::StreamFrom(is);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("UDPPacket::StreamFromHeader fail");
        result = CAW_ERROR_FAILURE;
    }
    else
    {
        is>>m_sourcePort;
        is>>m_destinationPort;
        is>>m_udplength;
        is>>m_udpchecksum;

        bool bGood = is.IsGood();
        CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    }

    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);

    return result;
}

CAWResult UDPPacket::StreamTo(CAWByteStreamNetwork& os)
{
    CAWResult result = CAW_OK;
    result = IPPacket::StreamTo(os);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("UDPPacket::StreamToHeader fail");
        result = CAW_ERROR_FAILURE;
    }
    else
    {
		if (m_udplength ==0)
		{
			m_udplength = GetStreamLen()-IPPacket::GetStreamLen();
		}
		if (m_udpchecksum==0)
		{
			m_udpchecksum=UDPCheckSum();
		}
        os<<m_sourcePort;
        os<<m_destinationPort;
        os<<m_udplength;
        os<<m_udpchecksum;
        
        bool bGood = os.IsGood();
        CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    }
    return result;
}
CAWResult UDPPacket::FromJson(Json::Value& jsonin)
{
    CAW_INFO_TRACE_THIS("UDPPacket::FromJson");

    IPPacket::FromJson(jsonin);
    if (!jsonin["udp"].isNull())
    {
        Json::Value &payloadjson = jsonin["udp"];
        ValueFromJson(payloadjson, CPACKET_JSON_SPORT, m_sourcePort);
        ValueFromJson(payloadjson, CPACKET_JSON_DPORT, m_destinationPort);
        ValueFromJson(payloadjson, CPACKET_JSON_CHECKSUM, m_udpchecksum);
        ValueFromJson(payloadjson, CPACKET_JSON_PAYLOAD_SIZE, m_udplength); 
     }

    return CAW_OK;
}
CAWResult UDPPacket::ToJson(Json::Value& jsonout)
{
    CAW_INFO_TRACE_THIS("UDPPacket::ToJson");

    IPPacket::ToJson(jsonout);


    Json::Value udppayloadjson;
	if (m_udpchecksum==0)
	{
		m_udpchecksum=UDPCheckSum();
	}

    udppayloadjson[CPACKET_JSON_SPORT]=ValueToJson(m_sourcePort);
    udppayloadjson[CPACKET_JSON_DPORT]=ValueToJson(m_destinationPort);
    udppayloadjson[CPACKET_JSON_CHECKSUM]=ValueToJson(m_udpchecksum);
    udppayloadjson[CPACKET_JSON_PAYLOAD_SIZE]=ValueToJson(m_udplength);
    
    jsonout["udp"]=udppayloadjson;

    return CAW_OK;
}

uint16_t UDPPacket::GetStreamLen() const
{        
    return IPPacket::GetStreamLen() + 8;
}

uint16_t UDPPacket::GetEncodeSpace() const
{
    return IPPacket::GetEncodeSpace() + 8; 
}


void UDPPacket::Dump() const
{
    IPPacket::Dump();
}
void UDPPacket::SetSourcePort(uint16_t port)
{
    m_sourcePort = port;
}
void UDPPacket::SetDestinationPort(uint16_t port)
{
    m_destinationPort = port;
}
uint16_t UDPPacket::GetSourcePort()
{
    return m_sourcePort;
}
uint16_t UDPPacket::GetDestinationPort()
{
    return m_destinationPort;
}


