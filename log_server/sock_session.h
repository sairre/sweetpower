#ifndef SOCK_SESSION_H
#define  SOCK_SESSION_H

#include "..\game_server\PlatForm.h"
#include "BufferHandler.h"
#define  MAX_LEN 8192
class sock_session
{
public:
    sock_session();
    ~sock_session();
    sw_sock get_sock() { return m_sock; }

    void set_sock(sw_sock sock) { m_sock = sock; }

    int sock_accept();

    int sock_recv(BufferHandler* pBufferHandler);

    int sock_send();

    int prototype_recv(sock_session* sock_ss, BufferHandler* pBufferHandler);

	bool is_connected() { return m_bConnected; }

	void set_connect() { m_bConnected = true; }
	void set_close() { m_bConnected = false; }

private:

    sw_sock m_sock;

    char m_recv_buf[MAX_LEN + 1];
    char m_send_buf[MAX_LEN + 1];

    int m_data_recved;
    int m_data_recv_handled;

	bool m_bConnected;

};

#endif

