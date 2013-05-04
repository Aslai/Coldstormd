#include "utility.h"

namespace ColdstormD{
    int validatechannelname(String name){
        if( name.length() > 20 ) return false;
        if( name[0] != '#' ) return false;
        if( (signed)(name.find_first_of(" \t\r\n")) >= 0 ) return false;
        return true;
    }

    int validateusername(String name){
        DEBUG;
        if( name.length() > 20 ) return false;
        DEBUG;
        if( (signed)(name.find_first_of(" \t\r\n")) >= 0 ) return false;
        DEBUG;
        String beans = "1234567890`-_=|}]{[qwertyuiopasdfghjklzxcvbnm<>,.?/!^*()";
        DEBUG;
        for( unsigned int i = 0; i < name.length(); ++i ){
            DEBUG;
            if( (signed)(beans.find_first_of(tolower(name[i]))) < (signed)0 ) return false;
            DEBUG;
        }
        DEBUG;
        return true;
    }





    String I(int in){
        int l = snprintf(0, 0, "%i", in);
        String ret;
        ret.reserve( l+1 );
        ret.resize( l );

        snprintf((char*)ret.c_str(), l+1, "%i", in);
        return ret;
    }






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
            if( users[i].name.tolower() == name.tolower() ){
                return i;
            }
        }
        return -1;
    }
    int getuserbynick(String name){
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
                return true;
            }
        }
        return false;
    }
}
