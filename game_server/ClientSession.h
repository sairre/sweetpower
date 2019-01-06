#ifndef _CLIENT_SESSION_H
#define _CLIENT_SESSION_H
#include <list>

class ClientSession
{
public:
	ClientSession(int socket, int port);
	~ClientSession();


	int get_player_id() { return m_player_id; }

	bool get_m_is_removed() { return m_is_removed; }
	
	void set_m_is_removed(bool value) { m_is_removed = value; }
private:
	ClientSession();

private:
	int m_socket;
	int m_port;
	int m_player_id;

	bool m_is_removed;
};


#endif
