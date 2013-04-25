#include "http.h"

http parsehttp( String in ){
    http ret;
    vector<String> a = in.split( "\r\n" );
    if( a.size() < 2 ) {ret.data=""; ret.path=""; ret.type=""; return ret;}
    unsigned int i;
    a[0].split(" ");
    ret.type = a[0].split(" ")[0].tolower();
    ret.path = a[0].split(" ")[1];
    vector<String> args;
    if( ret.path.indexof("?") >= 0 ){
        ret.path = ret.path.split("?")[0].urldecode();
        args = a[0].split(" ")[1].split("?")[1].split("&");
    }

    for( i = 0; i < args.size(); ++i ){
        vector<String> sides = args[i].split("=");
        if( sides.size() > 0 )
            ret.urldata[sides[0].urldecode()] = sides[1].urldecode();
    }
    unsigned int pos = a[0].length() + 4;
    for( i = 1; i < a.size(); ++i ){
        if( a[i] == "" ) break;
        int pos = a[i].indexof("=");
        if( pos >= 0 ){
            ret.header[a[i].substr(0, pos)] = a[i].substr( pos+1 );
        }
        else ret.header[a[i]]="EMPTY";
        pos += a[i].length() + 2;
    }
    ret.data = in.substr(pos);
    return ret;
}

int sendstr(SOCKET sock, String data){
    unsigned int len = 0;
    while( data.size() - len > 0 ){
        int tm = send( sock, data.c_str() + len, data.size()-len, MSG_NOSIGNAL );
        if( tm <= 0 ) return tm;
        len += tm;
    }
    return data.size();
}
int sendbuf(SOCKET sock, const char* data, unsigned int lenn ){
    unsigned int len = 0;
    while( lenn - len > 0 ){
        int tm = send( sock, data + len, lenn-len, MSG_NOSIGNAL );
        if( tm <= 0 ) return tm;
        len += tm;
    }
    return lenn;
}

int sendhttppoll(SOCKET sock){
    int len = snprintf( 0, 0, "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n" );
    char* buffer = (char*) malloc( len * sizeof( char ) +1 );
    snprintf( buffer, len+1,  "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n");
    int ret = sendbuf( sock, buffer, len );
    free( buffer );
    return ret;
}

int sendhttp( SOCKET sock, int status, String contenttype, String value ){
    int len = snprintf( 0, 0, "HTTP/1.1 %i OK\r\nContent-Type: %s\r\nContent-Length: %i\r\nConnection: close\r\n\r\n", status, contenttype.c_str(), value.length() );
    char* buffer = (char*) malloc( len * sizeof( char ) +1 );
    snprintf( buffer, len+1,  "HTTP/1.1 %i OK\r\nContent-Type: %s\r\nContent-Length: %i\r\nConnection: close\r\n\r\n", status, contenttype.c_str(), value.length());
    int ret = sendbuf( sock, buffer, len );
    if( ret > 0 )
        ret = sendbuf( sock, value.c_str(), value.length() );
    free( buffer );
    closesocket( sock );
    return ret > 0;
}

int servefile( SOCKET sock, String name, int status, String contenttype ){
    FILE* f = fopen( name.c_str(), "rb" );
    if( f <= 0 ){
        sendhttp( sock, 404, "text/html", "<h1>404: File not found</h1>" );
        return 0;
    }
    const char* str = "HTTP/1.1 %i OK\r\nContent-Type: %s\r\nConnection: close\r\n\r\n";
    int len = snprintf( 0, 0, str, status, contenttype.c_str());
    char* buffer = (char*) malloc( len * sizeof( char )+1 );
    snprintf( buffer, len+1, str, status, contenttype.c_str());
    int ret = sendbuf( sock, buffer, len );
    free( buffer );
    if( ret <= 0 ) return 0;

    do{
        char buff[1000];
        int haveread = fread( buff, 1, 1000, f );
        while( haveread > 0 ){
            int tm = send( sock, buff, haveread, MSG_NOSIGNAL );
            if( tm < 0 ) break;
            haveread -= tm;
        }
    } while( !feof(f) );
    fclose( f );
    closesocket(sock);
    return 1;
}

