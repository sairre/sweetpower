#include "SW_Thread.h"

#include <process.h>

static unsigned __stdcall SW_ThreadProc(void* args)
{
    SW_Thread* sw_thread = reinterpret_cast<SW_Thread*>(args);
    if (NULL == sw_thread)
    {
        return 0;
    }

    sw_thread->RunWork();

    return 0;

}
SW_Thread::SW_Thread():m_event(NULL),m_thread_handle(NULL),m_thread_func(NULL), m_status(false)
{
}


SW_Thread::~SW_Thread()
{
}

bool SW_Thread::CreateSWThread(THREAD_FUNC thread_func, void* args)
{
    if (NULL == thread_func)
    {
        return false;
    }

    m_args = args;
    m_thread_func = thread_func;
    
    // 创建事件进行线程同步，保证获取到的数据没有被修改
    // 所有创建线程应该都在同一个线程中，否则意义不大

    // 自动复位，初始未激发
    m_event = CreateEvent(NULL, FALSE, FALSE, L"");
    
    if (NULL == m_event)
    {
        return false;
    }

    m_thread_handle = (HANDLE)_beginthreadex(NULL, 0, SW_ThreadProc, this, 0, &m_thread_id);

    WaitForSingleObject(m_event, INFINITE);

    return true;
}

bool SW_Thread::RunWork()
{
    SetEvent(m_event);
    m_thread_func(m_args);

    return true;
}
