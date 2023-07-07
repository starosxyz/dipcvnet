/***********************************************************************************
* 版权所有 (C)2016-2021, 南京北星极网络科技有限公司。
*************************************************************************************/
#ifndef __MACADDRESS_H__
#define __MACADDRESS_H__
#include <starbase/CAWACEInclude.h>
#include <wface/CAWACEWrapper.h>
#include <stdint.h>
#include <string.h>
#ifdef CAW_LINUX
#include <net/if.h>
#endif
#include <sstream>
#include <iomanip>
#include <string>
using namespace starbase;
using namespace wface;
using namespace std;

class CAW_OS_EXPORT MACAddress {
    public:
        MACAddress();
        MACAddress(const char* address);
        MACAddress(const CAWString &address);
        MACAddress(const MACAddress &other);
        MACAddress(const uint8_t* data);
        
        MACAddress& operator=(const MACAddress &other);
        bool operator==(const MACAddress &other) const;
        bool operator!=(const MACAddress& other) const;
        void toArray(uint8_t* array) const;
        CAWString toString() const;
        static bool isMulticast(char macaddress[6]);
        
        static bool isBroadcast(char macaddress[6]);
        static bool isZero(char macaddresss[6]);
    private:    
        uint8_t data[6];
        void data_from_string(const CAWString &address);
};

#endif /* __MACADDRESS_H__ */



