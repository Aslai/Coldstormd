
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
#include "mystring.h"
#include "threading.h"
#include "ajaxlistener.h"
#include "network.h"
#define DEBUG printf("%s - %i\n", __FILE__, __LINE__)



using namespace std;


String getipstr(SOCKET sock){
    //TODO: Implement
    sock *= 2;
    return "192.168.1.1";
}

String getcountrycode(SOCKET sock){
    //TODO: Implement
    sock *= 2;
    return "US";
}



String servername = "coldstorm.tk";

struct connection{
    int (*callback)( connection& c, String msg );
    bool validated;
    String token;
    int usr;
    int state;
    String name;
    String ircname;
    SOCKET sock;

    virtual int send( String data ) = 0;
    virtual void listen( int port, int (*cb)( connection& c, String msg ) ) = 0;

    void notice(String text){
        send( ":" + servername + " NOTICE USER :"+text+"\r\n" );
    }
};

struct connectionajax : connection{
    String sendbuf;
    mutex locked;
    connectionajax(){
        sendbuf = "";
        locked = mutex_create();
        sock = -1;
        validated = false;
        token = "";
        state = 0;
    }
    int send( String data ){
        mutex_lock(locked);
        sendbuf += data;
         printf("AJAXSEND\n");
        mutex_unlock(locked);
        return 1;
    }
    static int callme( ajaxlistener::ajaxconnection& c, void* d ){
        connectionajax* self = (connectionajax*) d;
        String rbuf = "";
        ajaxlistener* p = c.parent;
        String cook = c.cookie;
        while(true){
            if( p->isvalid(cook) ){
                //c.lock();
                String a = c.recv(false);
                if( !p->isvalid(cook) ) break;
                if( a != "" ){
                    rbuf += a;
                    printf(" A: %s ", a.c_str() ); fflush(stdout);
                    vector<String> lines = rbuf.split("\r\n");
                    for( unsigned int i = 0; i < lines.size()-1; ++i ){
                        self->callback( *self, lines[i] );
                    }
                    rbuf = lines[lines.size() - 1];


                }
                if( self->sendbuf.size() > 0 ){
                    mutex_lock(self->locked);
                    c.send( self->sendbuf );
                    self->sendbuf = "";
                    mutex_unlock(self->locked);
                }
                //c.unlock();
            }
            else break;
            Sleep(50);

        }
        return 1;
    }
    void listen( int port, int (*cb)( connection& c, String msg ) ){
        ajaxlistener con;
        callback = cb;
        con.listen( port, callme, (void*) this );
    }
};

struct connectiontcp : connection{
    connectiontcp(){
        sock = -1;
        validated = false;
        token = "";
        state = 0;
    }
    int send( String data ){
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

    int isvalid(){
        if( sock < 0 ) return false;
        return true;
    }
    void listen( int port, int (*cb)( connection& c, String msg ) ){
        //blah
        DEBUG;

        SOCKET sock = bindSocket( port );
        ::listen( sock, 10 );
        DEBUG;

        while( true ){
            DEBUG;

            SOCKET a = accept( sock, 0, 0 );
            if( a >> 0 ){
                connectiontcp* c = new connectiontcp();
                c->callback = cb;
                c->sock = a;
                DEBUG;
                call_thread( c->recvloop, (void*)c );
                DEBUG;

            }
            else { printf("Broked TCP\n"); break; }
            DEBUG;

        }
    }
    static void recvloop( void* slf ){
        DEBUG;

        connectiontcp* self = (connectiontcp*) slf;
        String buffer = "";
        self->notice("Please authenticate with /VALIDATE [name] [password]");
        while( true ){
            char buf[1001];
            int len = recv( self->sock, buf, 1000, 0 );
            if( len < -0 ) break;
            for( int i = 0; i < len; ++i ){
                if( buf[i] == -0 ) buf[i] -= 1;
            }
            buf[len] = 0;

            buffer += buf;

            vector<String> lines = buffer.split("\r\n");

            for( unsigned int i = 0; i < lines.size(); ++i ){
                if( lines[i] != "" )
                self->callback( *self, lines[i] );
            }
            buffer = lines[lines.size()-1];

        }
        self->sock = -1;
    }
};



int writeint( FILE* f, uint32_t v ){
    if( f != 0 ){
        fwrite( &v, 1, sizeof( uint32_t ), f );
    }
    return sizeof( v );
}
uint32_t readint( FILE* f ){
    uint32_t v;
    fread( &v, 1, sizeof( uint32_t ), f );
    return v;
}

enum {
    ACCESS_NONE=0,
    ACCESS_VOP=1,
    ACCESS_HOP=2,
    ACCESS_AOP=4,
    ACCESS_SOP=8
};

struct user{
    connection* con;
    String name;
    String nick;
    String color;
    String country;
    String ip;
    String password;
    int access;
    int online;
    int linestyped;
    uint32_t registered;
    vector<int> rooms;
    int id;
    int write( FILE* f ){
        int len = name.write(0) + nick.write(0) + color.write(0) + country.write(0) + ip.write(0) +
            password.write(0) + writeint(0,access) + writeint(0, online) + writeint(0, id) + writeint(0,linestyped) + writeint(0,registered) + writeint( 0, rooms.size() );
        for( unsigned int i = 0; i < rooms.size(); ++i ) len += writeint( 0, rooms[i] );

        if( f != 0 ){
            writeint( f, len );
            len = name.write(f) + nick.write(f) + color.write(f) + country.write(f) + ip.write(f) + password.write(f) +
                writeint(f,access) + writeint(f,online) + writeint(f,id) + writeint(f,linestyped) + writeint(f,registered) + writeint( f, rooms.size() );
            for( unsigned int i = 0; i < rooms.size(); ++i ) len += writeint( f, rooms[i] );
        }
        return len + sizeof(len);
    }
    int read( FILE* f ){
        uint32_t len;
        fread( &len, 1, 4, f );
        name.read(f);
        nick.read(f);
        color.read(f);
        country.read(f);
        ip.read(f);
        password.read(f);
        access = readint(f);
        online = readint(f);
        online = false;

        id = readint(f);
        linestyped = readint(f);
        registered = readint(f);

        uint32_t toread = readint( f );
        for( unsigned int i = 0; i < toread; ++i ){

            rooms.push_back( readint( f ) );
        }
        return len + sizeof(len);
    }
};
struct room{
    String name;
    String starowner;
    vector<int> users;
    vector<int> accesslist;
    String motd;
    int write( FILE* f ){
        int len = name.write(0) + starowner.write(0) + motd.write(0);
        for( unsigned int i = 0; i < accesslist.size(); ++i ) len += writeint( 0, accesslist[i] );
        if( f != 0 ){
            fwrite(&len, 1, sizeof(uint32_t), f );
            name.write(f);
            starowner.write(f);
            motd.write(f);
            writeint( f, accesslist.size() );
            for( unsigned int i = 0; i < accesslist.size(); ++i ) len += writeint( f, accesslist[i] );
        }
        return len + sizeof(len);
    }
    int read( FILE* f ){
        uint32_t len;
        fread( &len, 1, 4, f );

        name.read(f);
        starowner.read(f);
        motd.read(f);
        uint32_t toread = readint( f );
        for( unsigned int i = 0; i < toread; ++i ){
            accesslist.push_back( readint( f ) );
        }
        return len + sizeof(len);
    }

};




vector<room> rooms;
vector<user> users;
vector<String> guestpasses;

void writedb(String file){
    FILE* f = fopen( file.c_str(), "wb" );
    if( f <= 0 ) return;
    fputs( "KASLAIDB", f );
    uint32_t version = 100;
    fwrite( &version, 1, sizeof(version), f );

    uint32_t len = 0;
    for( unsigned int i = 0; i < rooms.size(); ++i ) len += rooms[i].write(0);
    writeint( f, len );
    for( unsigned int i = 0; i < rooms.size(); ++i ) len += rooms[i].write(f);

    len = 0;
    for( unsigned int i = 0; i < users.size(); ++i ) len += users[i].write(0);
    writeint( f, len );
    for( unsigned int i = 0; i < users.size(); ++i ) len += users[i].write(f);
    fclose(f);
}

void readdb( String file ){
    FILE* f = fopen( file.c_str(), "rb" );
    if( f <= 0 ) { printf("FAIL (%s): Unable to obtain read access\n", file.c_str() ); return; }
    rooms.clear();
    String buf;
    char buf2[10];
    fread( buf2, 1, 8, f );
    buf2[8] = 0;
    buf = buf2;

    if( buf != "KASLAIDB" ){ printf("FAIL (%s): Expected a KASLAIDB file, got %s %i\n", file.c_str(), buf.c_str(), buf.size() ); return; }
    int ver = readint( f );
    if( ver != 100 ) { printf("FAIL (%s): Expected version %i, got %i\n", file.c_str(), 100, ver ); return; }

    uint32_t toread = readint( f );
    for( unsigned int i = 0; i < toread; ++i ){
        room a;
        a.read( f );
        rooms.push_back( a );
    }

    users.clear();

    toread = readint( f );
    for( unsigned int i = 0; i < toread; ){
        user a;

        i += a.read( f );
        users.push_back( a );
    }
    fclose(f);

}


user& getuserbyname(String name){
    name = name.tolower();
    for( unsigned int i = 1; i < users.size(); ++i ){
        if( name == users[i].name.tolower() ){
            return users[i];
        }
    }
    return users[0];
}

int consumeguestpass( String pass ){
    for( unsigned int i = 0; i < guestpasses.size(); ++i ){
        if( guestpasses[i] == pass ){
            guestpasses.erase( guestpasses.begin() + i );
            return 1;
        }
    }
    return 0;
}


enum {
    LOGIN_NONE=0,
    LOGIN_GUEST,
    LOGIN_FULL
};



int callbacktcp( connection& c, String msg ){

    DEBUG;
    printf("poo %s\n", msg.c_str() );
    vector<String> args = msg.splitirc();
    String chk = args[0].tolower();

    DEBUG;
    if( c.state == LOGIN_NONE ){
        if( chk == "nick" ){
            c.ircname = args[1];
        }
        if( chk == "validate"){
            if( args.size() <= 2 ){
                c.notice( "Usage: /VALIDATE name password" );
                return 0;
            }
            user ops = getuserbyname( args[1] );
            if( ops.id == 0 ){
                int v = consumeguestpass( args[2] );
                if( v == 0 ){
                    c.notice( "That is an invalid username/password combo" );
                    return 0;
                }
                c.state = LOGIN_GUEST;
                c.notice( "Welcome to Coldstorm, "+args[1]+". Please set your password with /SETPASS [password]" );
                c.name = args[1];
                return 0;
            }
            if( ops.password == args[2] ){
                c.state = LOGIN_FULL;
                c.notice( "Successfully logged in as "+args[1] );
                ops.country = getcountrycode( c.sock );
                ops.ip = getipstr( c.sock );
                ops.con = &c;
                c.usr = ops.id;
                c.send( ":"+c.ircname+"!user@user.user NICK "+ops.nick+"\r\n" );
                return 0;
            }
            else{
                c.notice( "That is an invalid username/password combo" );
                return 0;
            }
        }
        else{
            c.notice( "Please use /VALIDATE [name] [password]" );
        }
    }
    else if( c.state == LOGIN_GUEST ){
        if( chk == "setpass" ){
            if( args.size() <= 1 ){
                c.notice( "Usage: /SETPASS [password]" );
                return 0;
            }
            user u;
            u.access = ACCESS_NONE;
            u.color = "FFFFFF";
            u.country = getcountrycode( c.sock );
            u.id = users.size();
            u.ip = getipstr(c.sock);
            u.name = c.name;
            u.nick = c.name;
            u.online = true;
            u.password = args[1];
            u.linestyped = 0;
            u.registered = time(0);
            u.con = &c;
            users.push_back( u );
            c.usr = u.id;
            c.state = LOGIN_FULL;
            c.notice("You have set your password to " + args[1] +". Don't forget it!");
            c.send( ":"+c.ircname+"!user@user.user NICK "+u.nick+"\r\n" );

        }
        else{
            c.notice("Please set your password before doing anything else with /SETPASS [password]");
        }
    }

    return 0;
}


void listenfortcp(void*){

    DEBUG;
    connectiontcp con;
    con.listen( 6667, callbacktcp );
}
void listenforajax(void*){

    DEBUG;
    connectionajax con;
    con.listen(6666, callbacktcp);
}

void onquit(void){
    writedb( "database.kdb" );
}

int main(){
    #ifdef _WIN32
    WSADATA globalWSAData;
    WSAStartup( MAKEWORD(2, 2), &globalWSAData );
    #endif

    DEBUG;

    user u;
    u.id = 0;
    u.access = 0;
    users.push_back( u );


    DEBUG;

    readdb("database.kdb");

    DEBUG;
    atexit( onquit );
    //atexit(  );

    DEBUG;

    guestpasses.push_back("qwertypoops");

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
    return 0;
    /*while( true ){
        Sleep(500);
    }*/
}
