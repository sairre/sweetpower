#ifndef _PLATFORM_H
#define _PLATFORM_H

#ifdef _WIN32
#include <WinSock2.h>
#include <windows.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "WSOCK32.LIB")
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET sw_sock;
#define sw_error_sock INVALID_SOCKET
#else
typedef int sw_sock; 
#define sw_error_sock -1

#endif

#endif