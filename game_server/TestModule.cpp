#include "TestModule.h"

#include <stdio.h>
#include "ProcessHandle.h"
TestModule::TestModule()
{
    ProcessHandle::StaticRegisterHandle(CMD_TYPE_CLIENT_TEST, &TestModule::TestClientMsg, sizeof(TEST_DATA));
}


TestModule::~TestModule()
{
}

int TestModule::Update()
{

    return 0;
}

int TestModule::TestClientMsg(SW_Role * role, SERVER_BASE_DATA * data)
{
    printf("hello world\n");
    return 0;
}
