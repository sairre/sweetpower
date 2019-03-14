#include "sock_session.h"

#include <stdio.h>
#include "..\game_server\StructData.h"

sock_session::sock_session():m_data_recved(0),m_data_recv_handled(0),m_bConnected(false)
{
}


sock_session::~sock_session()
{
}

int sock_session::sock_accept()
{
    return 0;
}

int sock_session::sock_recv()
{
    int ret = 0;
    int recved = 0;
	// 防止覆盖数据发生
	if (m_data_recved >= MAX_LEN)
	{
		printf("rec buff is full\n");
		prototype_recv(this);
		return 0;
	}
	ret = recv(m_sock, m_recv_buf + m_data_recved, MAX_LEN - m_data_recved, 0);
	if (-1 == ret)
	{
		int err = WSAGetLastError();
		printf("recv err : %d \n", err);
		set_close();
		return 0;
	}
	else if (0 == ret)
	{
		printf("recv  sock is closed\n");
		set_close();
		return 0;
	}
	m_data_recved += ret;

	// 数据解析层次解析
    prototype_recv(this);

    return 0;
}

int sock_session::sock_send()
{
    return 0;
}

int sock_session::prototype_recv(sock_session* sock_ss)
{
    if (NULL == sock_ss)
    {
        return 0;
    }

	while (1)
	{
		// 读取包头，不足一个包头，放弃
		if (sock_ss->m_data_recved < 4)
		{
			return 0;
		}
		int len = *((int*)(sock_ss->m_recv_buf));

		// 判断是否有一个整包
		int recv_len = (sock_ss->m_data_recved);
		if (recv_len < len)
		{
			return 0;
		}

		// 有一个整包可以读取
		char* msg = (char*)calloc(1, sizeof(char) * len);
		if (NULL == msg)
		{
			return 0;
		}
		memmove(msg, sock_ss->m_recv_buf + 4, len);
		TEST_DATA * pdata = (TEST_DATA*)msg;
		printf("get msg:%s\n", pdata->stra);
		fflush(stdout);

		memmove(sock_ss->m_recv_buf, sock_ss->m_recv_buf + len + 4, len + 4);
		sock_ss->m_data_recved -= ( 4 + len);
	}
    

    return 0;
}
