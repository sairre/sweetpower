#ifndef SELECTOR_H
#define SELECTOR_H
#include "..\game_server\PlatForm.h"
#include <vector>
#include "sw_net.h"
class BufferHandler;
class sock_session;
class selector
{
public:
    selector();
    ~selector();

public:
    int init_evironment(const sw_net_cb& cb);

    void SetBufferHandler(BufferHandler* pBufferHandler) { m_pBufferHandler = pBufferHandler; }
   
    int set_opt(sw_sock socket);
    bool start_listen();

    bool start_connect();

    bool start();


private:

    int fill_fd_set();

    int accept_new_commer();

    int recv_data();

	int remove_dead_connect();

private:
    sw_sock m_sock;
    FD_SET m_read_set;
    FD_SET m_write_set;

    std::vector<sock_session*> m_sock_vec;

    sw_net_cb m_cb;
    BufferHandler* m_pBufferHandler;
};

#endif // !SELECTOR_H
