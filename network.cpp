#include "network.h"

static std::vector<SOCKET> bound;

SOCKET bindSocket( int port )
{
        SOCKET m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_socket < 0) {return 0;}
        sockaddr_in service;

        service.sin_family = AF_INET;
        service.sin_addr.s_addr = INADDR_ANY;//inet_addr( ip );
        service.sin_port = htons( port );
        if (bind(m_socket,(sockaddr*) (&service), sizeof(service)) < 0) {
            //closesocket(m_socket);
            return 0;
        }
        if( m_socket > 0 )
            bound.push_back( m_socket );
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
        closesocket(bound[i]);
    }
}
