#ifndef _PROCESS_HANDLE_H
#define _PROCESS_HANDLE_H
#include "PublicHeader.h"
#include "CmdType.h"

class SW_Role;
class ProcessHandle
{
public:
    ProcessHandle();
    virtual ~ProcessHandle();

    static ProcessHandle * Instance();
    static int StaticRegisterHandle(int cmd_type, PROCESS_HANDLER handle, int msg_len);

    static int StaticTriggerProcessHandle(SW_Role * role, SERVER_BASE_DATA * data, int data_len);
    

private:

    int RegisterHandle(int cmd_type, PROCESS_HANDLER handle, int msg_len);

    int TriggerProcessHandle(SW_Role * role, SERVER_BASE_DATA * data, int data_len);


private:

    bool IsValidCmdType(int cmd_type);

private:

    PROCESS_HANDLER m_process_arr[CMD_TYPE_CLIENT_MAX];
    unsigned int m_process_msg_len[CMD_TYPE_CLIENT_MAX];

    static ProcessHandle * m_process_handle;
};

#endif
