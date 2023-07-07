/***********************************************************************************
* 版权所有 (C)2016-2021, 南京北星极网络科技有限公司。
*************************************************************************************/
#ifndef MACHASH_FUN_HH
#define MACHASH_FUN_HH
#include <starbase/CAWACEInclude.h>
#include <wface/CAWACEWrapper.h>
using namespace starbase;
using namespace wface;

struct MacKey
{
    uint8_t dmac[6];
    uint16_t vlanid;
	MacKey() {
		::memset(this, 0, sizeof(*this));
	}
    MacKey& operator = (const MacKey& aRight)
    {
		::memset(this, 0, sizeof(*this));
        ::memcpy(dmac, aRight.dmac, 6);
        vlanid=aRight.vlanid;
        return *this;
    }
    bool operator == (const MacKey& aRight) const
    {
        return ((::memcmp(dmac, aRight.dmac, 6) == 0) && (vlanid == aRight.vlanid ));
    }
    bool operator != (const MacKey& aRight) const
    {
        return !(operator==(aRight));
    }
};
struct MacKeyHashFun
{
    size_t operator()(const MacKey& addr) const
    {
        size_t hashvalue = hash_bytes(&addr, sizeof(addr), 0);
        return hashvalue;
    }
};

#endif//MACHASH_FUN_HH
