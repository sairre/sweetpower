#ifndef __RECEIVE_DATA_QUEUE_H
#define __RECEIVE_DATA_QUEUE_H
#include "StructData.h"
#include "PlatForm.h"
#include <deque>
class ProcessHandle;
const int PER_MSG_DATA_MAX = 1024;
struct DataQueue
{
    int player_id;
    int msg_len;
    SERVER_BASE_DATA* msg_data;
    DataQueue() :player_id(0), msg_len(0),msg_data(NULL) {}
};
class ReceiveDataQueque
{
public:
    ReceiveDataQueque();
    ~ReceiveDataQueque();

    static ReceiveDataQueque * Instance();

    static bool ReceiveData(int player_id, const char* data, int len);

    static SERVER_BASE_DATA * FetchFrontData(int & player_id, int & len);

private:
    std::deque<DataQueue> m_data_deque;
    CRITICAL_SECTION m_rec_data_cs;

    static ReceiveDataQueque* single_queue;

    ProcessHandle * process_handle;
   
};

#endif

