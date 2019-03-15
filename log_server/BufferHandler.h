#ifndef BUFFER_HANDLER_H
#define BUFFER_HANDLER_H
#include <vector>
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
    std::vector<buffer*> m_send_vec;

    SW_Thread m_thread;
};
#endif

