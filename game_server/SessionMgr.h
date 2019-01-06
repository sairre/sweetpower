#ifndef _SESSION_MGR_H
#define _SESSION_MGR_H
#include "ClientSession.h"

typedef std::list<ClientSession*> ClientSessionList;
class SessionMgr
{
public:
	SessionMgr();
	~SessionMgr();

	bool AddSessionToList(int socket, int port);

	bool AddSessionToList(ClientSession* client_session);

	int ClearInvalidSession();
private:
	ClientSessionList m_session_list;
};

#endif // 

