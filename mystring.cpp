#include "mystring.h"
String::String() :string(){

}
String::String(string a) : string( a ){

}
String::String(const char* a) : string( a ){

}

String& String::operator=(string a){
    string::operator=(a);
    return *this;
}
String& String::operator=(const char* a){
    string::operator=(a);
    return *this;
}


int String::indexof( String what, int pos ){
    unsigned int count = 0;
    for( unsigned int i = pos; i < this->size();++i){
        if( operator[](i) == what[count] ){
            count++;
        }
        else {
            if( count > 0 )
                i -= count-1;
            count = 0;
        }
        if( count == what.size() )
            return i - count + 1;
    }
    return -1;
}

vector<String> String::split(String delimiter){
    vector<String> ret;
    int pos = 0;
    while( pos >= 0 ){
        int p2 = this->indexof( delimiter, pos );
        if( p2 >= 0)
            ret.push_back( this->substr(pos, p2-pos ) );
        else
            ret.push_back( this->substr(pos) );
        pos = p2 + delimiter.size();
        if( p2 < 0 ) pos = p2;
    }
    return ret;
}
vector<String> String::splitirc(){
    vector<String> ret = split( " " );
    int l = indexof(" :");
    if( l < 0 ) return ret;
    unsigned int i;
    for( i = 0; ret[i][0] != ':'; ++i ){}
    ret.resize( i+1 );
    ret[i] = substr( l+1 );
    return ret;
}

String String::tolower(){
    String tm = *this;
    char* in = &((tm)[0]);
    while( *in != 0 ) {*in = ::tolower(*in); in++; }
    return tm;
}
String String::toupper(){
    String tm = *this;
    char* in = &((tm)[0]);
    while( *in != 0 ) {*in = ::toupper(*in); in++; }
    return tm;
}
unsigned long int String::gethash(){
    unsigned long int ret = 1;
    for( unsigned int i = 0; i < size(); ++i ){
        ret *= operator[](i);
    }
    return ret;
}
int String::write( FILE* f ){
    if( f != 0 ){
        uint32_t len = size();
        fwrite( &len, 1, sizeof( uint32_t), f );
        fwrite( c_str(), 1, size(), f );
    }
    return sizeof( uint32_t) + size();
}
int String::read(FILE* f){
    uint32_t len;
    fread( &len, 1, sizeof( len ), f );
    this->resize( len );
    fread( (void*)c_str(), 1, len, f );
    //operator=(c_str());
    //this->resize(len+1);
    return sizeof( len ) + len;
}
String String::urldecode(){
    String ret = "";
    for( unsigned int i = 0; i < length(); ++i ){
        if( operator[](i) != '%' ){
            ret += operator[](i);
            continue;
        }
        if( i + 2u >= length() ) break;
        char c1 = ::toupper(operator[](++i));
        char c2 = ::toupper(operator[](++i));
        if( c1<='9' ) c1 -= '0'; if( c1>='A' ) c1 -= 'A' - 10;
        if( c2<='9' ) c2 -= '0'; if( c2>='A' ) c2 -= 'A' - 10;
        ret += (c1 << 4) + c2;
    }
    return ret;
}
