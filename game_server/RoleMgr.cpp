#include "RoleMgr.h"

#include "ReceiveDataQueque.h"
#include "SW_Role.h"
#include "SW_Thread.h"
#include "ProcessHandle.h"
RoleMgr::RoleMgr()
{
    
}


RoleMgr::~RoleMgr()
{
}

int RoleMgr::StartRoleMgr()
{
    m_worker_thread = new SW_Thread();
    if (NULL == m_worker_thread)
    {
        return 0;
    }

    m_worker_thread->CreateSWThread(&RoleMgr::RoleThread, this);

    return 0;
}

int RoleMgr::DoRoleWork()
{
    while (true)
    {
        HandleClientMsg();

        HandleDispatchMsg();
    }
}

int RoleMgr::HandleClientMsg()
{
    int player_id = 0;
    int msg_len = 0;
    SERVER_BASE_DATA * data = ReceiveDataQueque::FetchFrontData(player_id, msg_len);
    if (NULL == data)
    {
        return 0;
    }

    int role_len = sizeof(m_role_arr) / sizeof(SW_Role*);
    if (player_id < 0)
    {
        return 0;
    }

    if (player_id >= role_len)
    {
        return 0;
    }

    SW_Role * role = m_role_arr[player_id];
    if (NULL == role)
    {
        return 0;
    }

    ProcessHandle::StaticTriggerProcessHandle(role, data, msg_len);

    return 0;
    

}

int RoleMgr::HandleDispatchMsg()
{
    return 0;
}

int RoleMgr::HandleExecute()
{
    return 0;
}

void RoleMgr::RoleThread(void * args)
{
    RoleMgr * role_mgr = reinterpret_cast<RoleMgr*>(args);
    if (NULL == role_mgr)
    {
        return;
    }

    role_mgr->DoRoleWork();
}
