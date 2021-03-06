#include "SW_GameServer.h"

#include "NetIocp.h"
#include "ProcessHandle.h"
#include "RoleMgr.h"

SW_GameServer * g_sw_gameserver = NULL;
SW_GameServer::SW_GameServer()
{
}


SW_GameServer::~SW_GameServer()
{
}

int SW_GameServer::InitGameLogic()
{

    return 0;
}

int SW_GameServer::InitGameServer()
{
    m_process_handle = new ProcessHandle();
    if (NULL == m_process_handle)
    {
        return 0;
    }

    m_role_mgr = new RoleMgr();
    if (NULL == m_role_mgr)
    {
        return 0;
    }

    m_net_io = new NetIocp();
    if (NULL == m_net_io)
    {
        return 0;
    }

    InitGameLogic();

    m_role_mgr->StartRoleMgr();
    iocp_item cb_item;

    m_net_io->set_callback(cb_item);
    m_net_io->StartServer();

    return 0;
}
