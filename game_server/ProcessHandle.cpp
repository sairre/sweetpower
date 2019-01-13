#include "ProcessHandle.h"
#include <string.h>

ProcessHandle * ProcessHandle::m_process_handle = NULL;


ProcessHandle::ProcessHandle()
{
    m_process_handle = this;
    memset(&m_process_arr, 0, sizeof(m_process_arr));

}


ProcessHandle::~ProcessHandle()
{
}

ProcessHandle * ProcessHandle::Instance()
{
    return m_process_handle;
}

int ProcessHandle::StaticRegisterHandle(int cmd_type, PROCESS_HANDLER handle, int msg_len)
{
    return m_process_handle->RegisterHandle(cmd_type, handle, msg_len);
}

int ProcessHandle::StaticTriggerProcessHandle(SW_Role * role, SERVER_BASE_DATA * data, int data_len)
{
    return m_process_handle->TriggerProcessHandle(role, data, data_len);
}

int ProcessHandle::RegisterHandle(int cmd_type, PROCESS_HANDLER handle, int msg_len)
{
  
    if (!m_process_handle->IsValidCmdType(cmd_type))
    {
        return 0;
    }

    if (msg_len < 0)
    {
        return 0;
    }

    m_process_arr[cmd_type] = handle;
    m_process_msg_len[cmd_type] = msg_len;
    return true;
}

int ProcessHandle::TriggerProcessHandle(SW_Role * role, SERVER_BASE_DATA * data, int data_len)
{
    if (NULL == role)
    {
        return 0;
    }

    if (NULL == data)
    {
        return 0;
    }

    int cmd_type = data->cmd_type;
    
    if (!m_process_handle->IsValidCmdType(cmd_type))
    {
        return 0;
    }

    PROCESS_HANDLER process_func = m_process_arr[cmd_type];
    if (NULL == process_func)
    {
        return 0;
    }

    int msg_len = m_process_msg_len[cmd_type];

    // ´íÎóµÄ°ü£¬ÌÞ³ý
    if (msg_len != data_len)
    {
        return 0;
    }

    return process_func(role, data);

}

bool ProcessHandle::IsValidCmdType(int cmd_type)
{
    int arr_len = sizeof(m_process_arr) / sizeof(char);
    if (cmd_type < 0)
    {
        return false;
    }
    if (cmd_type >= arr_len)
    {
        return false;
    }

    return true;
}

