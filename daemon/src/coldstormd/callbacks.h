#pragma once
#include "coldstormd/connection.h"
#include "general/mystring.h"
#include "coldstormd/state.h"
#include "coldstormd/constants.h"
#include "coldstormd/database.h"
#include "coldstormd/utility.h"

namespace ColdstormD{
    int intro(connection& c, String name );
    int onmsgnewsession( connection& c, vector<String> args );
    int onmsgguest( connection& c, vector<String> args );
    int ping( int usr, String message );
    int pong( int usr, String message );
    int privmsg( int usr, String target, String message );
    int whois( int usr, String message );
    int onmsgfull( connection& c, vector<String> args );
    int callbacktcp( connection& c, String msg );
    int onconnect(connection& c);
    void listenfortcp(void*);
    void listenforajax(void*);
    void onquit(void);
    int onconclose(connection& c);
}
