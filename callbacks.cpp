#include "callbacks.h"

namespace ColdstormD{
    int intro(connection& c, String name ){
        if( c.introd == true ) return 0;
        c.introd = true;
        c.send( ":"+servername+" 005 "+name+" CHANMODES=b, CHANTYPES=# CHARSET=utf-8 AWAYLEN=10 KICKLEN=256 MAXBANS=200 MAXCHANNELS=60 MAXPARA=32 :are supported by this server\r\n" );
        c.send( ":"+servername+" 005 "+name+" NETWORK=Coldstorm NICKLEN=20 PREFIX=(sohv)~@#+ STATUSMSG=~@#+ TOPICLEN=200 :are supported by this server\r\n" );
        return 1;
    }

    int onmsgnewsession( connection& c, vector<String> args ){
        String chk = args[0].tolower();
        if( chk == "nick" ){
            c.ircname = args[1];
        }
        if( chk == "validate"){
            if( args.size() <= 2 ){
                c.notice( "Usage: /VALIDATE name password" );
                return 0;
            }
            if( validateusername(args[1]) == 0){
                c.notice( "Erroneous name" );
                return 0;
            }
            int us = getuserbyname( args[1] );

            user& ops = users[us<0?0:us];
            if( ops.id == 0 ){
                int v = consumeguestpass( args[2] );
                if( v == 0 ){
                    c.notice( "That is an invalid username/password combo" );
                    return 0;
                }
                c.state = LOGIN_GUEST;
                c.notice( "Welcome to Coldstorm, "+args[1]+". Please set your password with /SETPASS [password]" );
                c.name = args[1];
                return 0;
            }
            if( ops.password == args[2] ){
                c.state = LOGIN_FULL;
                c.notice( "Successfully logged in as "+args[1] );
                ops.country = getcountrycode( c.sock );
                ops.ip = getipstr( c.sock );
                ops.con = &c;
                c.usr = ops.id;
                ops.echo = 0;
                c.send( ":"+c.ircname+"!user@user.user NICK "+ops.nick+"\r\n" );
                vector<unsigned int> t = ops.rooms;
                ops.rooms.clear();
                for( unsigned int i = 0; i< t.size(); ++i ){
                        DEBUG;
                    if( t[i] < rooms.size() ){
                        users[ops.id].joinroom(rooms[t[i]].name,true);
                       /* switch( ::rooms[t[i]].canjoin(ops.id) > 0 ){
                            case 1: c.send( ":" + ops.nick + "!user@user JOIN "+rooms[t[i]].name+"\r\n"); break;
                            case 0: c.send( ":" + ops.nick + "!user@user ERROR BAN "+rooms[t[i]].name+" :Cannot join "+rooms[t[i]].name+" for you are banned\r\n"); break;
                        }*/
                    }
                }
                return 0;
            }
            else{
                c.notice( "That is an invalid username/password combo" );
                return 0;
            }
        }
        else{
            c.notice( "Please use /VALIDATE [name] [password]" );
        }

        return 0;
    }

    int onmsgguest( connection& c, vector<String> args ){
        String chk = args[0].tolower();
        if( chk == "setpass" ){
            if( args.size() <= 1 ){
                c.notice( "Usage: /SETPASS [password]" );
                return 0;
            }
            user u;
            u.access = ACCESS_NONE;
            u.color = "FFFFFF";
            u.country = getcountrycode( c.sock );
            u.id = users.size();
            u.ip = getipstr(c.sock);
            u.name = c.name;
            u.nick = c.name;
            u.online = true;
            u.password = args[1];
            u.linestyped = 0;
            u.registered = time(0);
            u.con = &c;
            u.echo = 0;

            c.usr = u.id;
            c.state = LOGIN_FULL;
            c.notice("You have set your password to " + args[1] +". Don't forget it!");
            DEBUG;
            c.send( ":"+c.ircname+"!user@user.user NICK "+u.nick+"\r\n" );
            users.push_back( u );

            DEBUG;
            users[u.id].joinroom("#Coldstorm");
            DEBUG;
            users[u.id].joinroom("#2");
            DEBUG;

            DEBUG;

        }
        else{
            c.notice("Please set your password before doing anything else with /SETPASS [password]");
        }
        return 0;
    }




    int ping( int usr, String message ){
        ColdstormD::users[usr].con->send("PONG :"+message+"\r\n");
        return 1;
    }
    int pong( int usr, String message ){
        //::users[usr].con->send("PONG :"+message+"\r\n");
        message[0]=message[0];
        usr*=2;
        return 1;
    }
    int privmsg( int usr, String target, String message ){
        printf("PM to %s\n", target.c_str() );
        if( target[0] == '#' )
        {
             printf("PM to room %s\n", target.c_str() );
            int t = getroombyname(target);
            if( t >= 0 ){
                printf("sending\n");
                return rooms[t].privmsg(usr, message);
            }
        }
        else{
                printf("PM to user%s\n", target.c_str() );
            int t = getuserbyname(target);
            if( t >= 0 ){
                    printf("sending\n");
                return users[t].privmsg(usr, message);
            }
        }
        //::users[usr].con->send("PONG :"+message+"\r\n");
        return 1;
    }

    int whois( int usr, String message ){
        //::users[usr].con->send("PONG :"+message+"\r\n");
        int found = 0;
        for( unsigned int i = 0; i < users.size(); ++i ){
            if( message.tolower() == users[i].nick.tolower() ){
                found = 1;
                users[usr].con->send(":"+servername+" 311 "+users[usr].nick+" "+message+" "+users[usr].color+" "+users[usr].ip+" * :"+users[usr].name+"\r\n");
            }
        }
        if( found == 0 ){
            users[usr].con->send(":"+servername+" 401 "+users[usr].nick+" "+message+" :No such nick/channel\r\n");
        }
        users[usr].con->send(":"+servername+" 318 "+users[usr].nick+" * :End of /WHOIS list.\r\n");

        return 1;
    }


    int onmsgfull( connection& c, vector<String> args ){
        String chk = args[0].tolower();
        if( chk == "join" && args.size() > 1 ){
            users[c.usr].joinroom(args[1]);
        } else if( chk == "ping" && args.size() > 1 ){
            ping(c.usr, args[1] );
        } else if( chk == "pong" && args.size() > 1 ){
            pong(c.usr, args[1] );
        } else if( chk == "privmsg" && args.size() > 2 ){
            privmsg(c.usr, args[1], args[2] );
        } else if( chk == "whois" && args.size() > 1 ){
            whois(c.usr, args[1] );
        } else if( chk == "part" && args.size() > 1 ){

        } else if( chk == "" && args.size() > 1 ){

        } else if( chk == "" && args.size() > 1 ){

        } else if( chk == "" && args.size() > 1 ){

        } else if( chk == "" && args.size() > 1 ){

        }


        return 0;
    }


    int callbacktcp( connection& c, String msg ){

        DEBUG;
        printf("poo %s\n", msg.c_str() );
        vector<String> args = msg.splitirc();
        String chk = args[0].tolower();

        DEBUG;
        printf("LOGIN STATE: %i\n", c.state );
        if( c.state == LOGIN_NONE ){
            return onmsgnewsession( c, args );
        }
        else if( c.state == LOGIN_GUEST ){
            return onmsgguest( c, args );
        }
        else if( c.state == LOGIN_FULL ){
            return onmsgfull( c, args );
        }

        return 0;
    }

    int onconnect(connection& c){
        printf("\n\n\nfag\n\n\n");
        return intro( c, "Auth" );
    }

    void listenfortcp(void*){

        DEBUG;
        connectiontcp con;
        con.listen( 6667, callbacktcp, onconnect );
    }
    void listenforajax(void*){

        DEBUG;
        connectionajax con;
        con.onconnect = onconnect;
        con.listen(6666, callbacktcp, onconnect);
    }

    void onquit(void){
        writedb( "database.kdb" );
    }
}
