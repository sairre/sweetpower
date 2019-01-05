#include <stdio.h>
#include "NetIocp.h"
int main(void)
{
	NetIocp* pIocp = new NetIocp();
	pIocp->StartServer();

	getchar();
    return 0;
}