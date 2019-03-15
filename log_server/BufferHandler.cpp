#include "BufferHandler.h"



BufferHandler::BufferHandler():m_consume_thread_func(0)
{
}


BufferHandler::BufferHandler(THREAD_FUNC pFunc):m_consume_thread_func(pFunc)
{

}

void BufferHandler::ConsumeBuffer()
{
    while (1)
    {
        if (m_send_vec.empty())
        {
            Sleep(10);
            continue;
        }
        // ��Ҫ�������У�����ɾ��
        buffer* pBuff = m_send_vec.front();
        m_consume_thread_func(pBuff);
        
    }
}

BufferHandler::~BufferHandler()
{
}

int BufferHandler::SetConsumeThread(THREAD_FUNC pFunc)
{
    m_consume_thread_func = pFunc;
    return 0;
}

int BufferHandler::StartBufferHandleThread()
{
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
    m_send_vec.push_back(buf);

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
