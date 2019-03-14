#include "selector.h"
#include "sock_session.h"
#include <stdio.h>

selector::selector()
{
    sw_net_cb cb;
    init_evironment(cb);
}


selector::~selector()
{
}

int selector::init_evironment(const sw_net_cb& cb)
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    m_cb = cb;
    return 0;
}

int selector::set_opt( sw_sock socket )
{
    int op = 0;
    int len = sizeof(op);
    return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&op, len);
}

bool selector::start_listen()
{
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sw_error_sock == m_sock)
    {
        return false;
    }

    sockaddr_in server_addr;
    //server_addr.sin_addr.S_un.S_addr = inet_pton("127.0.0.1");
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5150);
    int len = sizeof(server_addr);
    int ret = bind(m_sock, (sockaddr*)&server_addr, len);
    if (sw_error_sock == ret)
    {
        return false;
    }

    ret = listen(m_sock, 10);
    if (sw_error_sock == ret)
    {
        return false;
    }

    return true;
}

bool selector::start_connect()
{
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sw_error_sock == m_sock)
    {
        return false;
    }

    sockaddr_in server_addr;
    //server_addr.sin_addr.S_un.S_addr = inet_pton("127.0.0.1");
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(19900);

    int len = sizeof(server_addr);
    int ret = connect(m_sock, (sockaddr*)&server_addr, len);
    if (sw_error_sock == ret)
    {
        return false;
    }

    return true;

}

bool selector::start()
{
    while (1)
    {
        fill_fd_set();
        int ret = select(m_sock + 1, &m_read_set, &m_write_set, NULL, NULL);
        if (sw_error_sock == ret)
        {
            int err = WSAGetLastError();
            printf("select erro : %d \n", err);
            continue;
        }
        if (0 == ret)
        {
            continue;
        }

        accept_new_commer();
        recv_data();

    }
}

int selector::fill_fd_set()
{

    FD_ZERO(&m_write_set);
    FD_ZERO(&m_read_set);
    FD_SET(m_sock, &m_write_set);
    FD_SET(m_sock, &m_read_set);
    size_t len = m_sock_vec.size();
    for (size_t index = 0; index < len; ++ index)
    {
        sock_session* psock = m_sock_vec[index];
        if (NULL == psock)
        {
            continue;
        }
        FD_SET(psock->get_sock(), &m_read_set);
        FD_SET(psock->get_sock(), &m_write_set);
    }

    return 0;
}

int selector::accept_new_commer()
{
    if (!FD_ISSET(m_sock, &m_read_set))
    {
        return 0;
    }

    sockaddr_in client_addr;
    int len = sizeof(sockaddr_in);
    sw_sock client_sock = accept(m_sock, (sockaddr*)&client_addr, &len);
    if (sw_error_sock == client_sock)
    {
        int err = WSAGetLastError();
        printf("err accept :%d \n ", err);
    }
    sock_session* psock = new sock_session();
    psock->set_sock(client_sock);
    psock->sock_accept();
    m_sock_vec.push_back(psock);

    return 0;
}

int selector::recv_data()
{
    size_t len = m_sock_vec.size();
   for (size_t index = 0; index < len; ++ index)
   {
       sock_session* psock = m_sock_vec[index];
       if (NULL == psock)
       {
           continue;
       }
       if (!FD_ISSET(psock->get_sock(), &m_read_set))
       {
           continue;
       }

       psock->sock_recv();
   }

   return 0;
    
}
