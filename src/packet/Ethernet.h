/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/

#ifndef _ETHERNET_H
#define _ETHERNET_H
#include <packet/IPacketParser.h>
#include <starbase/CAWDefines.h>
using namespace starbase;

#define CETHERNET_JSON_DMAC "dmac"
#define CETHERNET_JSON_SMAC "smac"
#define CETHERNET_JSON_ETHTYPE "ethtype"

#define CETHERNET_JSON_VLANID "vlanid"

#define CETHERNET_JSON_PRIORITY "priority"
#define CETHERNET_JSON_QINQ_PRIORITY "qinqpriority"
#define CETHERNET_JSON_QINQ_VID "qinqvid"
#define CETHERNET_JSON_QINQ_TPID "qinqtpid"
#define CETHERNET_JSON_PAYLOAD "ethernet"
#define ETHERNET_MAX_SIZE 1500
#define ETHERNET_MAC_LENGTH 6

/*
  @类名:CEthernet
  @功能:对以太协议的管理
*/

class CAW_OS_EXPORT CEthernet : public IPacketParser
{
public:
    enum packet_type {
        /* Immutable messages. */
        ETHERNET_ARP                    = 0, 
        ETHERNET_UDP                    = 1, 
        ETHERNET_TCP                    = 2,
        ETHERNET_INVALID                = 0xff
    };

    CEthernet();
    CEthernet(uint16_t ethtype);
    virtual ~CEthernet();
    virtual CAWResult StreamFrom(CAWByteStreamNetwork& is);
    virtual CAWResult StreamTo(CAWByteStreamNetwork& os);
    virtual CAWResult FromJson(Json::Value& jsonin);
    virtual CAWResult ToJson(Json::Value& jsonout);
    virtual uint16_t GetStreamLen() const;
    virtual uint16_t GetEncodeSpace()   const;

    virtual void Dump() const;
    void SetDMac(char mac[6]);
    void SetSMac(char mac[6]);
    void GetDMac(char mac[6]);
    void GetSMac(char mac[6]);
    void SetEthType(uint16_t type);
    uint16_t GetEthType();
    uint16_t GetQinQTPID();
    uint16_t GetQinQVID();
    uint16_t GetVlanId();
    uint8_t GetQinQPriorityCode();
    uint8_t GetPriorityCode();

    void SetQinQTPID(uint16_t qingqtpid);
    void SetQinQVID(uint16_t qinqvid);
    void SetVlanId(uint16_t vlanid);
    void SetQinQPriorityCode(uint8_t code);
    void SetPriorityCode(uint8_t code);
    static CAWResult DecodeEthernet(const char *pdata, size_t datasize, CAWAutoPtr<CEthernet> &pethernet);
    static CAWResult DecodeEthernet(CAWMessageBlock &msgblock, CAWAutoPtr<CEthernet>& pethernet);
    static CAWResult DecodeEthernetFromJson(Json::Value& jsonvalue, CAWAutoPtr<CEthernet> &aEthernet);
    CEthernet(const CEthernet& right);
    CEthernet& operator=(const CEthernet& right);
    static void VerifyEthernetdata(const char *pmsg, size_t msgsize, CAWAutoPtr<CEthernet> ethernet);

    static void Dump(CAWAutoPtr<CEthernet> ethernet);
protected:
    char m_dmac[6];
    char m_smac[6];
    uint16_t m_ethtype;
    uint8_t m_priorityCode;
    uint8_t m_qInQPriorityCode;
    uint16_t m_vlanID;
    uint16_t m_qinqVID;
    uint16_t m_qinqTPID;
};

#endif //_ETHERNET_H

























