/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/Ethernet.h>
#include <packet/ARPPacket.h>
#include <packet/ICMPPacket.h>
#include <packet/UDPPacket.h>
#include <packet/TCPPacket.h>
#include <packet/DSPacket.h>
#include <packet/CLLDP.h>
#include <packet/UnknownEthernet.h>
#include <packet/UnknownIPPacket.h>
#include <packet/UnknownTCPPacket.h>
#include <packet/UnknownUDPPacket.h>
#include <packet/LinkDataPacket.h>
#include <packet/STPPacket.h>


CEthernet::CEthernet()
{
    ::memset(m_dmac,0,sizeof(m_dmac));
    ::memset(m_smac,0,sizeof(m_smac));
    m_ethtype = 0;

    m_priorityCode=0;
    m_qInQPriorityCode=0;
    m_vlanID=0xffff;
    m_qinqVID=0xffff;
    m_qinqTPID=0;
}
CEthernet::CEthernet(uint16_t ethtype)
{
    ::memset(m_dmac,0,sizeof(m_dmac));
    ::memset(m_smac,0,sizeof(m_smac));
    m_ethtype =ethtype;

    m_priorityCode=0;
    m_qInQPriorityCode=0;
    m_vlanID=0xffff;
    m_qinqVID=0xffff;
    m_qinqTPID=0;   
}

CEthernet::~CEthernet()
{
}


CAWResult CEthernet::StreamFrom(CAWByteStreamNetwork& is)
{
    is.Read(m_dmac,6);
    is.Read(m_smac,6);
    is>>m_ethtype;

    if (m_ethtype == 0x88a8)
    {
        uint16_t tci=0;
        is>>tci;
        m_qInQPriorityCode = (uint8_t) (tci >> 13 & 0x07);
        m_qinqVID = (uint16_t) (tci & 0x0fff);
        m_qinqTPID = 0x88a8;
        is>>m_ethtype;
    }

    if (m_ethtype == 0x8100)
    {
        uint16_t tci=0;
        is>>tci;  
        m_priorityCode = (uint8_t) (tci >> 13 & 0x07);
        m_vlanID = (short) (tci & 0x0fff);
        is>>m_ethtype;
        if (m_ethtype == 0x8100)
        {
            m_qInQPriorityCode = m_priorityCode;
            m_qinqVID = m_vlanID;
            m_qinqTPID = 0x8100;
            uint16_t innerTci =0;
            is>>innerTci;
            m_priorityCode = (uint8_t) (innerTci >> 13 & 0x07);
            m_vlanID = (uint16_t) (innerTci & 0x0fff);
            is>>m_ethtype;
        }
    }
    else 
    {
        m_vlanID = 0xffff; 
    }
    bool bGood = is.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    
    return CAW_OK;

}

CAWResult CEthernet::StreamTo(CAWByteStreamNetwork& os)
{
    os.Write(m_dmac,6);
    os.Write(m_smac,6);
    if (m_qinqVID!=0xffff)
    {
        os<<m_qinqTPID;
        uint16_t type=((uint16_t) (m_qInQPriorityCode << 13 | m_qinqVID & 0x0fff));
        os<<type;
    }
    if (m_vlanID != 0xffff)
    {
        uint16_t type=0x8100;
        uint16_t vlan=((uint16_t) (m_priorityCode << 13 | m_vlanID & 0x0fff));
        os<<type;
        os<<vlan;
    }

    os<<m_ethtype;
    
    bool bGood = os.IsGood();
    CAW_ASSERTE_RETURN(bGood, CAW_ERROR_FAILURE);
    
    return CAW_OK;

}


CAWResult CEthernet::FromJson(Json::Value& jsonin)
{

    CAW_INFO_TRACE_THIS("CEthernet::FromJson");

    if (!jsonin[CETHERNET_JSON_PAYLOAD].isNull())
    {
        Json::Value &payloadjsonin = jsonin[CETHERNET_JSON_PAYLOAD];

        MacAddressFromJson(payloadjsonin, CETHERNET_JSON_DMAC, (uint8_t*)m_dmac);
        MacAddressFromJson(payloadjsonin, CETHERNET_JSON_SMAC, (uint8_t*)m_smac);
        ValueFromJson(payloadjsonin, CETHERNET_JSON_ETHTYPE, m_ethtype);
        ValueFromJson(payloadjsonin, CETHERNET_JSON_VLANID, m_vlanID);
        ValueFromJson(payloadjsonin, CETHERNET_JSON_PRIORITY, m_priorityCode);
        ValueFromJson(payloadjsonin, CETHERNET_JSON_QINQ_PRIORITY, m_qInQPriorityCode);
        ValueFromJson(payloadjsonin, CETHERNET_JSON_QINQ_VID, m_qinqVID);
        ValueFromJson(payloadjsonin, CETHERNET_JSON_QINQ_TPID, m_qinqTPID);
    }

    
    return CAW_OK;
}

CAWResult CEthernet::ToJson(Json::Value& jsonout)
{

    CAW_INFO_TRACE_THIS("CEthernet::ToJson");
    Json::Value injsonout;
    injsonout[CETHERNET_JSON_DMAC]=MacAddressToJson((uint8_t*)m_dmac); 
    injsonout[CETHERNET_JSON_SMAC]=MacAddressToJson((uint8_t*)m_smac);
    injsonout[CETHERNET_JSON_ETHTYPE]=ValueToJson(m_ethtype);
    injsonout[CETHERNET_JSON_VLANID]=ValueToJson(m_vlanID);
    injsonout[CETHERNET_JSON_PRIORITY]=ValueToJson(m_priorityCode);
    injsonout[CETHERNET_JSON_QINQ_PRIORITY]=ValueToJson(m_qInQPriorityCode);
    injsonout[CETHERNET_JSON_QINQ_VID]=ValueToJson(m_qinqVID);
    injsonout[CETHERNET_JSON_QINQ_TPID]=ValueToJson(m_qinqTPID);

    jsonout[CETHERNET_JSON_PAYLOAD]=injsonout;

    return CAW_OK;
}


uint16_t CEthernet::GetStreamLen() const
{ 
    return 14+(m_vlanID == 0xffff ? 0 : 4)+(m_qinqVID == 0xffff? 0 : 4);
}

uint16_t CEthernet::GetEncodeSpace()   const
{
    return 14+(m_vlanID == 0xffff ? 0 : 4)+(m_qinqVID == 0xffff? 0 : 4);
}

void CEthernet::Dump() const
{

}
void CEthernet::SetDMac(char mac[6])
{
    ::memcpy(m_dmac,mac,6);
}

void CEthernet::SetSMac(char mac[6])
{
    ::memcpy(m_smac,mac,6);
}
void CEthernet::GetDMac(char mac[6])
{
    ::memcpy(mac,m_dmac,6);    
}

void CEthernet::GetSMac(char mac[6])
{
    ::memcpy(mac,m_smac,6);
}
void CEthernet::SetEthType(uint16_t type)
{
    m_ethtype = type;
}

uint16_t CEthernet::GetEthType()
{
    return m_ethtype;
}


uint16_t CEthernet::GetQinQTPID()
{
    return m_qinqTPID;
}
uint16_t CEthernet::GetQinQVID()
{
    return m_qinqVID;
}
uint16_t CEthernet::GetVlanId()
{
    return m_vlanID;
}
uint8_t CEthernet::GetQinQPriorityCode()
{
    return m_qInQPriorityCode;
}
uint8_t CEthernet::GetPriorityCode()
{
    return m_priorityCode;
}

void CEthernet::SetQinQTPID(uint16_t qingqtpid)
{
    m_qinqTPID=qingqtpid;
}
void CEthernet::SetQinQVID(uint16_t qinqvid)
{
    m_qinqVID=qinqvid;
}
void CEthernet::SetVlanId(uint16_t vlanid)
{
    m_vlanID=vlanid;
}
void CEthernet::SetQinQPriorityCode(uint8_t code)
{
    m_qInQPriorityCode=code;
}
void CEthernet::SetPriorityCode(uint8_t code)
{
    m_priorityCode=code;
}

CAWResult CEthernet::DecodeEthernet(CAWMessageBlock& aMB, CAWAutoPtr<CEthernet>& pethernet)
{
    size_t datasize = aMB.GetChainedLength();
    char* pdata = (char *)aMB.GetTopLevelReadPtr();

    return DecodeEthernet(pdata,datasize,pethernet);
}
CAWResult CEthernet::DecodeEthernet(const char *pdata, size_t datasize, CAWAutoPtr<CEthernet> &pethernet)
{   

    CAWMessageBlock aMB(datasize, 
        pdata, 
        CAWMessageBlock::DONT_DELETE, 
        datasize);
    
    CAWByteStreamNetwork etheris(aMB);

    CAWAutoPtr<CEthernet> pMessage;

    uint16_t etype = *(uint16_t *)(pdata+12);
    etype = ntohs(etype);
    uint8_t protocoltype=0;
    if (etype == 0x0806)
    {
        pMessage = new ARPPacket();
    }
    else if (etype == 0x9944)
    {
        pMessage = new DSPacket();
    }
    else if (etype == 0x9945)
    {
        pMessage = new LinkDataPacket();
    }
    else if (etype == 0x9946)
    {
        pMessage = new STPPacket();
    }
    else if (etype == 0x88CC)
    {
        pMessage = new CLLDP();
    }
    else if (etype == 0x0800)
    {
        protocoltype = *(uint8_t *)(pdata+23);
        if (protocoltype == 6)
        {
            pMessage = new UnknownTCPPacket();
        }
        else if (protocoltype == 17)
        {
            pMessage = new UnknownUDPPacket();
        }
        else if (protocoltype == 1)
        {
            pMessage = new ICMPPacket();
        }
        else 
        {
            pMessage = new UnknownIPPacket();
        }
    }
    else if (etype == 0x8100)
    {
        uint16_t suetype = *(uint16_t *)(pdata+12+4);
        suetype = ntohs(suetype);
        if (suetype == 0x8100)
        {
            suetype = *(uint16_t *)(pdata+12+4+4);
            suetype = ntohs(suetype);
            if (suetype == 0x0806)
            {
                pMessage = new ARPPacket(); 
            }
            else if (suetype == 0x9944)
            {
                pMessage = new DSPacket();
            }
            else if (suetype == 0x9945)
            {
                pMessage = new LinkDataPacket();
            }
            else if (suetype == 0x9946)
            {
                pMessage = new STPPacket();
            }
            else if (suetype == 0x0800)
            {
                uint8_t protocoltype = *(uint8_t *)(pdata+23);
                if (protocoltype == 6)
                {
                    pMessage = new UnknownTCPPacket();
                }
                else if (protocoltype == 17)
                {
                    pMessage = new UnknownUDPPacket();
                }
                else if (protocoltype == 1)
                {
                    pMessage = new ICMPPacket();
                }
                else 
                {
                    pMessage = new UnknownIPPacket();
                }

            }
            else 
            {
                pMessage = new UnknownEthernet();
            }
        }
        else if (suetype == 0x0806)
        {
            pMessage = new ARPPacket();
        }
        else if (suetype == 0x9944)
        {
            pMessage = new DSPacket();
        }
        else if (suetype == 0x9945)
        {
            pMessage = new LinkDataPacket();
        }
        else if (suetype == 0x9946)
        {
            pMessage = new STPPacket();
        }
        else if (suetype == 0x0800)
        {
            protocoltype = *(uint8_t *)(pdata+23);
            if (protocoltype == 6)
            {
                pMessage = new UnknownTCPPacket();
            }
            else if (protocoltype == 17)
            {
                pMessage = new UnknownUDPPacket();
            }
            else if (protocoltype == 1)
            {
                pMessage = new ICMPPacket();
            }
            else 
            {
                pMessage = new UnknownIPPacket();
            }

        }
        else 
        {
            pMessage = new UnknownEthernet();
        }
        
    }
    else 
    {
        pMessage = new UnknownEthernet();
    }
    if (pMessage.Get())
    {

        CAWResult nResult = pMessage->StreamFrom(etheris);
        if (CAW_FAILED(nResult)) {
            CAW_ERROR_TRACE("CEthernet::DecodeEthernet,"" fail in pdu->StreamFrom(), nResult=" << nResult);
            return nResult;
        }

        pethernet = pMessage;      

    }
    else 
    {
        CAW_ERROR_TRACE("CEthernet::DecodeEthernet no message");
        return CAW_ERROR_FAILURE;
    }

#ifdef CAW_DEBUG
    VerifyEthernetdata(pdata,datasize,pMessage);
#endif
    
    return CAW_OK;
}
CAWResult CEthernet::DecodeEthernetFromJson(Json::Value& jsonvalue, CAWAutoPtr<CEthernet> &aEthernet)
{
    CAWResult lRet  = CAW_ERROR_FAILURE;
    uint16_t   wType = 0;
    //uint16_t   wLen  = 0;
    CAWAutoPtr<CEthernet> pAct;

    if (jsonvalue[CETHERNET_JSON_ETHTYPE].isNull())
    {
        return CAW_ERROR_FAILURE;
    }

    //wType == (uint16_t)jsonvalue[COFP13ACTION_JSON_TYPE].asUInt();

    ValueFromJson_S(jsonvalue, CETHERNET_JSON_ETHTYPE, wType);

    switch (wType)
    {
        case 0x0806:
        {
            pAct = new ARPPacket();
            break;
        }
        case 0x0800:
        {
            uint8_t protocoltype=0;

            if (!jsonvalue[CETHERNET_JSON_PAYLOAD].isNull())
            {
                Json::Value &ippayloadjson = jsonvalue[CETHERNET_JSON_PAYLOAD];
                ValueFromJson_S(ippayloadjson, IPPACKET_JSON_PROTOCOL, protocoltype);
             }

            if (protocoltype == 6)
            {
                pAct = new UnknownTCPPacket();
            }
            else if (protocoltype == 17)
            {
                pAct = new UnknownUDPPacket();
            }
            else if (protocoltype == 1)
            {
                pAct = new ICMPPacket();
            }
            else 
            {
                pAct = new UnknownIPPacket();
            }
            break;
        }
        default:
        {
            pAct = new UnknownEthernet();
            break;
        }
    }
    
    CAW_ASSERTE_RETURN(pAct, CAW_ERROR_OUT_OF_MEMORY);

    //pAct->m_wType = wType;
    //pAct->m_wLen  = wLen;
    lRet = pAct->FromJson(jsonvalue);
    if (CAW_FAILED(lRet))
    {
        CAW_ERROR_TRACE("COFP13Action::DecodeAction(): fail in StreamFrom(), lRet=" << lRet);
        return lRet;
    }

    aEthernet = pAct;
    
    return CAW_OK;   
}

CEthernet::CEthernet(const CEthernet &right)
{
    (*this) = right;
}

CEthernet &CEthernet::operator=(const CEthernet &right)
{
    if (this != &right)
    {
        ::memcpy(m_dmac, right.m_dmac, 6);
        ::memcpy(m_smac, right.m_smac, 6);
        m_ethtype = right.m_ethtype;
        m_priorityCode = right.m_priorityCode;
        m_qInQPriorityCode = right.m_qInQPriorityCode;
        m_vlanID = right.m_vlanID;
        m_qinqVID = right.m_qinqVID;
        m_qinqTPID = right.m_qinqTPID;
    }
    return *this;
}

void CEthernet::VerifyEthernetdata(const char *pmsg, size_t msgsize, CAWAutoPtr<CEthernet> ethernet)
{
    char *pdata = NULL;
    size_t datasize = 0; 
    WORD16 space = ethernet->GetEncodeSpace();

    CAWMessageBlock msgblk(space);

    CAWByteStreamNetwork os(msgblk);

    if (ethernet->StreamTo(os) != CAW_OK)
    {
        CAW_ASSERTE(0);
        return;
    }
    pdata=(char*)msgblk.GetTopLevelReadPtr();
    datasize=msgblk.GetChainedLength();
    //printf("datasize=%d, msgsize=%d, type=%x\n",datasize,msgsize,ethernet->GetEthType());

    if(ethernet->GetEthType() != 0x0806)
    {
        if (datasize > msgsize)
        {
            CAW_ASSERTE(0);
        }

        for(int i=0;i<datasize;i++)
        {
            if (pdata[i]!=pmsg[i])
            {
                printf("pdata=%d, pmsg=%d, i=%d\n",pdata[i],pmsg[i], i);

                CAW_ASSERTE(0);
            }
        }
        
        if(::memcmp(pdata,pmsg,datasize)!=0)
        {
            CAW_ASSERTE(0);
        }
    }
}


void CEthernet::Dump(CAWAutoPtr<CEthernet> ethernet)
{
    uint16_t ethtype = ethernet->GetEthType();

    if(ethtype == 0x0806)
    {
        ARPPacket* arppacket = dynamic_cast<ARPPacket*>(ethernet.Get());
        uint32_t targetip = arppacket->GetTargetProtocolAddress();
        uint32_t sendip = arppacket->GetSenderProtocolAddress();

        CAWInetAddr targetipaddr;
        CAWInetAddr sendipaddr;
        targetipaddr.SetIpAddrBy4Bytes(targetip, false);
        sendipaddr.SetIpAddrBy4Bytes(sendip, false);
        CAW_INFO_TRACE("CEthernet::Dump 0x0806 OpCode="<<arppacket->GetOpCode()<<", targetip = "<<targetipaddr.GetIpDisplayName()
            <<",sendip="<<sendipaddr.GetIpDisplayName());
    }
    else if(ethtype == 0x0800)
    {
        IPPacket* ippacket = dynamic_cast<IPPacket*>(ethernet.Get());
        uint32_t dstip = ippacket->getDestinationAddress();
        uint32_t srcip = ippacket->getSourceAddress();
        CAWInetAddr srcaddress;
        CAWInetAddr dstaddress;
        
        srcaddress.SetIpAddrBy4Bytes(srcip, false);
        dstaddress.SetIpAddrBy4Bytes(dstip, false);

        
        CAW_INFO_TRACE("CEthernet::Dump 0x0800,srcip="<<srcaddress.GetIpDisplayName()
            <<",dstip="<<dstaddress.GetIpDisplayName()); 

    }
    else 
    {
        return ;
    }

}


