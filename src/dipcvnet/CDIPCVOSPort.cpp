#include "CDIPCVOSPort.h"
#include "dipcvnet/vos/CDIPCVnetVOSSession.h"
int VOSPortOutput(struct vos_veth* veth, void* m)
{
    char buffer[1024 * 2];

    void* data = (void*)buffer;
    int len = vos_mbuf_getpktlen(m);
    int ret = vos_mbuf_copydata(m, data, 0, len);
    if (ret < 0) {
        vos_mbuf_free(m);
        return -1;
    }

	CDIPCVOSPort *pport= (CDIPCVOSPort*)vos_veth_get_cookie(veth);
	if (pport)
	{
		pport->Output((char*)data,len);
	}
    return 0;
}

CDIPCVOSPort::CDIPCVOSPort()
	:m_if(NULL)
	,m_psession(NULL)
{
}
CDIPCVOSPort::~CDIPCVOSPort()
{
    if (m_if)
    {
        destroy_vos_veth(m_if);
        m_if = NULL;
    }
}
CAWResult CDIPCVOSPort::Init(CDIPCVnetVOSSession* psession, const CAWString& ifname)
{

    m_psession = psession;
    vos_in_addr_t ipaddr = 0;
    vos_in_addr_t netmask = 0;
    vos_in_addr_t broadcast = 0;
    vos_in_addr_t gw = 0;

    char macaddr[VOS_MAC_LEN] = { 0 };

    m_ifname = ifname;

    m_if = create_vos_veth(&ipaddr, &netmask, &broadcast, &gw, macaddr, (char*)m_ifname.c_str(), VOSPortOutput, this);
	if (m_if ==NULL)
	{
		return CAW_ERROR_FAILURE;
	}
	
	return CAW_OK;
}
void CDIPCVOSPort::Input(const char* data, size_t datasize)
{
    if (m_if)
    {
        vos_veth_input(m_if, data, datasize, 0);
    }
}
void CDIPCVOSPort::Output(const char* data, size_t datasize)
{
	/*use session send */
	if (m_psession)
	{
		m_psession->LowlayerSendPacket(data,datasize);
	}
}