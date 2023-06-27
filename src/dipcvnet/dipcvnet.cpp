#include "CDIPCVNetConfig.h"
#include "CDIPCVNetProcess.h"
//#include "CDIPCVnetManager.h"

#include "dipc/dipcclient.h"
#include "dipc/IDIPCCli.h"
#include "dipc/IDIPCSYSLOG.h"
#include "json/jsoncpp.h"
#include "dipc/IDIPCWebClient.h"
#include "wface/CAWACEWrapper.h"
#include "dipcutils/CDIPCAddress.h"
#include "dipcutils/CDIPCConfig.h"
#include <dipcutils/DIPCLogger.h>


using namespace dipc::dipcclient;
using namespace dipc::utils;
using namespace dipc;
using namespace std;


int main(int argc, char** argv)
{

   
    CDIPCVNetProcess *process= new CDIPCVNetProcess();
 
    CDIPCNodeAddress mylocation(CDipcVnetConfig::Instance()->GetDipcZone());
    CAWInetAddr dipcserver(CDipcVnetConfig::Instance()->GetDipcIp());
    const char* hostname = CDipcVnetConfig::Instance()->GetDipcHostname().c_str();
    const char* password = CDipcVnetConfig::Instance()->GetDipcPassword().c_str();

    DIPCProcessInit(argc, argv);
    CAWResult ret = DIPCProcessConnectToDIPCService(process,mylocation,DIPCVNet_JNO, 
                                            dipcserver,hostname,password,
                                            DIPCAppServerType::DIPC_APPSERVER_TCP);
    if( ret != CAW_OK)
    {
        std::cerr<<"\n###DIPCProcessConnectToDIPCService Failed ret="<<ret<<"\n\n\n"<<std::endl;
    }

    DIPCPollLoop();

    /* cleanup */
    if (process)
    {
        delete process;
    }
    return 0;
}

