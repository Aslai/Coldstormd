#pragma once
namespace ColdstormD{ struct user; }
#include "coldstormd/constants.h"
#include "general/mystring.h"
#include<vector>
#include "coldstormd/connection.h"
#include "coldstormd/database.h"

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

        int inroom(unsigned int roomname);

        int joinroom(String room, bool overrideblocks=false);
        int privmsg( int usr, String msg );
        int partroom(String room, String message );

        int notice( int usr, String msg );
        String getmask();
        int write( FILE* f );
        int read( FILE* f );
        int quit( String message );
    };
}
