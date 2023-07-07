#ifndef CDIPC_VNET_TYPE_H_
#define CDIPC_VNET_TYPE_H_

#include <string>
#include <starbase/CAWString.h>
using namespace starbase;

enum class DIPCVNetType {
  UNKNOWN,
  PPP,
  MPPP,
  VSWITCH,
  VOS
};

CAWString VNetToString(DIPCVNetType type);
DIPCVNetType StringToVNet(const CAWString &type);

#endif  // CDIPC_VNET_TYPE_H_
