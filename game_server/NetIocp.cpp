
#include "NetIocp.h"
#include <stdio.h>


#include <mswsock.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
//#pragma comment(lib, "odbc32.lib")
//#pragma comment(lib, "odbccp32.lib")
const int MAX_REC_DATA_SIZE = 256;

enum IOCP_DATA_ENUM
{
	IOCP_DATA_ACCEPT = 1,
	IOCP_DATA_RECV   = 2,

};
struct iocp_data
{
	WSAOVERLAPPED overlapped;
	int data_type;
	SOCKET accept_socket;
	WSABUF wsabuffer;
	char data[MAX_REC_DATA_SIZE];
	int recv_len;
	int read_len;
};

struct completion_key
{
	SOCKET client_socket;
	int port;

};

class IocpImpl
{
public:
	IocpImpl() :m_iocp(NULL), m_listen_socket(INVALID_SOCKET)
	{}

	~IocpImpl() {}

	iocp_item m_iocp_cbs;

	HANDLE m_iocp;
	SOCKET m_listen_socket;
};

NetIocp::NetIocp()
{
	InitSocket();
}


NetIocp::~NetIocp()
{
	CleanSocket();
}

int NetIocp::set_callback(iocp_item& item)
{
	m_iocp_cbs = item;
	return 0;
}

int NetIocp::StartServer()
{
	m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (NULL == m_iocp)
    {
        printf("error");
        return 0;
    }
    // 创建监听socket
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
    s_address.sin_port = htons(5150);

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

    // 添加监听事件
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_listen_socket), m_iocp, static_cast<DWORD>(0), 0);

	PostAcceptEvent();

	DoIocpWork();
    
    return 0;
}

int NetIocp::DoIocpWork()
{
	DWORD dw_trans = 0;
	completion_key* complete_key;
	iocp_data* pdata;
	while (true)
	{
		complete_key = NULL;
		dw_trans = 0;
		pdata = NULL;

		int ret = GetQueuedCompletionStatus(m_iocp, &dw_trans, (PULONG_PTR)&complete_key,
			(LPOVERLAPPED*)&pdata, WSA_INFINITE);
		if (0 == ret)
		{
			int err = GetLastError();
			printf("iocp error status :%d \n", err);
			closesocket(pdata->accept_socket);
			continue;
		}

		if (NULL == pdata)
		{
			continue;
		}

		if (0 == dw_trans && pdata->data_type == IOCP_DATA_RECV)
		{
			// client close
			continue;
		}
		//printf("new event comes!\n");

		switch (pdata->data_type)
		{
		case IOCP_DATA_ACCEPT:
		{
			SOCKET client_socket = pdata->accept_socket;
			int addr_size = sizeof(sockaddr_in) + 16;
			struct sockaddr_in * local_sock_addr = NULL;
			struct sockaddr_in * remote_sock_addr = NULL;
			int local_len, remote_len = 0;
			int len = sizeof(sockaddr_in);

			GetAcceptExSockaddrs(pdata->wsabuffer.buf,
				pdata->wsabuffer.len - 2 * addr_size,
				addr_size, addr_size, (sockaddr**)&local_sock_addr, &local_len,
				(sockaddr**)&remote_sock_addr, &remote_len);
			/*GetAcceptExSockaddrs(pdata->wsabuffer.buf,
				0,
				addr_size, addr_size, (sockaddr**)&local_sock_addr, &local_len,
				(sockaddr**)&remote_sock_addr, &remote_len);
*/
			completion_key * pkey = new completion_key();
			pkey->client_socket = client_socket;
			pkey->port = remote_sock_addr->sin_port;

			CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_socket), m_iocp, (ULONG_PTR)&pkey, 0);

			PostAcceptEvent();
			
			
			PostRecvEvent(pdata);

			
		}
		break;
		case IOCP_DATA_RECV:
		{
			// pdata->data[dw_trans] = 0; data 是实际数组起始位置偏移dw_trans位置，而pdata->wsabuffer.buf
			// 为本次接收的位置偏移，可能不是从0开始
			// pdata->wsabuffer.buf[dw_trans] = 0;
		
			pdata->data[pdata->recv_len + dw_trans] = 0;

			// 分包
			int len_size = 4;
			int pack_len = 0;

			int total_rec = 0;
			int start_len = pdata->read_len;
			int data_rec = pdata->recv_len - pdata->read_len + dw_trans;
			int left_data = 0;

			pdata->recv_len += dw_trans;

			do 
			{
				left_data = data_rec - total_rec;
				
				char* pPackHead = pdata->data + start_len + total_rec;
				if (NULL == pPackHead)
				{
					break;
				}

				if (left_data < len_size)
				{
					// 剩下收取到的数据长度不足以表示包头的话，
					// 清除前面的包数据，移动后面的数据
					memmove(pdata->data, pPackHead, left_data);
					pdata->recv_len = left_data;
					pdata->read_len = 0;
					break;
				}

				pack_len = *((int*)pPackHead);
				
				if (pack_len > MAX_REC_DATA_SIZE)
				{
					printf("too long data");
					pdata->recv_len = 0;
					pdata->read_len = 0;
					break;
				}

				if (left_data < pack_len)
				{
					// 剩下收取到的数据长度不足以表示一个包的话
					// 清除前面的包数据，移动后面的数据
					memmove(pdata->data, pPackHead, left_data);
					pdata->recv_len = left_data;
					pdata->read_len = 0;
					break;
				}

				if (left_data >= pack_len)
				{
					// 说明有一个完整的包
					// TODO 处理该包数据
					char* pChar = pPackHead + len_size;
					PrintData(pChar, pack_len);
					
					// 更新剩余数据长度
					total_rec += len_size + pack_len;
					left_data = data_rec - total_rec;
					pdata->read_len += len_size + pack_len;
				}

			} while (left_data > 0);

			
		
		
			PostRecvEvent(pdata);
		}
		break;
		default:
			break;
		}


	}
}

int NetIocp::PostAcceptEvent()
{
	iocp_data * pdata = gs_new iocp_data();
	if (NULL == pdata)
	{
		return 0;
	}
	memset(pdata, 0, sizeof(*pdata));

	pdata->data_type = IOCP_DATA_ACCEPT;

	// 指向一块已经分配好的内存
	pdata->wsabuffer.buf = pdata->data;
	pdata->wsabuffer.len = MAX_REC_DATA_SIZE - 1;

	DWORD dword = 0;
	SOCKET client_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	int addr_size = sizeof(sockaddr_in) + 16;
	pdata->accept_socket = client_socket;

	//AcceptEx(m_listen_socket, client_socket, pdata->wsabuffer.buf, pdata->wsabuffer.len - addr_size * 2,
	//	addr_size, addr_size, &dword, &pdata->overlapped);

	AcceptEx(m_listen_socket, client_socket, pdata->wsabuffer.buf, 0,
		addr_size, addr_size, &dword, &pdata->overlapped);

	return 0;
}

int NetIocp::PostRecvEvent(iocp_data * pdata)
{
	if (NULL == pdata)
	{
		return 0;
	}
	
	pdata->data_type = IOCP_DATA_RECV;

	// 指向一块已经分配好的内存
	pdata->wsabuffer.buf = pdata->data + pdata->recv_len;
	pdata->wsabuffer.len = MAX_REC_DATA_SIZE - 1 - pdata->recv_len;

	DWORD dword = 0;
	DWORD dwFlag = 0;

	int ret = WSARecv(pdata->accept_socket, &(pdata->wsabuffer),
		1, &dword, &dwFlag, &(pdata->overlapped), NULL);

	return 0;
}

int NetIocp::InitSocket()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    return 0;
}

int NetIocp::CleanSocket()
{
    WSACleanup();

    return 0;
}

int NetIocp::PrintData(const char* data, int len)
{
	printf("recv:%s ,len = %d \n", data, len);

	return 0;
}
