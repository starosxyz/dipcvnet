/***********************************************************************************
*           版权所有 (C)2016-2021, 南京北星极网络科技有限公司
*************************************************************************************/
#include "utils/MACAddress.h"

char kboradcast_mac[6]={(char)0xff,(char)0xff,(char)0xff,(char)0xff,(char)0xff,(char)0xff};
char zero_mac[6]={0};
MACAddress::MACAddress() {::memset((char *)data,0,6);}

MACAddress::MACAddress(const char* address) {
    ::memset((char *)data,0, 6);

    CAWString saddress(address);
    data_from_string(saddress);
}

MACAddress::MACAddress(const CAWString &address) {
    ::memset((char *)data,0, 6);

    data_from_string(address);
}

MACAddress::MACAddress(const uint8_t* ddata) {
    ::memset((char *)this->data,0, 6);

    memcpy(this->data, ddata, 6);
}

MACAddress::MACAddress(const MACAddress &other) {
    ::memset((char *)data,0, 6);

    other.toArray(this->data);
}

MACAddress& MACAddress::operator=(const MACAddress &other) {
    if (this != &other) {
        other.toArray(this->data);
    }
    return *this;
}

bool MACAddress::operator==(const MACAddress &other) const {
    return memcmp(other.data, this->data, 6) == 0;
}

bool MACAddress::operator!=(const MACAddress& other) const {
    return !(MACAddress::operator==(other));
}
void MACAddress::toArray(uint8_t* array) const {
    memcpy(array, this->data, 6);
}
        
CAWString MACAddress::toString() const {
    stringstream ss;
    ss << hex << setfill('0');
    for (int i = 0; i < 6; i++) {
        ss <<  setw(2) << (int) data[i]; 
        if (i < 6 - 1)
            ss << ':';
    }
        
    return ss.str().c_str();
}

void MACAddress::data_from_string(const CAWString &address) {
    char sc;
    int byte;
    std::string straddress = address.c_str();
    stringstream ss(straddress);
    ss << hex;
    for (int i = 0; i < 6; i++) {
        ss >> byte;
        ss >> sc;
        this->data[i] = (uint8_t) byte;
    }
}

bool MACAddress::isMulticast(char macaddress[6]) {

    return (macaddress[0] & 0x01) != 0;
}

bool MACAddress::isBroadcast(char macaddress[6]) {

    if (::memcmp(macaddress,kboradcast_mac,6)==0)
    {
        return true;
    }
    else 
    {
        return false;
    }
}
bool MACAddress::isZero(char macaddresss[6])
{
    if (::memcmp(macaddresss,zero_mac,6)==0)
    {
        return true;
    }
    else 
    {
        return false;
    }   
}

