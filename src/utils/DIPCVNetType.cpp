#include "utils/DIPCVNetType.h"

#include <string>

CAWString VNetToString(DIPCVNetType type){
  switch (type) {
    case DIPCVNetType::PPP:
      return "ppp";
    case DIPCVNetType::MPPP:
      return "mppp";
    case DIPCVNetType::VSWITCH:
      return "vswitch";
    case DIPCVNetType::VOS:
      return "vos";
    default:
      return "unknown";
  }
}
DIPCVNetType StringToVNet(const CAWString& type)
{
    if (type == "ppp")
        return DIPCVNetType::PPP;
    else if (type == "mppp")
        return DIPCVNetType::MPPP;
    else if (type == "vswitch")
        return DIPCVNetType::VSWITCH;
    else if (type == "vos")
        return DIPCVNetType::VOS;
    else
        return DIPCVNetType::UNKNOWN;
}

