#ifndef _NET_IOCP_H
#define _NET_IOCP_H
#include "PlatForm.h"

#define  gs_new new

#include <list>
struct iocp_data;
class SW_Thread;
typedef int (* CLIENT_CONNECT_CB)(int player_id, int port, const char * ip);
typedef int (* CLIENT_DATA_COME_CB)(int, int);
typedef int (*CLIENT_LOST_CONNECT_CB)(int, int);
struct iocp_item
{
    CLIENT_CONNECT_CB connect_cb;
    CLIENT_DATA_COME_CB data_cb;
    CLIENT_LOST_CONNECT_CB lost_con_cb;

    iocp_item():connect_cb(NULL), data_cb(NULL), lost_con_cb(NULL)
    {}

    iocp_item & operator = (const iocp_item & rhs)
    {
        connect_cb = rhs.connect_cb;
        data_cb = rhs.data_cb;
        lost_con_cb = rhs.lost_con_cb;
        return *this;
    }
};

struct completion_key
{
    int player_id;
    int port;
    char client_ip[64];
    SOCKET client_socket;
    bool bConnect;
    completion_key():player_id(0),
                     port(0),
                     client_ip(""),
                     client_socket(INVALID_SOCKET),
                     bConnect(false)
    {}
	
};

typedef std::list<completion_key*> ListSession;
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

    int ClearInvalidSession();


    static void NetThread(void * args);
private:
	iocp_item m_iocp_cbs;

	HANDLE m_iocp;
	SOCKET m_listen_socket;

    ListSession m_session;
    SW_Thread * m_worker_thread;
};

#endif