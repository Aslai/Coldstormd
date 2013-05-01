#pragma once
namespace ColdstormD{ struct user; }
#include "constants.h"
#include "mystring.h"
#include<vector>
#include "connection.h"
#include "database.h"

namespace ColdstormD{
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
        String getmask();
        int write( FILE* f );
        int read( FILE* f );
    };
}
