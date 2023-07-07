#ifndef DIPCVNETUTIL_H
#define DIPCVNETUTIL_H

#include <dipc/dipc.h>
using namespace dipc;


CAW_OS_EXPORT void ConvertKey(const CDIPCAddress& addr,DIPCAppKey* pkey);
CAW_OS_EXPORT uint64_t GetCurrentMs();
CAW_OS_EXPORT CAWString GetDIPCVNETHome();

#endif
