#ifndef CWHITELIST_H
#define CWHITELIST_H

#include "dipc/dipc.h"
#include "DIPCVNetError.h"

typedef std::list<uint16_t> CWHITELISTType;

class CWhiteList
{
public:
    CWhiteList();
    ~CWhiteList();
public:
    CAWResult Init();
    CAWResult Veryfy(uint16_t unit);
    CAWResult AddList(uint16_t unit);
    CAWResult RemoveList(uint16_t unit);
private:
    CWHITELISTType m_white_list;
};

#endif //CWHITELIST_H
