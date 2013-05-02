
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
#endif
#include<string>
#include<vector>
#include<map>
#include "general/mystring.h"
#include "general/threading.h"
#include "ajax/ajaxlistener.h"
#include "general/network.h"


#include "coldstormd/coldstormd.h"

using namespace std;



int main(){
    ColdstormD::run();
}
