#pragma once
#include <windows.h>
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

    int start_server();
private:
    int init_socket();

    int clean_socket();

private:
    iocp_item m_iocp_cbs;

    HANDLE m_iocp;
    SOCKET m_listen_socket;
};

