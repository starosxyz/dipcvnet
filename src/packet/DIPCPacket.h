/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef _DIPCPACKET_H
#define _DIPCPACKET_H
#include <packet/Ethernet.h>

#define DIPC_DATA_MAX_SIZE 1400

struct DIPCLLCAddress
{
	uint16_t jno;
	uint16_t pno;
	uint32_t nodeid;
	uint32_t clusterid;
	uint32_t datacenterid;
};

class CAW_OS_EXPORT DIPCPacket : public CEthernet
{
public:
    DIPCPacket();
    virtual ~DIPCPacket();
    CAWResult StreamFrom(CAWByteStreamNetwork& is);
    CAWResult StreamTo(CAWByteStreamNetwork& os);
    CAWResult FromJson(Json::Value& jsonin);
    CAWResult ToJson(Json::Value& jsonout);
    uint16_t GetStreamLen() const;
    uint16_t GetEncodeSpace()   const;
    void Dump() const;

    inline uint16_t GetType() const { return m_dipctype; }
    inline void SetType(uint16_t type) { m_dipctype = type; }

    void SetPayload(const char *pdata, size_t datasize);
    void GetPlayload(char* &pdata, size_t &datasize);


    DIPCPacket(const DIPCPacket& right);

    DIPCPacket& operator=(const DIPCPacket& right);
    virtual DIPCPacket* ClonePdu();
    void DestroyClone();
public:
    struct DIPCLLCAddress m_dstaddr;
    struct DIPCLLCAddress m_srcaddr;
    uint16_t m_dipctype;
    uint16_t m_payloadsize;
    char* m_payload;
};

#endif //_DIPCPACKET_H
