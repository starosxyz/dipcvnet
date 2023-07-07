#include "dipcvnetutils.h"

void ConvertKey(const CDIPCAddress& addr,DIPCAppKey* pkey)
{
    pkey->m_did= addr.GetDataCenterId();
    pkey->m_cid=addr.GetClusterId();
    pkey->m_nodeid=addr.GetNodeId();
    pkey->m_jno=addr.GetJno();
    return ;
    CAW_INFO_TRACE("CDIPCVnetManager ConvertKey, GetDataCenterId= "<<addr.GetDataCenterId()
                <<" , GetClusterId= "<<addr.GetClusterId()
                <<" , GetNodeId= "<<addr.GetNodeId()
                <<" , GetJno= "<<addr.GetJno());
}

uint64_t GetCurrentMs()
{
    CAWTimeValue tv = CAWTimeValue::GetTimeOfDay();
    return tv.GetTotalInMsec();
}

CAWString GetDIPCVNETHome()
{
	CAWString str=VOS_GetEnv("DIPCVNET_ROOT");
    if (str.size()==0)
    {
        str="/opt/staros.xyz/dipcvnet";
    }
    
	return str;
}