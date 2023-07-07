/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _ARPPACKET_H
#define _ARPPACKET_H
#include <packet/Ethernet.h>

#define ARPPACKET_JSON_HARDWARETYPE "hardwareType"
#define ARPPACKET_JSON_PROTOCOLTYPE "protocolType"
#define ARPPACKET_JSON_HARDWAREADDRESSLENGTH "hardwareaddresslength"
#define ARPPACKET_JSON_PROTOCOLADDRESSLENGTH "protocoladdresslength"
#define ARPPACKET_JSON_OPCODE "opcode"
#define ARPPACKET_JSON_SENDERHARDWAREADDRESS "senderhardwareaddress"
#define ARPPACKET_JSON_SENDERPROTOCOLADDRESS "senderprotocoladdress"
#define ARPPACKET_JSON_TARGETHARDWAREADDRESS "targethardwareaddress"
#define ARPPACKET_JSON_TARGETPROTOCOLADDRESS "targetprotocoladdress"
#define ARPPACKET_JSON_PAYLOAD "arp"

class CAW_OS_EXPORT ARPPacket : public CEthernet
{
public:
    ARPPacket();
    virtual ~ARPPacket();
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;
    virtual void Dump() const;
    inline uint16_t GetHardwareType() const {return m_hardwareType;}
    inline uint16_t GetProtocolType() const {return m_protocolType;}
    inline uint8_t GetHardwareAddressLength() const {return m_hardwareAddressLength;}
    inline uint8_t GetProtocolAddressLength() const {return m_protocolAddressLength;}
    inline uint16_t GetOpCode() const {return m_opCode;}
    inline uint32_t GetSenderProtocolAddress(){return m_senderProtocolAddress;}
    inline uint32_t GetTargetProtocolAddress(){return m_targetProtocolAddress;}
    void GetSenderHardwareAddress(char mac[ETHERNET_MAC_LENGTH]);
    void GetTargetHardwareAddress(char mac[ETHERNET_MAC_LENGTH]);

    inline void SetHardwareType(uint16_t type){m_hardwareType=type;}
    inline void SetProtocolType(uint16_t type){m_protocolType=type;}
    inline void SetHardwareAddressLength(uint8_t length){m_hardwareAddressLength=length;}
    inline void SetProtocolAddressLength(uint8_t length){m_protocolAddressLength=length;}
    inline void SetOpCode(uint16_t opcode){m_opCode=opcode;}
    inline void SetSenderProtocolAddress(uint32_t paddr){m_senderProtocolAddress=paddr;}
    inline void SetTargetProtocolAddress(uint32_t paddr){m_targetProtocolAddress=paddr;}
    void SetSenderHardwareAddress(char mac[ETHERNET_MAC_LENGTH]);
    void SetTargetHardwareAddress(char mac[ETHERNET_MAC_LENGTH]);

protected:
    uint16_t m_hardwareType;
    uint16_t m_protocolType;
    uint8_t m_hardwareAddressLength;
    uint8_t m_protocolAddressLength;
    uint16_t m_opCode;
    char m_senderHardwareAddress[ETHERNET_MAC_LENGTH];
    uint32_t m_senderProtocolAddress;
    char m_targetHardwareAddress[ETHERNET_MAC_LENGTH];
    uint32_t m_targetProtocolAddress;
};


#endif //_ETHERNET_H

