#include "CDIPCVNetConfig.h"
#include "CDIPCVNetProcess.h"
#include "dipc/dipcclient.h"
#include "dipc/IDIPCCli.h"
#include "dipc/IDIPCSYSLOG.h"
#include "json/jsoncpp.h"
#include "dipc/IDIPCWebClient.h"
#include "wface/CAWACEWrapper.h"
#include "dipcutils/CDIPCAddress.h"
#include "dipcutils/CDIPCConfig.h"
#include <dipcutils/DIPCLogger.h>
#include "utils/DIPCVNetDebug.h"
#include "fstack.h"

using namespace dipc::dipcclient;
using namespace dipc::utils;
using namespace dipc;
using namespace std;

int dipc_vnet_loop(void* arg)
{
    /* Wait for events to happen */
    DIPCPollWithoutWait();
    return 0;
}

int main(int argc, char** argv)
{

   
    CDIPCVNetProcess *process= new CDIPCVNetProcess();
 
    CDIPCNodeAddress mylocation(CDipcVnetConfig::Instance()->GetDipcZone());
    int DIPCVNet_JNO = CDipcVnetConfig::Instance()->GetDipcJno();
    CAWInetAddr dipcserver(CDipcVnetConfig::Instance()->GetDipcIp());
    const char* hostname = CDipcVnetConfig::Instance()->GetDipcHostname().c_str();
    const char* password = CDipcVnetConfig::Instance()->GetDipcPassword().c_str();

    if (process == NULL)
    {
        return -1;
    }

    if (process->InitVNet() != CAW_OK)
    {
        delete process;
        dipcvnet_printf("main init vnet failure\n");
        return -1;
    }

    DIPCProcessInit(argc, argv);
    CAWResult ret = DIPCProcessConnectToDIPCService(process,mylocation,DIPCVNet_JNO, 
                                            dipcserver,hostname,password,
                                            DIPCAppServerType::DIPC_APPSERVER_TCP);
    if( ret != CAW_OK)
    {
        std::cerr<<"\n###DIPCProcessConnectToDIPCService Failed ret="<<ret<<"\n\n\n"<<std::endl;
    }

    //DIPCPollLoop();
    fs_init(argc, argv);

    printf("run main thread\n");
    fs_run(dipc_vnet_loop, NULL);


    /* cleanup */
    if (process)
    {
        delete process;
    }
    return 0;
}

