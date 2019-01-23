#ifndef _ROLE_MGR_H
#define _ROLE_MGR_H
#include "PlatForm.h"
#include "RW_Queue.h"
#include "StructData.h"
class SW_Role;
class SW_Thread;
const int MAX_ROLE_NUMS = 65536;

struct DataQueue
{
    int player_id;
    int msg_len;
    SERVER_BASE_DATA* msg_data;
    DataQueue() :player_id(0), msg_len(0), msg_data(NULL) {}
};

class RoleMgr
{
public:
    RoleMgr();
    virtual ~RoleMgr();

    int StartRoleMgr();

    int AddRole(int player_id);

    int SetOnline(int player_id);

    int SetOffline(int player_id);

    int AddRecData(int player_id, const char* data, int len);
 
private:
 
    int DoRoleWork();

    int HandleLostClient();

    int HandleClientMsg();

    int HandleDispatchMsg();

    int HandleExecute();

private:
    bool IsValidPlayerId(int player_id);
private:
    static void RoleThread(void * args);

private:

    SW_Role * m_role_arr[MAX_ROLE_NUMS];
    SW_Thread * m_worker_thread;

    RW_Queue<DataQueue> * rec_data_queue;
};

#endif

