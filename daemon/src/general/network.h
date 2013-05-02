#pragma once
#include<stdlib.h>
#include<stdio.h>

#include<ctype.h>
#ifdef _WIN32
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define MSG_NOSIGNAL 0
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
    #define SOCKET int
    #define closesocket(sock) ::close( sock )
#endif
#include<vector>
#include "mystring.h"

SOCKET bindSocket( int port );
unsigned long getip( SOCKET sock );
void socketcleanup( void );
String getipstr(SOCKET sock);
String getcountrycode(SOCKET sock);
