/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _IPPACKET_H
#define _IPPACKET_H
#include <packet/Ethernet.h>


#define IPPACKET_JSON_VERSION "version"
#define IPPACKET_JSON_HEADERLENGTH "headerLength"
#define IPPACKET_JSON_DIFFSERV "diffserv"
#define IPPACKET_JSON_TOTALLENGTH "totallength"
#define IPPACKET_JSON_IDENTIFICATION "identification"
#define IPPACKET_JSON_FLAGS "flags"
#define IPPACKET_JSON_FRAGMENTOFFSET "fragmentoffset"
#define IPPACKET_JSON_TTL "ttl"
#define IPPACKET_JSON_PROTOCOL "protocol"
#define IPPACKET_JSON_CHECKSUM "ipchecksum"
#define IPPACKET_JSON_SOURCEADDRESS "sourceaddress"
#define IPPACKET_JSON_DESTIONADDRESS "destinationaddress"

#define IPPACKET_JSON_ETH_PAYLOAD "ip"

static const uint8_t PROTOCOL_ICMP = 0x1;
static const uint8_t PROTOCOL_IGMP = 0x2;
static const uint8_t PROTOCOL_TCP = 0x6;
static const uint8_t PROTOCOL_UDP = 0x11;
static const uint8_t PROTOCOL_PIM = 0x67;
static const uint8_t DSCP_MASK = 0x3f;
static const uint8_t DSCP_OFFSET = 2;
static const uint8_t ECN_MASK = 0x3;
static const uint16_t HEADER_LENGTH = 20;

class CAW_OS_EXPORT IPPacket : public CEthernet
{
public:
    IPPacket();
    IPPacket(uint8_t protocol);
    IPPacket(const IPPacket& right);
    IPPacket& operator=(const IPPacket& right);
    virtual ~IPPacket();
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;

    virtual void Dump() const;
    uint16_t CheckSum();

    uint8_t getVersion();
    void setVersion(uint8_t ver);
    uint8_t getHeaderLength();
    uint8_t getDscp();
    void setDscp(uint8_t dscp);
    uint8_t getEcn();
    void setEcn(uint8_t ecn);
    uint8_t getDiffServ();
    void setDiffServ(uint8_t diffS);
    uint16_t getTotalLength();
    uint16_t getIdentification();
    bool isTruncated();
    void setTruncated(bool isTruncated);
    uint8_t getFlags();
    void setFlags(uint8_t f);
    uint16_t getFragmentOffset();
    void setFragmentOffset(uint16_t frag);
    uint8_t getTtl();
    void setTtl(uint8_t ttlvalue);
    uint8_t getProtocol();
    void setProtocol(uint8_t protocolvlaue);
    uint16_t getChecksum();
    void setChecksum(uint16_t sum);
    void resetChecksum();
    uint32_t getSourceAddress();
    void setSourceAddress(uint32_t sourceaddr);
    uint32_t getDestinationAddress();
    void setDestinationAddress(uint32_t sourceaddr);
    void SetOption(char *data, size_t datasize);
    void GetOption(char *&data, size_t &datasize);
    size_t GetOptionSize(){return m_optionsLength;}
public:
    uint8_t m_version;
    uint8_t m_headerLength;
    uint8_t m_diffServ;
    uint16_t m_totalLength;
    uint16_t m_identification;
    uint8_t m_flags;
    uint16_t m_fragmentOffset;
    uint8_t m_ttl;
    uint8_t m_protocol;
    uint16_t m_checksum;
    uint32_t m_sourceAddress;
    uint32_t m_destinationAddress;
    size_t m_optionsLength;
    char *m_options;
    bool m_isTruncated;
};


#endif //_ETHERNET_H

