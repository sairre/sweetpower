#include "ClientSession.h"


ClientSession::ClientSession(int socket, int port):m_socket(socket),m_port(port)
{
	static int player_id = 0;
	++player_id;
	
	m_player_id = player_id;

	m_is_removed = false;
}

ClientSession::~ClientSession()
{
}
