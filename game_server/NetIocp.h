#ifndef _NET_IOCP_H
#define _NET_IOCP_H
#include "PlatForm.h"

#define  gs_new new


class SessionMgr;
struct iocp_data;
class SW_Thread;
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


    static void NetThread(void * args);
private:
	iocp_item m_iocp_cbs;

	HANDLE m_iocp;
	SOCKET m_listen_socket;

	SessionMgr * m_session_mgr;
    SW_Thread * m_worker_thread;
};

#endif