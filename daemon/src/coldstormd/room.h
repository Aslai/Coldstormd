#pragma once
namespace ColdstormD{ struct room; }
#include "coldstormd/constants.h"
#include "general/mystring.h"
#include<vector>
#include "coldstormd/database.h"
#include "coldstormd/utility.h"
using namespace std;
namespace ColdstormD{
    struct room{
        int id;
        int accesslink;
        String name;
        int starowner;
        vector<int> users;
        vector<int> accesslist;
        vector<int> usershave;
        String motd;
        vector<String> motdhist;
        int motdsetby;
        int motdseton;
        int options;
        int canjoin( int usr );
        int adduser( int usr, int access = ACCESS_NONE, bool forcejoin = true );
        int partuser( int usr, bool supress = false );
        int removeuser( int usr );
        int privmsg( int usr, String msg );
        int notice( int usr, String msg );
        int sendinfo(int usr);
        int sendwho(int usr);
        int useraccess(int usr);
        int setmotd(int usr, String mo);
        int invite(int usr, int target);
        int kick(int usr, int target, String reason);
        int ban(int usr, int target, String reason );
        int setaccess(int usr, int target, int access, int requiredaccess);
        int revokeaccess(int usr, int target, int access, int requiredaccess);

        int mute(int usr, int target, String reason );
        int voice(int usr, int target);
        int mod(int usr, int target);
        int op(int usr, int target);
        int sop(int usr, int target);
        int star(int usr, int target);
        int setoptions(int usr, String option, int value);
        int strip(int usr, int target);
        int banlist(int usr);
        int unban(int usr, int target );

        void broadcast(int usr, String message, bool supressecho = true);

        int write( FILE* f );
        int read( FILE* f );
    };
}
