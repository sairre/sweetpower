#pragma once
#include "INetMgr.h"
class LogNetMgr:public INetMgr
{
public:
    LogNetMgr();
    ~LogNetMgr();

    virtual int CreateListener(int type, int port) override;

private:

};

