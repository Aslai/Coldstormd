#pragma once
#include<stdarg.h>
#include<stdlib.h>

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<time.h>
#ifdef _WIN32
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include<process.h>
    #define socklen_t int
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <termios.h>
    #include <pthread.h>
    #include <errno.h>
    #include <signal.h>
    #define SOCKET int
    #define Sleep(a) usleep(a*1000)
    #define closesocket(sock) close( sock )
    //#define SOCKADDR_IN sockaddr_in
#endif
#include<string>
#include<vector>
#include<map>
#include<deque>
#include "mystring.h"
#include "http.h"
#include "threading.h"
#include "network.h"

using namespace std;

struct ajaxlistener{
    struct ajaxconnection{
        ajaxlistener* parent;
        string cookie;
        SOCKET polling;
        mutex locked;
        unsigned long ip;
        unsigned long timesincelast;
        unsigned long timeout;
        int state;
        ajaxconnection();
        deque<String> dataqueue;
        String sendqueue;
        void lock();
        void unlock();
        int send(string data);
        String recv(int block = true);
        int close();
        void push( SOCKET sock, String data );
        int updatetime(int val);
        ~ajaxconnection();
    };
    struct cbargs{
        ajaxlistener* self;
        ajaxconnection* con;
        void* data;
    };
    map<string,ajaxconnection*> connections;
    SOCKET sock;
    void* data;
    int (*callback)(ajaxconnection&,void*);
    ajaxlistener();
    int listen( int port, int (*cb)(ajaxconnection&,void*), void* );
    ajaxconnection& getconnection( string cookie );
    int close();
    int remove( String cookie );
    static String generatecookie(SOCKET sock);
    int isvalid(String cookie);

    static void runcallback(void* sel);
    void docallback( ajaxconnection* c, void* d );
};
