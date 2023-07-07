/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/ARPPacket.h>

ARPPacket::ARPPacket()
    :CEthernet(0x0806)
    ,m_hardwareType(0)
    ,m_protocolType(0)
    ,m_hardwareAddressLength(0)
    ,m_protocolAddressLength(0)
    ,m_opCode(0)
    ,m_senderProtocolAddress(0)
    ,m_targetProtocolAddress(0)
{
    ::memset(m_senderHardwareAddress, 0, sizeof(m_senderHardwareAddress));
    ::memset(m_targetHardwareAddress, 0, sizeof(m_targetHardwareAddress));
}
ARPPacket::~ARPPacket()
{

}

CAWResult ARPPacket::StreamFrom(CAWByteStreamNetwork& is)
{
    CAWResult result = CAW_OK;
    result = CEthernet::StreamFrom(is);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("CEthernet::StreamFrom fail");
        result = CAW_ERROR_FAILURE;
    }
    else 
    {
        //读进来
        is>>m_hardwareType;
        is>>m_protocolType;
        is>>m_hardwareAddressLength;
        is>>m_protocolAddressLength;
        is>>m_opCode;
        is.Read(m_senderHardwareAddress, sizeof(m_senderHardwareAddress));
        is>>m_senderProtocolAddress;
        is.Read(m_targetHardwareAddress, sizeof(m_targetHardwareAddress));
        is>>m_targetProtocolAddress;
    }
    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return result;
}

CAWResult ARPPacket::StreamTo(CAWByteStreamNetwork& os)
{
    CAWResult result = CAW_OK;
    result = CEthernet::StreamTo(os);
    if (CAW_FAILED(result))
    {
        CAW_ERROR_TRACE("CEthernet::StreamTo fail");
        result = CAW_ERROR_FAILURE;
    }
    else
    {
        //写出去
        os<<m_hardwareType;
        os<<m_protocolType;
        os<<m_hardwareAddressLength;
        os<<m_protocolAddressLength;
        os<<m_opCode;
        os.Write(m_senderHardwareAddress, sizeof(m_senderHardwareAddress));
        os<<m_senderProtocolAddress;
        os.Write(m_targetHardwareAddress, sizeof(m_targetHardwareAddress));
        os<<m_targetProtocolAddress;
    }
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    return result;
}
CAWResult ARPPacket::FromJson(Json::Value& jsonin)
{
    //以JSON形式读取数据
    CEthernet::FromJson(jsonin);

    if (!jsonin[ARPPACKET_JSON_PAYLOAD].isNull())
    {
        Json::Value &payloadjson = jsonin[ARPPACKET_JSON_PAYLOAD];
        ValueFromJson(payloadjson, ARPPACKET_JSON_HARDWARETYPE, m_hardwareType);
        ValueFromJson(payloadjson, ARPPACKET_JSON_PROTOCOLTYPE, m_protocolType);
        ValueFromJson(payloadjson, ARPPACKET_JSON_HARDWAREADDRESSLENGTH, m_hardwareAddressLength);
        ValueFromJson(payloadjson, ARPPACKET_JSON_PROTOCOLADDRESSLENGTH, m_protocolAddressLength);
        ValueFromJson(payloadjson, ARPPACKET_JSON_OPCODE, m_opCode);
        MacAddressFromJson(payloadjson, ARPPACKET_JSON_SENDERHARDWAREADDRESS, (uint8_t*)m_senderHardwareAddress);
        IPv4FromJson(payloadjson, ARPPACKET_JSON_SENDERPROTOCOLADDRESS, m_senderProtocolAddress);
        MacAddressFromJson(payloadjson, ARPPACKET_JSON_TARGETHARDWAREADDRESS, (uint8_t*)m_targetHardwareAddress);
        IPv4FromJson(payloadjson, ARPPACKET_JSON_TARGETPROTOCOLADDRESS, m_targetProtocolAddress);
    
}
    
    return CAW_OK;
}
CAWResult ARPPacket::ToJson(Json::Value& jsonout)
{
    //以json形式写出数据
    CEthernet::ToJson(jsonout);

    Json::Value payloadjsonout;
    payloadjsonout[ARPPACKET_JSON_HARDWARETYPE]=ValueToJson(m_hardwareType);
    payloadjsonout[ARPPACKET_JSON_PROTOCOLTYPE]=ValueToJson(m_protocolType);
    payloadjsonout[ARPPACKET_JSON_HARDWAREADDRESSLENGTH]=ValueToJson(m_hardwareAddressLength);
    payloadjsonout[ARPPACKET_JSON_PROTOCOLADDRESSLENGTH]=ValueToJson(m_protocolAddressLength);
    payloadjsonout[ARPPACKET_JSON_OPCODE]=ValueToJson(m_opCode);
    payloadjsonout[ARPPACKET_JSON_SENDERHARDWAREADDRESS]=MacAddressToJson((uint8_t*)m_senderHardwareAddress);
    payloadjsonout[ARPPACKET_JSON_SENDERPROTOCOLADDRESS]=IPv4ToJson(m_senderProtocolAddress);
    payloadjsonout[ARPPACKET_JSON_TARGETHARDWAREADDRESS]=MacAddressToJson((uint8_t*)m_targetHardwareAddress);
    payloadjsonout[ARPPACKET_JSON_TARGETPROTOCOLADDRESS]=IPv4ToJson(m_targetProtocolAddress);
    jsonout[ARPPACKET_JSON_PAYLOAD]=payloadjsonout;
    return CAW_OK;
}

uint16_t ARPPacket::GetStreamLen() const
{
    return (CEthernet::GetStreamLen()+
            sizeof(m_hardwareType) + 
            sizeof(m_protocolType) + 
            sizeof(m_hardwareAddressLength) + 
            sizeof(m_protocolAddressLength) +
            sizeof(m_opCode) +
            sizeof(m_senderHardwareAddress) +
            sizeof(m_senderProtocolAddress) +
            sizeof(m_targetHardwareAddress) +
            sizeof(m_targetProtocolAddress));
}

uint16_t ARPPacket::GetEncodeSpace()   const
{
        return (CEthernet::GetEncodeSpace()+
                sizeof(m_hardwareType) + 
                sizeof(m_protocolType) + 
                sizeof(m_hardwareAddressLength) + 
                sizeof(m_protocolAddressLength) +
                sizeof(m_opCode) +
                sizeof(m_senderHardwareAddress) +
                sizeof(m_senderProtocolAddress) +
                sizeof(m_targetHardwareAddress) +
                sizeof(m_targetProtocolAddress));
}
void ARPPacket::Dump() const
{
    CAW_INFO_TRACE("ARPPacket::Dump()");
    CAW_INFO_TRACE("hardwareType=" << m_hardwareType 
                    << "protocolType=" << m_protocolType 
                    << "hardwareAddressLength=" << m_hardwareAddressLength 
                    << "protocolAddressLength=" << m_protocolAddressLength
                    << "opCode=" << m_opCode
                    << "senderHardwareAddress=" << m_senderHardwareAddress
                    << "senderProtocolAddress=" << m_senderProtocolAddress
                    << "targetHardwareAddress=" << m_targetHardwareAddress
                    << "targetProtocolAddress=" << m_targetProtocolAddress);

}


void ARPPacket::GetSenderHardwareAddress(char mac[ETHERNET_MAC_LENGTH])
{
    ::memcpy(mac, m_senderHardwareAddress,ETHERNET_MAC_LENGTH);
}

void ARPPacket::GetTargetHardwareAddress(char mac[ETHERNET_MAC_LENGTH])
{
    ::memcpy(mac, m_targetHardwareAddress,ETHERNET_MAC_LENGTH);
}
void ARPPacket::SetSenderHardwareAddress(char mac[ETHERNET_MAC_LENGTH])
{
    ::memcpy(m_senderHardwareAddress, mac,ETHERNET_MAC_LENGTH);
}

void ARPPacket::SetTargetHardwareAddress(char mac[ETHERNET_MAC_LENGTH])
{
    ::memcpy(m_targetHardwareAddress, mac,ETHERNET_MAC_LENGTH);
}


