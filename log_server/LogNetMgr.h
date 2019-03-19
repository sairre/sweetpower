#pragma once
#include "INetMgr.h"
class LogNetMgr:public INetMgr
{
public:
    ~LogNetMgr();
    static LogNetMgr* Instance();
    
    virtual int CreateListener(int type, int port) override;
private:
    LogNetMgr();

private:
    static LogNetMgr* m_pInstance;

};

