/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#ifndef PACKECHECKSUM_H
#define PACKECHECKSUM_H
#include <packet/Ethernet.h>

CAW_OS_EXPORT uint16_t checksum(uint16_t *buffer,int size);


#endif

