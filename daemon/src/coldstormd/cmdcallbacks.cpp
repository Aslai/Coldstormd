#include "coldstormd/cmdcallbacks.h"

namespace ColdstormD{
    namespace callbacks{
        int ping( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            int usr = c.usr;
            ColdstormD::users[usr].con->send("PONG :"+args[1]+"\r\n");
            return 1;
        }
        int pong( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            ColdstormD::users[c.usr].con->send("PONG :"+args[1]+"\r\n");
            return 1;
        }
        int privmsg( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            int usr = c.usr;
            String target = args[1];
            String message = args[2];

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

        int whois( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
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

            return 1;
        }
        int join( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return users[c.usr].joinroom(args[1]);
        }
        int nick( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int part( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int motd( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int invite( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int ban( connection& c, vector<String> args ){
            if( args.size() < 4 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int unban( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int kick( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int mute( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int mod( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int star( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int op( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int sop( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int options( connection& c, vector<String> args ){
            if( args.size() < 4 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int servmute( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int guestpass( connection& c, vector<String> args ){
            if( args.size() < 1 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int servstrip( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int strip( connection& c, vector<String> args ){
            if( args.size() < 3 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int servoper( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int servmod( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int servban( connection& c, vector<String> args ){
            if( args.size() < 4 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int servunban( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int servbanlist( connection& c, vector<String> args ){
            if( args.size() < 1 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int banlist( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int help( connection& c, vector<String> args ){
            if( args.size() < 1 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int echo( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
        int names( connection& c, vector<String> args ){
            if( args.size() < 2 ){
                c.notice("Insufficient parameters");
                return 0;
            }
            return 1;
        }
    }
}
