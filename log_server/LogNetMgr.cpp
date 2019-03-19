#include "LogNetMgr.h"

#include "selector.h"
#include "BufferHandler.h"
#include "TestNetSession.h"
LogNetMgr* LogNetMgr::m_pInstance = NULL;
LogNetMgr::LogNetMgr()
{
}


LogNetMgr::~LogNetMgr()
{
}

LogNetMgr* LogNetMgr::Instance()
{
    // TODO 
    // perhaps need not to handle thread-safe
    if (NULL == m_pInstance)
    {
        m_pInstance = new LogNetMgr();
    }

    return m_pInstance;
}

int LogNetMgr::CreateListener(int type, int port)
{
    selector* pSelector = new selector();
    TestNetSession* pNetSession = new TestNetSession();
    pSelector->SetBufferHandler(pNetSession->GetBufferHandler());
    pSelector->start_listen();
    pSelector->start();
    return 0;
}
