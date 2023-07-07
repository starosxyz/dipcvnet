/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _ICMPETHERNET_H
#define _ICMPETHERNET_H
#include <packet/IPPacket.h>
#include <string.h>


const static uint8_t TYPE_ECHO_REQUEST = 0x08;
const static uint8_t TYPE_ECHO_REPLY = 0x00;
const static uint8_t SUBTYPE_ECHO_REPLY = 0x00;

const static uint16_t ICMP_HEADER_LENGTH = 4;

#define ICMP_JSON_TYPE "icmptype"
#define ICMP_JSON_CODE "icmpcode"
#define ICMP_JSON_CHECKSUM "icmpchecksum"
#define ICMP_JSON_ID "identifier"
#define ICMP_JSON_SEQ "sequence"
#define ICMP_JSON_PAYLOAD "icmp"
#define ICMP_JSON_PAYLOAD_SIZE "payloadsize"

class CAW_OS_EXPORT ICMPPacket : public IPPacket {
public:
    ICMPPacket();
    virtual ~ICMPPacket();
    uint8_t getIcmpType();
    void setIcmpType(uint8_t icmptype);
    uint8_t getIcmpCode();
    void setIcmpCode(uint8_t icmpcode);
    uint16_t getChecksum();
    void setChecksum(uint16_t checksum);
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;
    virtual void Dump() const;
    void SetIdentifier(uint16_t id);
    uint16_t GetIdentifier();
    void SetSequencenum(uint16_t seq);
    uint16_t GetSequencenum();
    uint16_t ICMPCheckSum();
    uint16_t icmp_checksum(uint16_t *buffer,int size);
    size_t SetAppPayload(char *data, size_t datasize);
    void GetAppPayload(char *&payload, size_t &payloadsize);

private:
    uint8_t m_icmpType;
    uint8_t m_icmpCode;
    uint16_t m_icmpchecksum;
    uint16_t m_icmpidentifier;
    uint16_t m_icmpsequencnum;
    char m_etherpayload[ETHERNET_MAX_SIZE];
    size_t m_etherpayloadsize;
};


#endif
