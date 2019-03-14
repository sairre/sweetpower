#include "RoleMgr.h"

#include "SW_Role.h"
#include "SW_Thread.h"
#include "ProcessHandle.h"
RoleMgr::RoleMgr()
{
    memset(m_role_arr, 0, sizeof(m_role_arr));
    rec_data_queue = new RW_Queue<DataQueue>;
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

	return 0;
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

int RoleMgr::AddRecData(int player_id, const char* data, int len)
{
    const SERVER_BASE_DATA* base_data = reinterpret_cast<const SERVER_BASE_DATA*>(data);
    if (NULL == base_data)
    {
        return 0;
    }

    SERVER_BASE_DATA* copy_data = (SERVER_BASE_DATA*)malloc(sizeof(char) * len);
    if (NULL == copy_data)
    {
        return 0;
    }

    memmove(copy_data, base_data, len);
    DataQueue rec_data;
    rec_data.msg_data = copy_data;
    rec_data.msg_len = len;
    rec_data.player_id = player_id;

    rec_data_queue->AddToQueue(rec_data);

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
    DataQueue rec_data;
    if (!rec_data_queue->FetchData(rec_data))
    { if (startEx > 0)
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
    if (rec_data.player_id < 0)
    {
        return 0;
    }

    if (rec_data.player_id >= role_len)
    {
        return 0;
    }

    SW_Role * role = m_role_arr[rec_data.player_id];
    if (NULL == role)
    {
        return 0;
    }
  
    ProcessHandle::StaticTriggerProcessHandle(role, rec_data.msg_data, rec_data.msg_len);
    free(rec_data.msg_data);
    rec_data.msg_data = NULL;

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
