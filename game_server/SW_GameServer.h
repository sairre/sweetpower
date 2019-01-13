#ifndef _SW_GAMESERVER_H
#define _SW_GAMESERVER_H

class ReceiveDataQueque;
class ProcessHandle;
class RoleMgr;
class NetIocp;
class SW_GameServer
{
public:
    SW_GameServer();
    ~SW_GameServer();

    int InitGameLogic();

    int InitGameServer();

private:

    ReceiveDataQueque* m_receive_data_queue;
    ProcessHandle * m_process_handle;
    NetIocp * m_net_io;
    RoleMgr * m_role_mgr;


};

#endif
