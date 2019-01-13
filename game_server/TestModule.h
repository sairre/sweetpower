#ifndef _TEST_MODULE_H
#define _TEST_MODULE_H
#include "IGameLogic.h"
class SW_Role;
struct SERVER_BASE_DATA;
class TestModule :public IGameLogic
{
public:
    TestModule();
    virtual ~TestModule();

    virtual int Update();

    static int TestClientMsg(SW_Role * role, SERVER_BASE_DATA * data);
};
#endif
