#include "NetIocp.h"
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

NetIocp::NetIocp():m_iocp(NULL), m_listen_socket(INVALID_SOCKET)
{
}


NetIocp::~NetIocp()
{
}

int NetIocp::set_callback(iocp_item& item)
{
    m_iocp_cbs = item;
    return 0;
}

int NetIocp::start_server()
{
    m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (NULL == m_iocp)
    {
        printf("error");
        return 0;
    }
    // ´´½¨¼àÌýsocket
    m_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == m_listen_socket)
    {
        printf("error");
        return 0;
    }

    struct sockaddr_in s_address;
    memset(&s_address, 0, sizeof(s_address));
    s_address.sin_family = AF_INET;
    s_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    s_address.sin_port = htons(8998);

    if (0 != bind(m_listen_socket, (struct sockaddr *) &s_address, sizeof(s_address)))
    {
        printf("error");
        return 0;
    }

    if (0 != listen(m_listen_socket, SOMAXCONN)) 
    {
        printf("error");
        return 0;
    }

    
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_listen_socket), m_iocp, static_cast<DWORD>(0), 0);
    
    return 0;
}

int NetIocp::init_socket()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    return 0;
}

int NetIocp::clean_socket()
{
    WSACleanup();

    return 0;
}
