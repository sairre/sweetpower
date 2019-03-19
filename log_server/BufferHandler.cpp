#include "BufferHandler.h"
#include "buffer.h"



BufferHandler::BufferHandler():m_consume_thread_func(0)
{
    InitializeCriticalSection(&m_queue_cs);
}


BufferHandler::BufferHandler(THREAD_FUNC pFunc):m_consume_thread_func(pFunc)
{
    InitializeCriticalSection(&m_queue_cs);
}

void BufferHandler::ConsumeBuffer()
{
    while (1)
    {
        while (1)
        {
            EnterCriticalSection(&m_queue_cs);
            if (!m_send_queue.empty())
            {
                break;
            }
            LeaveCriticalSection(&m_queue_cs);
            // ����߳�ͬʱ����ʱ����ֹ���Ѻ�vec�ֿ��ˣ���Ҫѭ���ж�
            WaitForSingleObject(m_NoticeConsume, INFINITE);
        }

        // �������У���С����Χ
        std::queue<buffer*> tmp_queue;
        tmp_queue.swap(m_send_queue);
        LeaveCriticalSection(&m_queue_cs);

        while (!tmp_queue.empty())
        {
            buffer* pBuff = tmp_queue.front();
            tmp_queue.pop();
            m_consume_thread_func(pBuff);
            delete pBuff;
            pBuff = NULL;

        }
    }
}

BufferHandler::~BufferHandler()
{
    DeleteCriticalSection(&m_queue_cs);
}

int BufferHandler::SetConsumeThread(THREAD_FUNC pFunc)
{
    m_consume_thread_func = pFunc;
    return 0;
}

int BufferHandler::StartBufferHandleThread()
{
    m_NoticeConsume = CreateEvent(NULL, FALSE, FALSE, "NoticeConsume");
    m_thread.CreateSWThread(BufferThread, this);
    return 0;
}

int BufferHandler::SendBuffer(buffer* buf)
{
    if (NULL == buf)
    {
        return 0;
    }

    // �����ߣ���Ҫ��֤�̰߳�ȫ
    EnterCriticalSection(&m_queue_cs);
    m_send_queue.push(buf);
    LeaveCriticalSection(&m_queue_cs);
    SetEvent(m_NoticeConsume);

    return 0;
}

void BufferHandler::BufferThread(void * args)
{
    BufferHandler* pBufferHandler = (BufferHandler*)(args);
    if (NULL == pBufferHandler)
    {
        return;
    }

    pBufferHandler->ConsumeBuffer();
}
