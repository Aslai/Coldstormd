#include "coldstormd.h"
namespace ColdstormD{
    int run(){
        #ifdef _WIN32
        WSADATA globalWSAData;
        WSAStartup( MAKEWORD(2, 2), &globalWSAData );
        #endif

        DEBUG;

        user u;
        u.id = 0;
        u.access = 0;
        u.nick = u.name = u.ip = u.color = "";
        users.push_back( u );


        DEBUG;

        readdb("database.kdb");

        DEBUG;
        //atexit( onquit );
        //atexit( socketcleanup );

        DEBUG;

        guestpasses.push_back("1");
        guestpasses.push_back("2");
        guestpasses.push_back("3");
        guestpasses.push_back("4");
        guestpasses.push_back("5");
        guestpasses.push_back("6");


        DEBUG;


        call_thread( listenfortcp, 0 );

        DEBUG;
        call_thread( listenforajax, 0 );

        DEBUG;
        char test[100];

        DEBUG;
        gets(test);

        DEBUG;
        socketcleanup();
        onquit();
        return 0;
    }
}
