#ifndef BUFFER_HANDLER_H
#define BUFFER_HANDLER_H
#include "../game_server/PlatForm.h"
#include <queue>
#include "..\game_server\SW_Thread.h"
class buffer;
class BufferHandler
{
public:
    BufferHandler(THREAD_FUNC pFunc);
    ~BufferHandler();

    int SetConsumeThread(THREAD_FUNC pFunc);

    int StartBufferHandleThread();

    int SendBuffer(buffer* buf);


    static void BufferThread(void * args);
private:
    BufferHandler();

    void ConsumeBuffer();

private:
    THREAD_FUNC m_consume_thread_func;
    std::queue<buffer*> m_send_queue;

    SW_Thread m_thread;
    HANDLE m_NoticeConsume;
    CRITICAL_SECTION m_queue_cs;
};
#endif

