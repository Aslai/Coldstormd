#pragma once
#include "coldstormd/connection.h"
#include "general/mystring.h"
#include "coldstormd/state.h"
#include "coldstormd/constants.h"
#include "coldstormd/database.h"
#include "coldstormd/utility.h"

namespace ColdstormD{
    namespace callbacks{
        int ping( connection& c, vector<String> args);
        int pong( connection& c, vector<String> args);
        int privmsg( connection& c, vector<String> args);
        int whois( connection& c, vector<String> args);
        int join( connection& c, vector<String> args);
        int nick( connection& c, vector<String> args);
        int part( connection& c, vector<String> args);
        int motd( connection& c, vector<String> args);
        int invite( connection& c, vector<String> args);
        int ban( connection& c, vector<String> args);
        int unban( connection& c, vector<String> args);
        int kick( connection& c, vector<String> args);
        int mute( connection& c, vector<String> args);
        int voice( connection& c, vector<String> args);
        int mod( connection& c, vector<String> args);
        int star( connection& c, vector<String> args);
        int op( connection& c, vector<String> args);
        int sop( connection& c, vector<String> args);
        int options( connection& c, vector<String> args);
        int servmute( connection& c, vector<String> args);
        int guestpass( connection& c, vector<String> args);
        int servstrip( connection& c, vector<String> args);
        int strip( connection& c, vector<String> args);
        int servoper( connection& c, vector<String> args);
        int servmod( connection& c, vector<String> args);
        int servban( connection& c, vector<String> args);
        int servunban( connection& c, vector<String> args);
        int servbanlist( connection& c, vector<String> args);
        int banlist( connection& c, vector<String> args);
        int help( connection& c, vector<String> args);
        int echo( connection& c, vector<String> args);
        int names( connection& c, vector<String> args);
    }
}
