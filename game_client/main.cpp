#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../game_server/StructData.h"

#include<winsock2.h>
#include <windows.h>
#pragma comment(lib, "WSOCK32.LIB")

int main(int argc, char** argv) {

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return -1;
    }

    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        return -1;
    }
    struct sockaddr_in sockaddr;
    sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(5150);

    int ret = connect(s, ((struct sockaddr*) &sockaddr), sizeof(sockaddr));
    if (ret != 0) {
        printf("connect error\n");
        closesocket(s);
        system("pause");
        goto out;
        return -1;
    }

    printf("connect success\n");

    int i = 0;
    TEST_DATA data;
    while (1)
    {
        data.cmd_type = 1;
        data.sub_type = -i;
        data.ext_test = 1032;
        ++i;
        data.para_int = i * i;

        if (i > 100000)
        {
            break;
        }

        int len = sizeof(TEST_DATA);
        send(s, (const char*)&len, 4, 0);
        send(s, (const char*)&data, len, 0);
    }
    system("pause");

    closesocket(s);
out:
#ifdef WIN32
    WSACleanup();
#endif
    return 0;
}
