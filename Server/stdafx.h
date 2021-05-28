#pragma once

#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);
