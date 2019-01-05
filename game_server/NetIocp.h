#ifndef _NET_IOCP_H
#define _NET_IOCP_H
#include <WinSock2.h>

#include <windows.h>

#define  gs_new new

class IocpImpl;
struct iocp_data;
typedef int (*pFunc)(int, int);
struct iocp_item
{
    pFunc func;
};
class NetIocp
{
public:
    NetIocp();
    ~NetIocp();
    
    int set_callback(iocp_item& item);

    int StartServer();

	int DoIocpWork();

private:

	int PostAcceptEvent();

	int PostRecvEvent(iocp_data * pdata);

private:
    int InitSocket();

    int CleanSocket();

	int PrintData(const char* data, int len);

private:
	iocp_item m_iocp_cbs;

	HANDLE m_iocp;
	SOCKET m_listen_socket;
};

#endif