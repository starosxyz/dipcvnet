/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/checksum.h>

uint16_t checksum(uint16_t *buffer,int size)
{
    uint16_t cksum=0;
    while(size>1)
    {
        cksum+=*buffer++;
        size-=sizeof(uint16_t);
    }
    if(size)
    {
        cksum+=*(char *)buffer;
    }
    cksum=(cksum>>16)+(cksum&0xffff);
    cksum+=(cksum>>16);
    return (uint16_t)(~cksum);
}

