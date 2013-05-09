#include "network.h"

static std::vector<SOCKET> bound;


static String readsock(SOCKET sock){
    String ret = "";
    unsigned int len = 1000;
    unsigned int actual = 0;
    while(1){
        len *= 2;
        ret.reserve(len);
        ret.resize(len);
        int tm = recv( sock, (char*)ret.c_str()+actual, len-actual, 0 );
        if( tm >= 0 ) actual += tm;
        if( ret.indexof("\r\n\r\n") >= 0 ) break;
        if( tm <= 0 ) break;


    }
    ret.resize(actual);
    return ret;
}
static int sendstr(SOCKET sock, String data){
    unsigned int len = 0;
    while( data.size() - len > 0 ){
        int tm = send( sock, data.c_str() + len, data.size()-len, MSG_NOSIGNAL );
        if( tm <= 0 ) return tm;
        len += tm;
    }
    return data.size();
}
String request(String url){
    if( url.tolower().substr(0, 7) == "http://" ){
        url = url.substr(7);
    }
    String addr = url.substr(0, url.indexof("/")-1 );
    int port = 80;
    addr = addr.split(":")[0];
    String git = url.substr(url.indexof("/")+1);
    String snd =    "GET "+git+" HTTP/1.1\r\n"+
                    "Host: "+addr+"\r\n\r\n";
    SOCKET s = connectTCP( resolveAddr(addr.c_str()), port);
    sendstr( s, snd );
    String v = readsock( s );
    return v.substr(v.indexof("\r\n\r\n")+4);

}

SOCKET bindSocket( int port, unsigned long addr )
{
        SOCKET m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_socket == 0) {return 0;}
        sockaddr_in service;

        service.sin_family = AF_INET;
        service.sin_addr.s_addr = addr;
        service.sin_port = htons( port );
        if (bind(m_socket,(sockaddr*) (&service), sizeof(service)) < 0) {
            //closesocket(m_socket);
            return 0;
        }
        //if( m_socket > 0 )
            //bound.push_back( m_socket );
        return m_socket;
}

unsigned long getip( SOCKET sock ){
    sockaddr_in client_info;
    socklen_t p = sizeof(client_info);
    getpeername(sock, (sockaddr*)&client_info, &p);
    in_addr ip =  client_info.sin_addr;
    return ip.s_addr;
}

void socketcleanup( void ){
    for( unsigned int i = 0; i < bound.size(); ++i ){
        shutdown(bound[i], SHUT_RDWR);
        closesocket(bound[i]);
    }
}

String getipstr(SOCKET sock){
    //TODO: Implement
    char buf[2048];
    sockaddr_in client_info;
    socklen_t p = sizeof(client_info);
    getpeername(sock, (sockaddr*)&client_info, &p);

    #ifdef _WIN32
        unsigned long int siz =  sizeof(client_info);
        unsigned long int siz2 = 2048;

        WSAAddressToString((sockaddr*)&client_info,siz, 0, buf, &siz2 );

    #else
        sockaddr_in* sin = &client_info;
        inet_ntop(sin->sin_family, &sin->sin_addr, buf, sizeof(buf));
    #endif

    String r = buf;
    return r;
}

String getcountrycode(SOCKET sock){
    //TODO: Implement
    //http://www.geoplugin.net/json.gp?ip=dottedip
    /*String r =request( "http://www.geoplugin.net/json.gp?ip=" + getipstr(sock) ).tolower();
    int p = r.indexof("countrycode");
    if( p > 0 ){
        r.indexof("\"", p);
    }*/
    return "US";
}


unsigned long resolveAddr( const char* addr, int ind )
{
    hostent* host = gethostbyname( addr );
    if( host == 0 )
    {
        printf( "Failed to lookup %s.", addr );
        return 0;
    }
    if( ind > host->h_length ) return -1;
    unsigned long     myhost = *((unsigned long*) host->h_addr);
    return myhost;
}

SOCKET connectTCP( unsigned long ip, short int port )
{
    SOCKET ret;
    ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( ret < 0 ) return 0;
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = ip;
    clientService.sin_port = htons( port );
    int result = connect( ret, (sockaddr*) &clientService, sizeof(clientService));
    if( result < 0 ) return 0;
    return ret;
}
