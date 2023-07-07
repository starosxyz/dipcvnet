/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/TpPort.h>


TpPort::TpPort()
    :m_tpport(0)
{
}

TpPort::TpPort(uint16_t tpport)
    :m_tpport(tpport)
{
}
TpPort::~TpPort()
{
}

TpPort::TpPort(const TpPort &right)
{
    m_tpport=right.m_tpport;
}
TpPort& TpPort::operator=(const TpPort& right)
{
    if (&right != this)
    {
        m_tpport=right.m_tpport;
    }
    return *this;
}

bool TpPort::operator==(const TpPort& right) const
{
    return m_tpport == right.m_tpport;
}

bool TpPort::operator!=(const TpPort& right) const
{
    return !(*this == right);
}

uint16_t TpPort::toShort()
{
    return this.m_tpport;
}

CAWString TpPort::toString() 
{
    char buffer[1024]={0};
    snprintf(buffer,sizeof(buffer), "%d", m_tpport);
    return CAWString(buffer);
}


