#ifndef CDIPC_VNET_POLICY_TYPE_H_
#define CDIPC_VNET_POLICY_TYPE_H_

#include <string>
#include <starbase/CAWString.h>
using namespace starbase;

enum class DIPCVNetPolicyType {
  UNKNOWN,
  CLONE,
  LOADBALANCE,
  AGG,
  ADPATER,
  AUTO,
  NONE
};

CAWString VNetPolicyToString(DIPCVNetPolicyType type);
DIPCVNetPolicyType StringToVNetPolicy(const CAWString &type);

#endif  // CDIPC_VNET_POLICY_TYPE_H_
