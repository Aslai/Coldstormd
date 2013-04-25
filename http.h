#pragma once
#include<stdlib.h>
#include<stdio.h>

#include<ctype.h>
#include "mystring.h"
#include "network.h"
#include<map>

using namespace std;

struct http{
    String type;
    String path;
    map<String, String> header;
    map<String, String> urldata;
    String data;
};

http parsehttp( String in );

int sendstr(SOCKET sock, String data);
int sendbuf(SOCKET sock, const char* data, unsigned int lenn );
int sendhttppoll(SOCKET sock);
int sendhttp( SOCKET sock, int status, String contenttype, String value );
int servefile( SOCKET sock, String name, int status=200, String contenttype = "text/html" );

