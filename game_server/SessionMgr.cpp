#include "SessionMgr.h"



SessionMgr::SessionMgr()
{
}


SessionMgr::~SessionMgr()
{
}

bool SessionMgr::AddSessionToList(int socket, int port)
{
	return true;
}

bool SessionMgr::AddSessionToList(ClientSession* client_session)
{
	m_session_list.push_back(client_session);
    printf("new comer \n");
	return true;
}

int SessionMgr::ClearInvalidSession()
{
	ClientSessionList::iterator iter = m_session_list.begin();
	for (; iter != m_session_list.end(); )
	{
		ClientSession* client_session = *iter;
		if (NULL == client_session)
		{
			iter = m_session_list.erase(iter);
            printf("invalid session\n");
			continue;
		}
		

		if (client_session->get_m_is_removed())
		{
			iter = m_session_list.erase(iter);
            printf("lost one session\n");
			continue;
		}

		++iter;
	

	}

	return 0;
}
