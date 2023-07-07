#include "utils/DIPCVNetPolicyType.h"

#include <string>
#if 0
#1=clone; 2=loadbalance; 3=aggregation,4,adpater, 5,auto
#endif
  
CAWString VNetPolicyToString(DIPCVNetPolicyType type){
  switch (type) {
    case DIPCVNetPolicyType::CLONE:
      return "clone";
    case DIPCVNetPolicyType::LOADBALANCE:
      return "loadbalance";
    case DIPCVNetPolicyType::AGG:
      return "aggregation";
    case DIPCVNetPolicyType::ADPATER:
      return "adpater";
    case DIPCVNetPolicyType::AUTO:
      return "auto";
    case DIPCVNetPolicyType::NONE:
        return "none";
    default:
      return "unknown";
  }
}
DIPCVNetPolicyType StringToVNetPolicy(const CAWString& type)
{
    if (type == "clone")
        return DIPCVNetPolicyType::CLONE;
    else if (type == "loadbalance")
        return DIPCVNetPolicyType::LOADBALANCE;
    else if (type == "aggregation")
        return DIPCVNetPolicyType::AGG;
    else if (type == "adpater")
        return DIPCVNetPolicyType::ADPATER;
    else if (type == "auto")
        return DIPCVNetPolicyType::AUTO;
    else if (type == "none")
        return DIPCVNetPolicyType::NONE;
    else
        return DIPCVNetPolicyType::UNKNOWN;
}

