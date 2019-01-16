#ifndef _ROLE_MGR_H
#define _ROLE_MGR_H
class SW_Role;
class SW_Thread;
const int MAX_ROLE_NUMS = 65536;
class RoleMgr
{
public:
    RoleMgr();
    virtual ~RoleMgr();

    int StartRoleMgr();

    int AddRole(int player_id);

    int SetOnline(int player_id);

    int SetOffline(int player_id);
 
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

};

#endif

