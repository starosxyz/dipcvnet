/***********************************************************************
    Copyright (C) 2018-2023 南京北星极网络科技有限公司
**********************************************************************/
#ifndef VNETPDU_H
#define VNETPDU_H
#include "dipcutils/DIPCPdu.h"


#define DIPC_VNET_DATA						    (uint32_t)(0)
#define DIPC_VNET_KEEPALIVE_REQUEST			    (uint32_t)(1)
#define DIPC_VNET_KEEPALIVE_RESPONSE		    (uint32_t)(2)
#define DIPC_VNET_DATA_KEEPALIVE_REQUET          (uint32_t)(3)
#define DIPC_VNET_DATA_KEEPALIVE_RESPONSE		(uint32_t)(4)

struct VNetPacketPDU
{
    struct DIPCPDUHead head;   
    uint32_t datasize;
	char data[0];
};

struct VNetKeepAlivePdu
{
    struct DIPCPDUHead head;   
    uint64_t timestamp;
};


#endif  //VNETPDU_H


