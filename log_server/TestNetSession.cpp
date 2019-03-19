#include "TestNetSession.h"
#include "BufferHandler.h"
#include "buffer.h"
#include "..\game_server\StructData.h"



TestNetSession::TestNetSession()
{
    m_pBufferHandler = new BufferHandler(&TestNetSession::HandleRecData);
}


TestNetSession::~TestNetSession()
{
}

void TestNetSession::HandleRecData(void* args)
{
    buffer * buf = (buffer*)args;
    TEST_DATA * pdata = (TEST_DATA*)(buf->get_buffer());
    printf("print: %s \n", pdata->stra);
    fflush(stdout);
}
