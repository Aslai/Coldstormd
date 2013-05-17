#include "connection.h"

namespace ColdstormD{
    void connection::notice(String text){
        send( ":" + alertname + " NOTICE USER :"+text+"\r\n" );
    }
    connectionajax::connectionajax(){
        sendbuf = "";
        locked = mutex_create();
        sock = -1;
        validated = false;
        token = "";
        state = LOGIN_NONE;
        introd = false;
        onquit = 0;
        onconnect = 0;
        closed = 0;
        usr = 0;
    }
    int connectionajax::send( String data ){
        mutex_lock(locked);
        sendbuf += data;
         printf("AJAXSEND\n");
        mutex_unlock(locked);
        return 1;
    }
    int connectionajax::callme( ajaxlistener::ajaxconnection& c, void* d ){
        connectionajax* self = (connectionajax*) d;
        if( self->onconnect != 0 )
            self->onconnect(*self);
        String rbuf = "";
        ajaxlistener* p = c.parent;
        String cook = c.cookie.c_str();
        printf("%s", cook.c_str() );
        while(!self->closed){
            if( p->isvalid(cook) ){
                printf("%s", cook.c_str() );
                c.lock();
                String a = c.recv(false);
                if( !p->isvalid(cook) ) break;
                if( a != "" ){
                    rbuf += a;
                    printf(" A: %s ", a.c_str() ); fflush(stdout);
                    vector<String> lines = rbuf.split("\r\n");
                    if( lines[0].length() > 2000 || rbuf.length() > 4000 )
                        self->close();
                    for( unsigned int i = 0; i < lines.size()-1; ++i ){
                        self->callback( *self, lines[i] );
                    }
                    rbuf = lines[lines.size() - 1];

                }
                printf("T");
                if( self->sendbuf.size() > 0 ){
                        printf("B");
                    mutex_lock(self->locked);
                    c.send( self->sendbuf );
                    if( !p->isvalid(cook) ) break;
                    self->sendbuf = "";
                    mutex_unlock(self->locked);
                }
                c.unlock();
            }
            else break;
            Sleep(100);
        }
        if( self->onquit != 0 )
            self->onquit(*self);
        return 1;
    }
    void connectionajax::listen( int port, int (*cb)( connection& c, String msg ), int (*oncon)( connection& c ), int (*onq)( connection& c )  ){
        ajaxlistener con;
        callback = cb;
        onconnect = oncon;
        onquit = onq;
        con.listen( port, callme, (void*) this );
    }
    int connectionajax::close(){
        closed = 1;
        return 1;
    }


    connectiontcp::connectiontcp(){
        sock = -1;
        validated = false;
        token = "";
        state = 0;
        introd = false;
        onquit = 0;
        onconnect = 0;
        closed = 0;
        usr = 0;

    }
    int connectiontcp::send( String data ){
        char* tmp = (char*)data.c_str();
        unsigned int len = data.size();
        while( len > 0 ){
            int diff = ::send( sock, tmp, len, MSG_NOSIGNAL );
            if( diff < 0 ){
                sock = -1;
                return -1;
            }
            len -= diff;
            tmp += diff;
        }
        return 0;
    }
    int connectiontcp::isvalid(){
        if( sock == 0 ) return false;
        return true;
    }
    void connectiontcp::listen( int port, int (*cb)( connection& c, String msg ), int (*oncon)( connection& c ), int (*onq)( connection& c )  ){
        SOCKET sock = bindSocket( port );
        ::listen( sock, 10 );

        while( true ){

            SOCKET a = accept( sock, 0, 0 );
            if( a > 0 ){
                connectiontcp* c = new connectiontcp();
                c->callback = cb;
                c->sock = a;
                c->onconnect = oncon;
                c->onquit = onq;
                call_thread( c->recvloop, (void*)c );
            }
            else { printf("Broked TCP\n"); break; }

        }
    }
    void connectiontcp::recvloop( void* slf ){

        connectiontcp* self = (connectiontcp*) slf;
        if( self->onconnect != 0 )
            self->onconnect(*self);
        String buffer = "";
        self->notice("Please authenticate with /VALIDATE [name] [password]");
        while( true ){
            Sleep(100);
            char buf[1001];
            int len = recv( self->sock, buf, 1000, 0 );
            DEBUG;
            if( len <= -0 ) {
                DEBUG;
                break;
            }
            for( int i = 0; i < len; ++i ){
                if( buf[i] == -0 ) buf[i] -= 1;
            }
            buf[len] = 0;
            buffer += buf;
            vector<String> lines = buffer.split("\r\n");
            if( lines[0].length() > 2000 || buffer.length() > 4000 )
                self->close();
            for( unsigned int i = 0; i < lines.size(); ++i ){
                if( lines[i] != "" )
                self->callback( *self, lines[i] );
            }
            buffer = lines[lines.size()-1];
        }
        self->sock = 0;
        DEBUG;
        if( self->onquit != 0 )
            self->onquit(*self);
    }
    int connectiontcp::close(){
        closed = 1;
        closesocket( sock );
        return 1;
    }

}

