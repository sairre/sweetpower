#include <stdio.h>
#include "SW_GameServer.h"
int main(void)
{
    SW_GameServer * server = new SW_GameServer();
    server->InitGameServer();
	getchar();

    return 0;
}