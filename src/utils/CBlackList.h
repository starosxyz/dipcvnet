#ifndef CBLACKLIST_H
#define CBLACKLIST_H

#include "dipc/dipc.h"
#include "DIPCVNetError.h"

typedef std::list<uint16_t> CBLACKLISTType;

class CBlackList
{
public:
    CBlackList();
    ~CBlackList();

public:
    CAWResult Init();
    CAWResult Veryfy(uint16_t unit);
    CAWResult AddList(uint16_t unit);
    CAWResult RemoveList(uint16_t unit);
private:
    CBLACKLISTType m_black_list;
};


#endif//CBLACKLIST_H