#pragma once
namespace ColdstormD{
    struct connection;
    struct connectionajax;
    struct connectiontcp;
}
#include "mystring.h"
#include "network.h"
#include "threading.h"
#include "ajaxlistener.h"
#include "constants.h"

namespace ColdstormD{
    struct connection{
        int (*callback)( connection& c, String msg );
        int (*onconnect)( connection& c );
        int (*onquit)( connection& c );

        bool validated;
        String token;
        int usr;
        int state;
        String name;
        String ircname;
        SOCKET sock;
        int introd;
        int closed;

        virtual int send( String data ) = 0;
        virtual void listen( int port, int (*cb)( connection& c, String msg ), int (*oncon)( connection& c ), int (*onq)( connection& c ) ) = 0;

        void notice(String text);
        virtual int close() = 0;
    };


    struct connectionajax : connection{
        String sendbuf;
        mutex locked;
        connectionajax();
        int send( String data );
        static int callme( ajaxlistener::ajaxconnection& c, void* d );
        void listen( int port, int (*cb)( connection& c, String msg )=0, int (*oncon)( connection& c )=0, int (*onq)( connection& c )=0  );
        int close();
    };

    struct connectiontcp : connection{
        connectiontcp();
        int send( String data );
        int isvalid();
        void listen( int port, int (*cb)( connection& c, String msg )=0, int (*oncon)( connection& c )=0, int (*onq)( connection& c )=0  );
        static void recvloop( void* slf );
        int close();
    };
}
