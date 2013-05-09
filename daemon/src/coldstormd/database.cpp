#include "database.h"
#define VERSION_DATABASE 106
namespace ColdstormD{
    void writedb(String file){
        FILE* f = fopen( file.c_str(), "wb" );
        if( f == 0 ) return;
        fputs( "KASLAIDB", f );
        uint32_t version = VERSION_DATABASE;
        fwrite( &version, 1, sizeof(version), f );

        uint32_t len = 0;
        for( unsigned int i = 0; i < rooms.size(); ++i ) len += rooms[i].write(0);
        writeint( f, len );
        for( unsigned int i = 0; i < rooms.size(); ++i ) len += rooms[i].write(f);

        len = 0;
        for( unsigned int i = 0; i < users.size(); ++i ) len += users[i].write(0);
        writeint( f, len );
        for( unsigned int i = 0; i < users.size(); ++i ) len += users[i].write(f);
        fflush(f);
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

        if( buf != "KASLAIDB" ){ printf("FAIL (%s): Expected a KASLAIDB file, got %s %i\n", file.c_str(), buf.c_str(), buf.size() ); fclose(f); return; }
        int ver = readint( f );
        if( ver != VERSION_DATABASE ) { printf("FAIL (%s): Expected version %i, got %i\n", file.c_str(), VERSION_DATABASE, ver ); fclose(f); return; }
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
}
