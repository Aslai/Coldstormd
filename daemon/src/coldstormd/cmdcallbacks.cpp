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
            if( rm < 0 ){
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
                }
            }
            else{
                    printf("PM to user%s\n", target.c_str() );
                int t = getuserbynick(target);
                if( t >= 0 ){
                        printf("sending\n");
                    ret = users[t].privmsg(usr, message);
                }
            }
            if( ret != ERROR_NONE ){
                if( ret == ERROR_PERMISSION ){
                    c.send( ":"+servername+" NOTICE "+users[c.usr].nick+" :You are muted on channel "+target+"\r\n");
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
            if( rm < 0 ){
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
            if( rm < 0 ){
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
            if( rm < 0 ){
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
            if( rm < 0 ){
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
            if( rm < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].mute(c.usr, us, args[3]) == ERROR_PERMISSION ){
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
            if( rm < 0 ){
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
            if( rm < 0 ){
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
            if( rm < 0 ){
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
            if( rm < 0 ){
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
            if( rm < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].sop(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
            }
            return ERROR_NONE;
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
            if( rm < 0 ){
                c.notice("Unknown user");
                return ERROR_NOTFOUND;
            }
            if( rooms[rm].strip(c.usr, us) == ERROR_PERMISSION ){
                c.notice("Insufficient permission");
                return ERROR_PERMISSION;
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
        int echo( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
        int names( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return ERROR_PARAM;
            }
            return ERROR_NONE;
        }
    }


}
