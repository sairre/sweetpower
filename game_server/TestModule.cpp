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
    TEST_DATA * test_data =reinterpret_cast<TEST_DATA*>(data);
    printf("hello world : %d, %d\n", role->get_player_id(), test_data->para_int);
    return 0;
}
