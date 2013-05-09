#include "network.h"

static std::vector<SOCKET> bound;

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
    sock *= 2;
    return "US";
}
