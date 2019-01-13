#ifndef _ROLE_MGR_H
#define _ROLE_MGR_H
class SW_Role;
class SW_Thread;
class RoleMgr
{
public:
    RoleMgr();
    virtual ~RoleMgr();

    int StartRoleMgr();
private:
 
    int DoRoleWork();

    int HandleClientMsg();

    int HandleDispatchMsg();

    int HandleExecute();

private:
    static void RoleThread(void * args);

private:

    SW_Role * m_role_arr[1024]; 
    SW_Thread * m_worker_thread;
};

#endif

