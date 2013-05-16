#include "coldstormd.h"
namespace ColdstormD{
    String guestpass(){
        srand(time(0));
        String ret = "GUEST-";
        char index[]="1234567890qwertyuiopASDFGHJKLZXCVBNMzxcvbnmQWERTYUIOPasdfghjkl";
        for( int i = ret.length(); i < 20; ++i ){
            ret += index[rand() % (sizeof(index)/sizeof(index[0]))];
        }
        return ret;
    }
    void flushthread(void*){
        while( true ){
            Sleep(1);
            queued::flush(callbacktcp);
        }
    }

    int run(){


        #ifdef _WIN32
        WSADATA globalWSAData;
        WSAStartup( MAKEWORD(2, 2), &globalWSAData );
        #endif

        functions["ping"].push_back(callbacks::ping);
        functions["pong"].push_back(callbacks::pong);
        functions["privmsg"].push_back(callbacks::privmsg);
        functions["whois"].push_back(callbacks::whois);
        functions["join"].push_back(callbacks::join);
        functions["nick"].push_back(callbacks::nick);
        functions["part"].push_back(callbacks::part);
        functions["motd"].push_back(callbacks::motd);
        functions["invite"].push_back(callbacks::invite);
        functions["ban"].push_back(callbacks::ban);
        functions["unban"].push_back(callbacks::unban);
        functions["kick"].push_back(callbacks::kick);
        functions["mute"].push_back(callbacks::mute);
        functions["unmute"].push_back(callbacks::unmute);
        functions["mod"].push_back(callbacks::mod);
        functions["voice"].push_back(callbacks::voice);
        functions["star"].push_back(callbacks::star);
        functions["op"].push_back(callbacks::op);
        functions["sop"].push_back(callbacks::sop);
        functions["options"].push_back(callbacks::options);
        functions["servmute"].push_back(callbacks::servmute);
        functions["guestpass"].push_back(callbacks::guestpass);
        functions["servstrip"].push_back(callbacks::servstrip);
        functions["strip"].push_back(callbacks::strip);
        functions["servoper"].push_back(callbacks::servoper);
        functions["servmod"].push_back(callbacks::servmod);
        functions["servban"].push_back(callbacks::servban);
        functions["servunban"].push_back(callbacks::servunban);
        functions["servbanlist"].push_back(callbacks::servbanlist);
        functions["banlist"].push_back(callbacks::banlist);
        functions["help"].push_back(callbacks::help);
        functions["echo"].push_back(callbacks::echo);
        functions["who"].push_back(callbacks::who);
        functions["mode"].push_back(callbacks::mode);
        functions["ignore"].push_back(callbacks::ignore);
        functions["listen"].push_back(callbacks::listen);
        functions["ignorelist"].push_back(callbacks::ignorelist);
        functions["color"].push_back(callbacks::color);
        functions["away"].push_back(callbacks::away);
        functions["motdhistory"].push_back(callbacks::motdhistory);
        functions["topic"].push_back(callbacks::motd);
        functions["timezone"].push_back(callbacks::timezone);
        //functions[""].push_back(callbacks::);
        //functions[""].push_back(callbacks::);
        //functions[""].push_back(callbacks::);
        //functions[""].push_back(callbacks::);




        DEBUG;

        user u;
        u.id = 0;
        u.access = 0;
        u.nick = u.name = u.ip = u.color = "";
        users.push_back( u );
        u.id = 1;
        u.access = ACCESS_SOP|ACCESS_AOP|ACCESS_HOP|ACCESS_VOP|ACCESS_NONE;
        u.nick = u.name = servername;
        u.ip = "Coldstorm";
        u.color = "FFFFFF";
        u.country = "CS";
        users.push_back( u );
        frist = users.size();


        DEBUG;

        readdb("database.kdb");

        DEBUG;
        //atexit( onquit );
        //atexit( socketcleanup );

        DEBUG;

        guestpasses.push_back("1");
        guestpasses.push_back("2");
        guestpasses.push_back("3");
        guestpasses.push_back("4");
        guestpasses.push_back("5");
        guestpasses.push_back("6");


        DEBUG;
        call_thread( flushthread, 0 );

        call_thread( listenfortcp, 0 );

        DEBUG;
        call_thread( listenforajax, 0 );

        DEBUG;
        char test[100];

        DEBUG;
        gets(test);

        DEBUG;
        socketcleanup();
        onquit();
        return ERROR_NONE;
    }
}
