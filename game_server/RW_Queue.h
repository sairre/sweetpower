#ifndef _RW_QUEUE_H
#define _RW_QUEUE_H
#include "PlatForm.h"
#include <deque>

template <class T>
class RW_Queue
{
public:
    RW_Queue();
    virtual ~RW_Queue();

    int AddToQueue(const T & data);

    bool FetchData(T & data);

private:

    bool FetchFromQueue(std::deque<T> & putin_queue, std::deque<T> & fetch_queue, T & data);

private:
    std::deque<T> m_queue_0;  // Ĭ��д��
    std::deque<T> m_queue_1;  // Ĭ��ȡ��
    SRWLOCK status_rw_lock;
    bool m_status;
};

template <class T>
RW_Queue<T>::RW_Queue() :m_status(false)
{
    InitializeSRWLock(&status_rw_lock);
}


template <class T>
RW_Queue<T>::~RW_Queue()
{
}



template <class T>
int RW_Queue<T>::AddToQueue(const T & data)
{
    AcquireSRWLockShared(&status_rw_lock);
    if (m_status)
    {
        m_queue_1.push_back(data);
    }
    else
    {
        m_queue_0.push_back(data);
    }
    ReleaseSRWLockShared(&status_rw_lock);

    return 0;
}

template <class T>
bool RW_Queue<T>::FetchData(T & data)
{
    bool result = false;
    AcquireSRWLockShared(&status_rw_lock);
    if (m_status)
    {
        ReleaseSRWLockShared(&status_rw_lock);
        result = FetchFromQueue(m_queue_1, m_queue_0, data);
    }
    else
    {
        ReleaseSRWLockShared(&status_rw_lock);
        result = FetchFromQueue(m_queue_0, m_queue_1, data);
    }


    return result;
}

template <class T>
bool RW_Queue<T>::FetchFromQueue(std::deque<T> & putin_queue, std::deque<T> & fetch_queue, T & data)
{
    // ���ȡ���ݶ����Ѿ�Ϊ�գ���Ӧ�ý�������
    if (fetch_queue.empty())
    {
        // �޸�����Ҫ��д��
        AcquireSRWLockExclusive(&status_rw_lock);
        if (putin_queue.empty())
        {
            ReleaseSRWLockExclusive(&status_rw_lock);
            return false;
        }
        int count = putin_queue.size();
        m_status = !m_status;
        printf("exchange :%d \n", count);

        // ȡ����Ҫ�������������������߳���ͬʱ�����ö���
        data = putin_queue.front();
        putin_queue.pop_front();
        ReleaseSRWLockExclusive(&status_rw_lock);
    }
    else
    {
        data = fetch_queue.front();
        fetch_queue.pop_front();
    }

    return true;
}
#endif

