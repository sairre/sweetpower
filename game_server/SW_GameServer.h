#ifndef _SW_GAMESERVER_H
#define _SW_GAMESERVER_H

#include "RoleMgr.h"
class ReceiveDataQueque;
class ProcessHandle;
class NetIocp;
class SW_GameServer
{
public:
    SW_GameServer();
    ~SW_GameServer();

    int InitGameLogic();

    int InitGameServer();

public:

    ProcessHandle * m_process_handle;
    NetIocp * m_net_io;
    RoleMgr * m_role_mgr;
};

extern SW_GameServer * g_sw_gameserver;
#endif
