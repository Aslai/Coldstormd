#pragma once
#include "coldstormd/connection.h"
#include "general/mystring.h"
#include "coldstormd/state.h"
#include "coldstormd/constants.h"
#include "coldstormd/database.h"
#include "coldstormd/utility.h"

namespace ColdstormD{
    namespace callbacks{
        int ping( int usr, String message );
        int pong( int usr, String message );
        int privmsg( int usr, String target, String message );
        int whois( int usr, String message );
        int nick( int usr, String message );
        int part( int usr, String message );
        int motd( int usr, String channel, String message );
        int invite( int usr, String channel, String target );
        int ban( int usr, String channel, String target, int length, String message );
        int unban( int usr, String channel, String target, int length, String message );
        int kick( int usr, String channel, String target, String message );
        int mute( int usr, String channel, String target, int length );
        int mod( int usr, String target );
        int star( int usr, String target );
        int op( int usr, String target );
        int sop( int usr, String target );
        int options( int usr, String opttype, String optvalue );
        int servmute( int usr, String target, int duration );
        int guestpass( int usr );
        int servstrip( int usr, String target );
        int strip( int usr, String channel, String target );
        int servoper( int usr, String target );
        int servmod( int usr, String target );
        int servban( int usr, String target, int duration, String message );
        int servunban( int usr, String target );
        int servbanlist( int usr );
        int banlist( int usr, String channel );
        int help( int usr );
        int echo( int usr, String tf );
        int names( int usr, String channel );
    }
}
