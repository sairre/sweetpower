#ifndef __SW_THREAD_H
#define __SW_THREAD_H
#include "PlatForm.h"
typedef void (*THREAD_FUNC)(void* args);
class SW_Thread
{
public:
    SW_Thread();
    ~SW_Thread();

    bool CreateSWThread(THREAD_FUNC thread_func, void* args);

   /* bool Run();
    bool ResumeThread();

    bool StopThread();*/

    bool RunWork();

private:
    HANDLE m_event;
    HANDLE m_thread_handle;
    THREAD_FUNC m_thread_func;
    bool m_status;
    void* m_args;
    unsigned int m_thread_id;
};
#endif

