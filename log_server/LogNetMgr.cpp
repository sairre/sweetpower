#include "LogNetMgr.h"

#include "selector.h"
#include "BufferHandler.h"

LogNetMgr::LogNetMgr()
{
}


LogNetMgr::~LogNetMgr()
{
}

int LogNetMgr::CreateListener(int type, int port)
{
    selector* pSelector = new selector();
    //pSelector->SetBufferHandler(new BufferHandler());
    pSelector->start_listen();
    pSelector->start();
    return 0;
}
