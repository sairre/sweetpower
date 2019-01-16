
#include "NetIocp.h"

#include <stdio.h>

#include "SW_Thread.h"
#include "ReceiveDataQueque.h"
#include "SW_GameServer.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
//#pragma comment(lib, "odbc32.lib")
//#pragma comment(lib, "odbccp32.lib")
const int MAX_REC_DATA_SIZE = 256;
class ClientSession;
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





NetIocp::NetIocp()
{
	InitSocket();


    m_worker_thread = new SW_Thread();
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
    // ��������socket
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

    // ��Ӽ����¼�
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_listen_socket), m_iocp, static_cast<DWORD>(0), 0);

	PostAcceptEvent();

    m_worker_thread->CreateSWThread(&NetIocp::NetThread, this);
    
    return 0;
}

int NetIocp::DoIocpWork()
{
	DWORD dw_trans = 0;
	completion_key* complete_key;
	iocp_data* pdata;
	
	while (true)
	{
        ClearInvalidSession();

		complete_key = NULL;
		dw_trans = 0;
		pdata = NULL;
		int ret = GetQueuedCompletionStatus(m_iocp, &dw_trans, (PULONG_PTR)&complete_key,
			(LPOVERLAPPED*)&pdata, WSA_INFINITE);
		
        if (0 == ret)
        {
            if (NULL == complete_key)
            {
                continue;
            }
            complete_key->bConnect = false;
            continue;
        }

        if (NULL == pdata)
        {
            continue;
        }

        if (0 == dw_trans && pdata->data_type == IOCP_DATA_RECV)
        {
            // client close
            if (NULL == complete_key)
            {
                continue;
            }

            complete_key->bConnect = false;
            continue;
        }

        switch (pdata->data_type)
        {
        case IOCP_DATA_ACCEPT:
        {
			SOCKET client_socket = pdata->accept_socket;
			int addr_size = sizeof(sockaddr_in) + 16;
			struct sockaddr_in * local_sock_addr = NULL;
			struct sockaddr_in * remote_sock_addr = NULL;
			int local_len, remote_len = 0;

			// pdata->wsabuffer.len - 2 * addr_size ���غ�Զ�̵�ַ�����������λ�ã�ռ����2 * addr_size ��С
			// ��Ҫ��õ�ַ���ڶ��������Ͳ�����Ϊ0
			GetAcceptExSockaddrs(pdata->wsabuffer.buf,
				pdata->wsabuffer.len - 2 * addr_size,
				addr_size, addr_size, (sockaddr**)&local_sock_addr, &local_len,
				(sockaddr**)&remote_sock_addr, &remote_len);
			//GetAcceptExSockaddrs(pdata->wsabuffer.buf,
			//	0,
			//	addr_size, addr_size, (sockaddr**)&local_sock_addr, &local_len,
			//	(sockaddr**)&remote_sock_addr, &remote_len);

			// �����Ѿ����յ������ݣ�Ҫ���ϣ�����������ݻ����
			pdata->recv_len += dw_trans;
			

            static int id = 0;

			completion_key * pkey = new completion_key();
			pkey->client_socket = client_socket;
			pkey->port = remote_sock_addr->sin_port;
            
            pkey->player_id = id++;
            pkey->bConnect = true;
            // strncpy(pkey->client_ip, remote_sock_addr->sin_addr);
            g_sw_gameserver->m_role_mgr->AddRole(pkey->player_id);
            g_sw_gameserver->m_role_mgr->SetOnline(pkey->player_id);
            m_session.push_back(pkey);
			CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_socket), m_iocp, (ULONG_PTR)pkey, 0);

			PostAcceptEvent();
			PostRecvEvent(pdata);
		}
		break;
		case IOCP_DATA_RECV:
		{
			// pdata->data[dw_trans] = 0; data ��ʵ��������ʼλ��ƫ��dw_transλ�ã���pdata->wsabuffer.buf
			// Ϊ���ν��յ�λ��ƫ�ƣ����ܲ��Ǵ�0��ʼ
			// pdata->wsabuffer.buf[dw_trans] = 0;
		
			//pdata->data[pdata->recv_len + dw_trans] = 0;

			// �ְ�
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
					// ʣ����ȡ�������ݳ��Ȳ����Ա�ʾ��ͷ�Ļ���
					// ���ǰ��İ����ݣ��ƶ����������
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
					// ʣ����ȡ�������ݳ��Ȳ����Ա�ʾһ�����Ļ�
					// ���ǰ��İ����ݣ��ƶ����������
					memmove(pdata->data, pPackHead, left_data);
					pdata->recv_len = left_data;
					pdata->read_len = 0;
					break;
				}

				if (left_data >= pack_len)
				{
					// ˵����һ�������İ�
					// TODO ����ð�����
					char* pChar = pPackHead + len_size;
					
                    ReceiveDataQueque::ReceiveData(complete_key->player_id, pChar, pack_len);
					
					// ����ʣ�����ݳ���
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

	// ָ��һ���Ѿ�����õ��ڴ�
	pdata->wsabuffer.buf = pdata->data;
	pdata->wsabuffer.len = MAX_REC_DATA_SIZE - 1;

	DWORD dword = 0;
	SOCKET client_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	int addr_size = sizeof(sockaddr_in) + 16;
	pdata->accept_socket = client_socket;

	int ret = AcceptEx(m_listen_socket, client_socket, pdata->wsabuffer.buf, pdata->wsabuffer.len - addr_size * 2,
		addr_size, addr_size, &dword, &pdata->overlapped);

	/*int ret = AcceptEx(m_listen_socket, client_socket, pdata->wsabuffer.buf, 0,
		addr_size, addr_size, &dword, &pdata->overlapped);*/

	int error = GetLastError();
	//printf("accept:ret = %d, err = %d\n", ret, error);

	return 0;
}

int NetIocp::PostRecvEvent(iocp_data * pdata)
{
	if (NULL == pdata)
	{
		return 0;
	}
	
	pdata->data_type = IOCP_DATA_RECV;

	// ָ��һ���Ѿ�����õ��ڴ�
	pdata->wsabuffer.buf = pdata->data + pdata->recv_len;
	pdata->wsabuffer.len = MAX_REC_DATA_SIZE - 1 - pdata->recv_len;

	DWORD dword = 0;
	DWORD dwFlag = 0;

	int ret = WSARecv(pdata->accept_socket, &(pdata->wsabuffer),
		1, &dword, &dwFlag, &(pdata->overlapped), NULL);
	int error = GetLastError();
	//printf("recvieve:ret = %d, err = %d\n", ret, error);

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
	//printf("recv:%s ,len = %d \n", data, len);

	return 0;
}

int NetIocp::ClearInvalidSession()
{
    ListSession::iterator iter = m_session.begin();
    for (; iter != m_session.end();)
    {
        completion_key * key = *iter;
        if (NULL == key)
        {
            continue;
        }
        if (!key->bConnect)
        {
            closesocket(key->client_socket);
            g_sw_gameserver->m_role_mgr->SetOffline(key->player_id);
            iter = m_session.erase(iter);
            delete key;
            key = NULL;
        }
        else
        {
            ++iter;
        }
    }

    return 0;
}

void NetIocp::NetThread(void * args)
{
    NetIocp* iocp = reinterpret_cast<NetIocp*>(args);
    if (NULL == iocp)
    {
        return;
    }

    iocp->DoIocpWork();
}
