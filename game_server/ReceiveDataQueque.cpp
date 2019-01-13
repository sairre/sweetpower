#include "ReceiveDataQueque.h"
#include <string.h>
#include "CriticalLock.h"

ReceiveDataQueque* ReceiveDataQueque::single_queue = NULL;
ReceiveDataQueque::ReceiveDataQueque()
{
    single_queue = this;
    InitializeCriticalSection(&m_rec_data_cs);
}


ReceiveDataQueque::~ReceiveDataQueque()
{
    DeleteCriticalSection(&m_rec_data_cs);
}

ReceiveDataQueque * ReceiveDataQueque::Instance()
{
    // 不在这里互斥的访问，如果为空，new一个出来
    // 会频繁的在多个线程中调用，所以不做限制，在主线程中new
   return single_queue;
}

bool ReceiveDataQueque::ReceiveData(int player_id, const char* data, int len)
{
    const SERVER_BASE_DATA* base_data = reinterpret_cast<const SERVER_BASE_DATA*>(data);
    if (NULL == base_data)
    {
        return false;
    }

    SERVER_BASE_DATA* copy_data = (SERVER_BASE_DATA*)malloc(sizeof(char) * len);
    if (NULL == copy_data)
    {
        return false;
    }

    memmove(copy_data, base_data, len);
    DataQueue data_item;
    data_item.player_id = player_id;
    data_item.msg_data = copy_data;
    data_item.msg_len = len;
    
    {
        CriticalLock clock(single_queue->m_rec_data_cs); 
        single_queue->m_data_deque.push_back(data_item);
    }

    return true;
   
}

SERVER_BASE_DATA * ReceiveDataQueque::FetchFrontData(int & player_id, int & len)
{
    {
        CriticalLock clock(single_queue->m_rec_data_cs); 
        if (!single_queue->m_data_deque.empty())
        {
            DataQueue data = single_queue->m_data_deque.front();
            player_id = data.player_id;
            len = data.msg_len;
            single_queue->m_data_deque.pop_front();
            return data.msg_data;
        }

        return NULL;
    }
}
