#include "RoleMgr.h"

#include "ReceiveDataQueque.h"
#include "SW_Role.h"
#include "SW_Thread.h"
#include "ProcessHandle.h"
RoleMgr::RoleMgr()
{
    memset(m_role_arr, 0, sizeof(m_role_arr));
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

int RoleMgr::AddRole(int player_id)
{
    // TODO  数据库中是否有记录，没有就添加

    if (!IsValidPlayerId(player_id))
    {
        return 0;
    }
    SW_Role * role = new SW_Role();
    role->set_player_id(player_id);
    
    m_role_arr[player_id] = role;

}

int RoleMgr::SetOnline(int player_id)
{
    if (!IsValidPlayerId(player_id))
    {
        return 0;
    }
   SW_Role * role = m_role_arr[player_id];
    if (NULL == role)
    {
        return 0;
    }

    role->set_is_connected(true);
    return 0;
}

int RoleMgr::SetOffline(int player_id)
{
    if (!IsValidPlayerId(player_id))
    {
        return 0;
    }
    SW_Role * role = m_role_arr[player_id];
    if (NULL == role)
    {
        return 0;
    }

    role->set_is_connected(false);
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

int RoleMgr::HandleLostClient()
{
    return 0;
}
int startEx = 0;
int RoleMgr::HandleClientMsg()
{
    int player_id = 0;
    int msg_len = 0;

    SERVER_BASE_DATA * data = ReceiveDataQueque::FetchFrontData(player_id, msg_len);
    if (NULL == data)
    {
        if (startEx > 0)
        {
            int end = GetTickCount64();
            printf("time: %d \n", end - startEx);
            startEx = 0;
        }
       
        return 0;
    }

    static int start = GetTickCount64();
    startEx = start;
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
    free(data);
    data = NULL;

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

bool RoleMgr::IsValidPlayerId(int player_id)
{
    return (player_id >= 0) && (player_id < MAX_ROLE_NUMS);
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
