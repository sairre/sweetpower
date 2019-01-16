#include <stdio.h>
#include "SW_GameServer.h"
int main(void)
{
    g_sw_gameserver = new SW_GameServer();
    g_sw_gameserver->InitGameServer();
	getchar();

    return 0;
}