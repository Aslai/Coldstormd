#include "coldstormd/cmdcallbacks.h"



namespace ColdstormD{
    namespace callbacks{
        /*int domode( connection& c, String room, String Target, String reason = "" ){
            if( (reason != "" && args.size() < 4)||(reason=="" && args.size()<3) ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].ban(c.usr, us, args[3]) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }*/
        int ping( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int usr = c.usr;
            ColdstormD::users[usr].con->send("PONG :"+args[1]+"\r\n");
            return ERROR_NONE;
        }
        int pong( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            ColdstormD::users[c.usr].con->send("PONG :"+args[1]+"\r\n");
            return ERROR_NONE;
        }
        int privmsg( connection& c, vector<String> args ){

            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int usr = c.usr;
            String target = args[1];
            String message = args[2];

            printf("PM to %s\n", target.c_str() );
            int ret = ERROR_NONE;
            if( target[0] == '#' )
            {
                 printf("PM to room %s\n", target.c_str() );
                int t = getroombyname(target);
                if( t >= 0 ){
                    printf("sending\n");
                    ret = rooms[t].privmsg(usr, message);
                    if( ret == ERROR_NONE ){
                        users[c.usr].linestyped++;
                    }
                }
            }
            else{
                    printf("PM to user%s\n", target.c_str() );
                int t = getuserbynick(target);
                if( t >= 0 ){
                    printf("sending\n");
                    ret = users[t].privmsg(usr, message);
                    if( ret == ERROR_NONE ){
                        users[c.usr].linestyped++;
                    }
                }
            }
            if( ret != ERROR_NONE ){
                if( ret == ERROR_PERMISSION ){
                    c.send( ":"+alertname+" NOTICE "+users[c.usr].nick+" :"+target+" had you blocked, and will not receive your message\r\n");
                }
            }
            //::users[usr].con->send("PONG :"+message+"\r\n");
            return ret;
        }

        int whois( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            //::users[usr].con->send("PONG :"+message+"\r\n");
            int usr = c.usr;
            String message = args[1];
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

            return ERROR_NONE;
        }
        int join( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return users[c.usr].joinroom(args[1]);
        }
        int nick( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            if( getuserbynick( args[1] ) >= 0 ){
                c.notice("Nickname already in use");
                return ERROR_PARAM;
            }
            if( !validateusername(args[1]) ){
                c.notice("Erroneous nickname");
                return ERROR_PARAM;
            }
            c.send( ":"+users[c.usr].getmask()+" NICK "+args[1]+"\r\n" );
            users[c.usr].broadcast("NICK "+args[1] );
            users[c.usr].nick = args[1];
            return ERROR_NONE;
        }
        int part( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return users[c.usr].partroom(args[1], args.size() > 2 ? args[2] : "" );
        }
        int motd( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].setmotd(c.usr, args[2]) == ERROR_PERMISSION ){
                c.notice("Insufficient permissions");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int invite( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }

            if( rooms[rm].invite(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int ban( connection& c, vector<String> args ){
            if( args.size() < 4 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].ban(c.usr, us, args[3]) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int unban( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].unban(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int kick( connection& c, vector<String> args ){
            if( args.size() < 4 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].kick(c.usr, us, args[3]) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int mute( connection& c, vector<String> args ){
            if( args.size() < 4 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].mute(c.usr, us, args[3]) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int unmute( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].unmute(c.usr, us ) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int voice( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].voice(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int mod( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].mod(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int star( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].star(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int op( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].op(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }
        int sop( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].sop(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }



        int strip( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = getuserbynick( args[2] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].strip(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
        }

        int echo( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            if( args[1].tolower() == "true" ){
                users[c.usr].echo = true;
            }
            else if( args[1].tolower() == "false" ){
                users[c.usr].echo = false;
            }
            else return ERROR_PARAM;
            return ERROR_NONE;
        }

        int mode( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            int us = -5;
            if( args.size() >= 4 ){
                us = getuserbynick( args[3] );
                if( us < 0 ){
                    c.notice("Unknown user");
                    return ERROR_NOTFOUND;
                }
            }
            int typ = 1, po = 0;
            if( args[2][0] == '-' ){
                typ = -1;
            }
            if( args[2][0] == '-' || args[2][0] == '+' ){
                po = 1;
            }
            if( typ == 1 )
                switch( args[2][po] ){
                    case 'b':
                        if( us == -5 ){c.notice("Insufficient parameters"); return ERROR_PARAM;}
                        rooms[rm].ban( c.usr, us, "Bye bye!" );
                        break;
                    case 'v':
                        if( us == -5 ){c.notice("Insufficient parameters"); return ERROR_PARAM;}
                        rooms[rm].voice( c.usr, us );
                        break;
                    case 'h':
                        if( us == -5 ){c.notice("Insufficient parameters"); return ERROR_PARAM;}
                        rooms[rm].mod( c.usr, us );
                        break;
                    case 'o':
                        if( us == -5 ){c.notice("Insufficient parameters"); return ERROR_PARAM;}
                        rooms[rm].op( c.usr, us );
                        break;
                    case 'q':
                        if( us == -5 ){c.notice("Insufficient parameters"); return ERROR_PARAM;}
                        rooms[rm].sop( c.usr, us );
                        break;
                    case 'i':
                        rooms[rm].setoptions( c.usr, OPTION_INVITEONLY, true );
                        break;
                    case 'm':
                        if( us == -5 ){c.notice("Insufficient parameters"); return ERROR_PARAM;}
                        rooms[rm].mute( c.usr, us, "Be quiet" );
                        break;
                }
            if( typ == -1 )
                switch( args[2][po] ){
                    case 'b':
                        if( us == -5 ){c.notice("Insufficient parameters"); return ERROR_PARAM;}
                        rooms[rm].ban( c.usr, us, "Bye bye!" );
                        break;
                    case 'v': case 'h': case 'o': case 'q':
                        if( us == -5 ){c.notice("Insufficient parameters"); return ERROR_PARAM;}
                        rooms[rm].strip( c.usr, us );
                        break;
                    case 'i':
                        rooms[rm].setoptions( c.usr, OPTION_INVITEONLY, false );
                        break;
                    case 'm':
                        if( us == -5 ){c.notice("Insufficient parameters"); return ERROR_PARAM;}
                        rooms[rm].unmute( c.usr, us );
                        break;
                }
            return ERROR_NONE;
        }


        int ignore( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int us = getuserbynick( args[1] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }

            return users[c.usr].ignore(us);
        }
        int listen( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int us = getuserbynick( args[1] );
            if( us < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            return users[c.usr].listen(us);
        }

        int color( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return users[c.usr].setcolor(args[1]);
        }
        int away( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return users[c.usr].away(args[1]);
        }
        int ignorelist( connection& c, vector<String> args ){
            if( args.size() < 1 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return users[c.usr].ignorelist();
        }


        int who( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            return rooms[rm].sendinfo(c.usr);
        }


        int motdhistory( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            int rm = getroombyname( args[1] );
            if( rm < 0 ){
                c.notice("Invalid room");
                return ERROR_NOTFOUND;
            }
            return rooms[rm].motdhistory(c.usr);
        }

        int options( connection& c, vector<String> args ){
            if( args.size() < 4 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int servmute( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int guestpass( connection& c, vector<String> args ){
            if( args.size() < 1 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int servstrip( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int servoper( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int servmod( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int servban( connection& c, vector<String> args ){
            if( args.size() < 4 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int servunban( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int servbanlist( connection& c, vector<String> args ){
            if( args.size() < 1 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int banlist( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int help( connection& c, vector<String> args ){
            if( args.size() < 1 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
    }


}
