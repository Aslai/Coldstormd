
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

enum {
    LOGIN_NONE=0,
    LOGIN_GUEST,
    LOGIN_FULL
};



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

    virtual int send( String data ) = 0;
    virtual void listen( int port, int (*cb)( connection& c, String msg ), int (*oncon)( connection& c ), int (*onq)( connection& c ) ) = 0;

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
        state = LOGIN_NONE;
        introd = false;
        onquit = 0;
        onconnect = 0;
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
        if( self->onconnect != 0 )
            self->onconnect(*self);
        String rbuf = "";
        ajaxlistener* p = c.parent;
        String cook = c.cookie.c_str();
        printf("%s", cook.c_str() );
        while(true){
                DEBUG;
            if( p->isvalid(cook) ){
                DEBUG;
                printf("%s", cook.c_str() );
        DEBUG;
                c.lock();
                DEBUG;
                String a = c.recv(false);
                DEBUG;
                if( !p->isvalid(cook) ) break;
                DEBUG;
                if( a != "" ){
                        DEBUG;
                    rbuf += a;
                        DEBUG;
                    printf(" A: %s ", a.c_str() ); fflush(stdout);
                        DEBUG;
                    vector<String> lines = rbuf.split("\r\n");
                        DEBUG;
                    for( unsigned int i = 0; i < lines.size()-1; ++i ){
                        self->callback( *self, lines[i] );
                        DEBUG;
                    }
                    rbuf = lines[lines.size() - 1];
                        DEBUG;


                }
                        DEBUG;
                printf("T");
                        DEBUG;
                if( self->sendbuf.size() > 0 ){
                        DEBUG;
                        printf("B");
                        DEBUG;
                    mutex_lock(self->locked);
                    DEBUG;
                    c.send( self->sendbuf );
                    DEBUG;
                    if( !p->isvalid(cook) ) break;
                    DEBUG;
                    self->sendbuf = "";
                    DEBUG;
                    mutex_unlock(self->locked);
                    DEBUG;
                }
                DEBUG;
                c.unlock();
                DEBUG;
            }
            else break;
            DEBUG;
            Sleep(50);

        }
        DEBUG;
        if( self->onquit != 0 )
            self->onquit(*self);
        return 1;
    }
    void listen( int port, int (*cb)( connection& c, String msg )=0, int (*oncon)( connection& c )=0, int (*onq)( connection& c )=0  ){
        ajaxlistener con;
        callback = cb;
        onconnect = oncon;
        onquit = onq;

        con.listen( port, callme, (void*) this );
    }
};

struct connectiontcp : connection{
    connectiontcp(){
        sock = -1;
        validated = false;
        token = "";
        state = 0;
        introd = false;
        onquit = 0;
        onconnect = 0;
    }
    int send( String data ){
        char* tmp = (char*)data.c_str();
        unsigned int len = data.size();
        while( len > 0 ){
            DEBUG;
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
        if( sock == 0 ) return false;
        return true;
    }
    void listen( int port, int (*cb)( connection& c, String msg )=0, int (*oncon)( connection& c )=0, int (*onq)( connection& c )=0  ){
        //blah
        DEBUG;

        SOCKET sock = bindSocket( port );
        ::listen( sock, 10 );
        DEBUG;

        while( true ){
            DEBUG;

            SOCKET a = accept( sock, 0, 0 );
            if( a > 0 ){
                connectiontcp* c = new connectiontcp();
                c->callback = cb;
                c->sock = a;
                c->onconnect = oncon;
                c->onquit = onq;

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
        if( self->onconnect != 0 )
            self->onconnect(*self);
        String buffer = "";
        self->notice("Please authenticate with /VALIDATE [name] [password]");
        while( true ){
                DEBUG;
            char buf[1001];
            int len = recv( self->sock, buf, 1000, 0 );
            if( len < -0 ) break;
            for( int i = 0; i < len; ++i ){
                DEBUG;
                if( buf[i] == -0 ) buf[i] -= 1;
            }
            buf[len] = 0;

            buffer += buf;

            vector<String> lines = buffer.split("\r\n");

            for( unsigned int i = 0; i < lines.size(); ++i ){
                    DEBUG;
                if( lines[i] != "" )
                self->callback( *self, lines[i] );
            }
            buffer = lines[lines.size()-1];

        }
        self->sock = -1;
        if( self->onquit != 0 )
            self->onquit(*self);
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

int validatechannelname(String name){
    if( name.length() > 20 ) return 0;
    if( name[0] != '#' ) return 0;
    if( (signed)(name.find_first_of(" \t\r\n")) >= 0 ) return 0;
    return 1;
}

int validateusername(String name){
    DEBUG;
    if( name.length() > 20 ) return 0;
    DEBUG;
    if( (signed)(name.find_first_of(" \t\r\n")) >= 0 ) return 0;
    DEBUG;
    String beans = "1234567890`-_=|}]{[qwertyuiopasdfghjklzxcvbnm<>,.?/!^*()";
    DEBUG;
    for( unsigned int i = 0; i < name.length(); ++i ){
        DEBUG;
        if( (signed)(beans.find_first_of(tolower(name[i]))) < (signed)0 ) return 0;
        DEBUG;
    }
    DEBUG;
    return 1;
}


enum {
    ACCESS_NONE=0,
    ACCESS_VOP=1,
    ACCESS_HOP=2,
    ACCESS_AOP=4,
    ACCESS_SOP=8,
    ACCESS_BANNED=16
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
    int echo;
    uint32_t registered;
    vector<unsigned int> rooms;
    int id;
    int joinroom(String room, bool overrideblocks=false);
        int privmsg( int usr, String msg );
    int notice( int usr, String msg );

    String getmask(){
        printf("%s\n", nick.c_str() );
        printf("%s\n", color.c_str() );
        printf("%s\n", country.c_str() );
        printf("%s\n", ip.c_str() );

        return nick+"!"+color+country+"@"+ip;
    }

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
            DEBUG;
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
    vector<int> usershave;
    String motd;
    int motdsetby;
    int motdseton;
    int canjoin( int usr );
    int adduser( int usr, int access = ACCESS_NONE );
    int partuser( int usr );
    int removeuser( int usr );
    int privmsg( int usr, String msg );
    int notice( int usr, String msg );
    int sendinfo(int usr);
    int sendwho(int usr);


    void broadcast(int usr, String message, bool supressecho = true);

    int write( FILE* f ){
        int len = name.write(0) + starowner.write(0) + motd.write(0) + writeint(0, motdsetby) + writeint(0, motdseton) ;
        for( unsigned int i = 0; i < accesslist.size(); ++i ) len += writeint( 0, accesslist[i] );
        if( f != 0 ){
            fwrite(&len, 1, sizeof(uint32_t), f );
            name.write(f);
            starowner.write(f);
            motd.write(f);
            writeint(f, motdsetby);
            writeint(f, motdseton);
            writeint( f, accesslist.size() );
            for( unsigned int i = 0; i < accesslist.size(); ++i ) len += writeint( f, accesslist[i] );
        }
        return len + sizeof(len);
    }
    int read( FILE* f ){
        DEBUG;
        uint32_t len;
        DEBUG;
        fread( &len, 1, 4, f );
        DEBUG;

        name.read(f);
        DEBUG;
        starowner.read(f);
        DEBUG;
        motd.read(f);
        motdsetby = readint(f);
        motdseton = readint(f);

        DEBUG;
        uint32_t toread = readint( f );
        DEBUG;
        for( unsigned int i = 0; i < toread; ++i ){
            DEBUG;
            accesslist.push_back( readint( f ) );
            DEBUG;

        }
        DEBUG;
        return len + sizeof(len);
    }

};




vector<room> rooms;
vector<user> users;
vector<String> guestpasses;


void room::broadcast(int usr, String message, bool supressecho){
    printf("ba\n");
    for( unsigned int i = 0; i < usershave.size(); ++i ){
        if( supressecho )
            if( usershave[i] == usr && ::users[usershave[i]].echo == false ) continue;
        printf("Broadcast from %s to %s\n", name.c_str(), ::users[usershave[i]].nick.c_str() );
        ::users[usershave[i]].con->send(message);
    }
}
int room::canjoin( int usr ){
        for( unsigned int i = 0; i < users.size(); ++i ){
            DEBUG;
            if( ::users[users[i]].id == usr ){
                if( (accesslist[i] & ACCESS_BANNED) != 0 )
                    return 0;
                return 1;
            }
        }
        return 1;
    }

String I(int in){
    int l = snprintf(0, 0, "%i", in);
    String ret;
    ret.resize( l+1 );
    snprintf((char*)ret.c_str(), l+1, "%i", in);
    return ret;
}

int room::sendwho(int usr){
    for( unsigned int i = 0; i < usershave.size(); ++i ){
        String prep="";
        for( unsigned int j = 0; j < users.size(); ++j ){
            if( users[j] == usershave[i] ){
                switch( accesslist[j] ){
                    case ACCESS_VOP: prep = "+"; break;
                    case ACCESS_HOP: prep = "#"; break;
                    case ACCESS_AOP: prep = "@"; break;
                    case ACCESS_SOP: prep = "~"; break;
                }
            }
        }
        ::users[usr].con->send(":"+servername+" 352 "+::users[usr].nick+" "+name
                               +" "+::users[usr].color+" "+::users[usr].ip+" "+servername+" "
                               +::users[usr].nick+" H"+prep+" "+::users[usr].name+"\r\n");
    }
    return 1;
}

int room::sendinfo(int usr){
    ::users[usr].con->send(":"+servername+" 332 "+::users[usr].nick+" "+name+" :"+motd+"\r\n");
    ::users[usr].con->send(":"+servername+" 333 "+::users[usr].nick+" "+name+" "+::users[motdsetby].getmask()+" "+I(motdseton)+"\r\n");
    String s = ":"+servername+" 353 "+::users[usr].nick+" = "+name+" :";
    for( unsigned int i = 0; i < usershave.size(); ++i ){
        String prep="";
        for( unsigned int j = 0; j < users.size(); ++j ){
            if( users[j] == usershave[i] ){
                switch( accesslist[j] ){
                    case ACCESS_VOP: prep = "+"; break;
                    case ACCESS_HOP: prep = "#"; break;
                    case ACCESS_AOP: prep = "@"; break;
                    case ACCESS_SOP: prep = "~"; break;
                }
            }
        }
        s += prep + ::users[usershave[i]].nick+" ";
    }
    ::users[usr].con->send(s+"\r\n");
    sendwho(usr);
    return 1;
}

int room::adduser( int usr, int access ){
    if( !canjoin(usr) ) return 0;

    for( unsigned int i = 0; i < usershave.size(); ++i ){
            DEBUG;
        if( ::users[usershave[i]].id == usr ){
            return 0;
        }
    }
    for( unsigned int i = 0; i < users.size(); ++i ){
        DEBUG;
        if( ::users[users[i]].id == usr ){
            usershave.push_back(usr);
            DEBUG;


            broadcast(usr, ":"+::users[usr].getmask()+" JOIN "+name+"\r\n", false);
            sendinfo(usr);
            return 1;
        }
    }

    users.push_back(usr);
    accesslist.push_back(access);
    usershave.push_back(usr);
    DEBUG;
    broadcast(usr, ":"+::users[usr].getmask()+" JOIN "+name+"\r\n", false);
    sendinfo(usr);
    //::users[usr].con->send( ":" + ::users[usr].nick + "!user@user JOIN "+name+"\r\n");
    return 1;
}
int room::partuser(int usr){
    for( unsigned int i = 0; i < usershave.size(); ++i ){
            DEBUG;
        if( usershave[i] == usr ){
            broadcast(usr, ":" + ::users[usr].getmask() + " PART "+name+"\r\n", false);
            usershave.erase( usershave.begin() + i );
            return 1;
        }
    }
    return 0;
}
int room::removeuser(int usr){
    partuser(usr);
    for( int i = 0; i < users[i]; ++i ){
            DEBUG;
        if( users[i] == usr ){
            users.erase( users.begin() + i );
            return 1;
        }
    }
    return 0;
}

int room::privmsg( int usr, String msg ){
    broadcast(usr, ":"+::users[usr].getmask()+" PRIVMSG "+name+" :" + msg+"\r\n" );
    return 1;
}
int room::notice( int usr, String msg ){
    broadcast(usr, ":"+::users[usr].getmask()+" NOTICE "+name+" :" + msg+"\r\n" );
    return 1;
}
int user::privmsg( int usr, String msg ){
    con->send(":"+::users[usr].getmask()+" PRIVMSG "+nick+" :" + msg+"\r\n" );
    return 1;
}
int user::notice( int usr, String msg ){
    con->send(":"+::users[usr].getmask()+" NOTICE "+nick+" :" + msg+"\r\n" );
    return 1;
}



int user::joinroom(String room, bool overrideblocks){
    if( !overrideblocks ){
        if( validatechannelname(room) == 0 ){
            con->send( ":" + nick + "!user@user ERROR CHANNAME "+room+
                " :Erroneous Channel Name :"+room+"\r\n");
            return 0;
        }
        for( unsigned int i = 0; i < rooms.size();++i){
                DEBUG;
            if( rooms[i] < ::rooms.size() ){
                if( ::rooms[rooms[i]].name.tolower() == room.tolower() ){

                    con->send( ":" + nick + "!user@user ERROR PRESENT "+::rooms[rooms[i]].name+
                             " :Cannot join "+::rooms[rooms[i]].name+" for you are already there\r\n");
                    return 0;
                }
            }
            else break;
        }
    }
    for( unsigned int i = 0; i < rooms.size();++i){
            DEBUG;
        if( rooms[i] < ::rooms.size() ){
            if( ::rooms[rooms[i]].name.tolower() == room.tolower() ){
                struct room &r = ::rooms[rooms[i]];
                for( unsigned int j = 0; j <r.usershave.size(); ++j ){
                    if( r.usershave[j] == id ){
                        printf("USER: %i\n", r.usershave[j]);
                        con->send( ":" + nick + "!user@user ERROR PRESENT "+::rooms[rooms[i]].name+
                                " :Cannot join "+::rooms[rooms[i]].name+" for you are already there\r\n");
                        return 0;
                    }
                }
                break;

            }
        }
        else break;
    }

    for( unsigned int i = 0; i < ::rooms.size();++i){
            DEBUG;
        if( ::rooms[i].name.tolower() == room.tolower() ){
            if( ::rooms[i].canjoin(id) ){
                rooms.push_back( i );
                //con->send( ":" + nick + "!user@user JOIN "+::rooms[i].name+"\r\n");
                return ::rooms[i].adduser(id);
            }
            else {
                con->send( ":" + nick + "!user@user ERROR BAN "+::rooms[i].name+
                " :Cannot join "+::rooms[i].name+" for you are banned\r\n");
                return -1;
            }
        }
    }
    struct room a;
    a.motd = "";
    //a.users.push_back( id );
    //a.accesslist.push_back( ACCESS_SOP );
    a.name = room;
    a.starowner = "";
    a.motdsetby = 0;
    a.motdseton = 0;
    ::rooms.push_back( a );
    rooms.push_back( ::rooms.size()-1 );
    return ::rooms[::rooms.size()-1].adduser(id,ACCESS_SOP);
}

void writedb(String file){
    FILE* f = fopen( file.c_str(), "wb" );
    if( f == 0 ) return;
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
    if( f == 0 ) { printf("FAIL (%s): Unable to obtain read access\n", file.c_str() ); return; }
    rooms.clear();
    String buf;
    char buf2[10];
    fread( buf2, 1, 8, f );
    buf2[8] = 0;
    buf = buf2;

    if( buf != "KASLAIDB" ){ printf("FAIL (%s): Expected a KASLAIDB file, got %s %i\n", file.c_str(), buf.c_str(), buf.size() ); return; }
    int ver = readint( f );
    if( ver != 100 ) { printf("FAIL (%s): Expected version %i, got %i\n", file.c_str(), 100, ver ); return; }
DEBUG;
    uint32_t toread = readint( f );
    DEBUG;
    for( unsigned int i = 0; i < toread; ){
        DEBUG;
        room a;
        DEBUG;
        i+=a.read( f );
       DEBUG;
        rooms.push_back( a );
    }
DEBUG;
    users.clear();
DEBUG;
    toread = readint( f );
    DEBUG;
    for( unsigned int i = 0; i < toread; ){
        DEBUG;
        user a;
DEBUG;
        i += a.read( f );
        DEBUG;
        users.push_back( a );
        DEBUG;
    }
    DEBUG;
    fclose(f);

}


/*user& getuserbyname(String name){
    name = name.tolower();
    for( unsigned int i = 1; i < users.size(); ++i ){
        if( name == users[i].name.tolower() ){
            return users[i];
        }
    }
    return users[0];
}*/
int getroombyname(String name){
    for( unsigned int i = 0; i < rooms.size(); ++i ){
            DEBUG;
        if( rooms[i].name.tolower() == name.tolower() ){
            return i;
        }
    }
    return -1;
}

int getuserbyname(String name){
    for( unsigned int i = 0; i < users.size(); ++i ){
            DEBUG;
        if( users[i].nick.tolower() == name.tolower() ){
            return i;
        }
    }
    return -1;
}

int consumeguestpass( String pass ){
    for( unsigned int i = 0; i < guestpasses.size(); ++i ){
            DEBUG;
        if( guestpasses[i] == pass ){
            guestpasses.erase( guestpasses.begin() + i );
            return 1;
        }
    }
    return 0;
}

int intro(connection& c, String name ){
    if( c.introd == true ) return 0;
    c.introd = true;
    c.send( ":"+servername+" 005 "+name+" CHANMODES=b, CHANTYPES=# CHARSET=utf-8 AWAYLEN=10 KICKLEN=256 MAXBANS=200 MAXCHANNELS=60 MAXPARA=32 :are supported by this server\r\n" );
    c.send( ":"+servername+" 005 "+name+" NETWORK=Coldstorm NICKLEN=20 PREFIX=(sohv)~@#+ STATUSMSG=~@#+ TOPICLEN=200 :are supported by this server\r\n" );
    return 1;
}

int onmsgnewsession( connection& c, vector<String> args ){
    String chk = args[0].tolower();
    if( chk == "nick" ){
        c.ircname = args[1];
    }
    if( chk == "validate"){
        if( args.size() <= 2 ){
            c.notice( "Usage: /VALIDATE name password" );
            return 0;
        }
        if( validateusername(args[1]) == 0){
            c.notice( "Erroneous name" );
            return 0;
        }
        int us = getuserbyname( args[1] );

        user& ops = users[us<0?0:us];
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
            ops.echo = 0;
            c.send( ":"+c.ircname+"!user@user.user NICK "+ops.nick+"\r\n" );
            vector<unsigned int> t = ops.rooms;
            ops.rooms.clear();
            for( unsigned int i = 0; i< t.size(); ++i ){
                    DEBUG;
                if( t[i] < rooms.size() ){
                    users[ops.id].joinroom(rooms[t[i]].name,true);
                   /* switch( ::rooms[t[i]].canjoin(ops.id) > 0 ){
                        case 1: c.send( ":" + ops.nick + "!user@user JOIN "+rooms[t[i]].name+"\r\n"); break;
                        case 0: c.send( ":" + ops.nick + "!user@user ERROR BAN "+rooms[t[i]].name+" :Cannot join "+rooms[t[i]].name+" for you are banned\r\n"); break;
                    }*/
                }
            }
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

    return 0;
}

int onmsgguest( connection& c, vector<String> args ){
    String chk = args[0].tolower();
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
        u.echo = 0;

        c.usr = u.id;
        c.state = LOGIN_FULL;
        c.notice("You have set your password to " + args[1] +". Don't forget it!");
        DEBUG;
        c.send( ":"+c.ircname+"!user@user.user NICK "+u.nick+"\r\n" );
        users.push_back( u );

        DEBUG;
        users[u.id].joinroom("#Coldstorm");
        DEBUG;
        users[u.id].joinroom("#2");
        DEBUG;

        DEBUG;

    }
    else{
        c.notice("Please set your password before doing anything else with /SETPASS [password]");
    }
    return 0;
}




int ping( int usr, String message ){
    ::users[usr].con->send("PONG :"+message+"\r\n");
    return 1;
}
int pong( int usr, String message ){
    //::users[usr].con->send("PONG :"+message+"\r\n");
    message[0]=message[0];
    usr*=2;
    return 1;
}
int privmsg( int usr, String target, String message ){
    printf("PM to %s\n", target.c_str() );
    if( target[0] == '#' )
    {
         printf("PM to room %s\n", target.c_str() );
        int t = getroombyname(target);
        if( t >= 0 ){
            printf("sending\n");
            return rooms[t].privmsg(usr, message);
        }
    }
    else{
            printf("PM to user%s\n", target.c_str() );
        int t = getuserbyname(target);
        if( t >= 0 ){
                printf("sending\n");
            return users[t].privmsg(usr, message);
        }
    }
    //::users[usr].con->send("PONG :"+message+"\r\n");
    return 1;
}

int whois( int usr, String message ){
    //::users[usr].con->send("PONG :"+message+"\r\n");
    int found = 0;
    for( int i = 0; i < users.size(); ++i ){
        if( message.tolower() == users[i].nick.tolower() ){
            found = 1;
            users[usr].con->send(":"+servername+" 311 "+users[usr].nick+" "+message+" "+users[usr].color+" "+users[usr].ip+" * :"+users[usr].name+"\r\n");
        }
    }
    if( found == 0 ){
        users[usr].con->send(":"+servername+" 401 "+users[usr].nick+" "+message+" :No such nick/channel\r\n");
    }
    users[usr].con->send(":"+servername+" 318 "+users[usr].nick+" * :End of /WHOIS list.\r\n");

    return 1;
}


int onmsgfull( connection& c, vector<String> args ){
    String chk = args[0].tolower();
    if( chk == "join" && args.size() > 1 ){
        users[c.usr].joinroom(args[1]);
    } else if( chk == "ping" && args.size() > 1 ){
        ping(c.usr, args[1] );
    } else if( chk == "pong" && args.size() > 1 ){
        pong(c.usr, args[1] );
    } else if( chk == "privmsg" && args.size() > 2 ){
        privmsg(c.usr, args[1], args[2] );
    } else if( chk == "whois" && args.size() > 1 ){
        whois(c.usr, args[1] );
    } else if( chk == "" && args.size() > 2 ){

    }

    return 0;
}


int callbacktcp( connection& c, String msg ){

    DEBUG;
    printf("poo %s\n", msg.c_str() );
    vector<String> args = msg.splitirc();
    String chk = args[0].tolower();

    DEBUG;
    printf("LOGIN STATE: %i\n", c.state );
    if( c.state == LOGIN_NONE ){
        return onmsgnewsession( c, args );
    }
    else if( c.state == LOGIN_GUEST ){
        return onmsgguest( c, args );
    }
    else if( c.state == LOGIN_FULL ){
        return onmsgfull( c, args );
    }

    return 0;
}

int onconnect(connection& c){
    printf("\n\n\nfag\n\n\n");
    return intro( c, "Auth" );
}

void listenfortcp(void*){

    DEBUG;
    connectiontcp con;
    con.listen( 6667, callbacktcp, onconnect );
}
void listenforajax(void*){

    DEBUG;
    connectionajax con;
    con.onconnect = onconnect;
    con.listen(81, callbacktcp, onconnect);
}

void onquit(void){
    writedb( "database.kdb" );
}

/*int makeroom(String name){
    struct room a;
    a.motd = "";
    a.name = room;
    a.starowner = "";
    ::rooms.push_back( a );
    rooms.push_back( ::rooms.size()-1 );
}*/

int main(){
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
    exit(0);
    return 0;
    /*while( true ){
        Sleep(500);
    }*/
}
