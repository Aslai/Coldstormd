#include "coldstormd/cmdcallbacks.h"

namespace ColdstormD{
    namespace callbacks{
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

        int nick( int usr, String message ){
        }
        int part( int usr, String message ){
        }
        int motd( int usr, String channel, String message ){
        }
        int invite( int usr, String channel, String target ){
        }
        int ban( int usr, String channel, String target, int length, String message ){
        }
        int unban( int usr, String channel, String target, int length, String message ){
        }
        int kick( int usr, String channel, String target, String message ){
        }
        int mute( int usr, String channel, String target, int length ){
        }
        int mod( int usr, String target ){
        }
        int star( int usr, String target ){
        }
        int op( int usr, String target ){
        }
        int sop( int usr, String target ){
        }
        int options( int usr, String opttype, String optvalue ){
        }
        int servmute( int usr, String target, int duration ){
        }
        int guestpass( int usr ){
        }
        int servstrip( int usr, String target ){
        }
        int strip( int usr, String channel, String target ){
        }
        int servoper( int usr, String target ){
        }
        int servmod( int usr, String target ){
        }
        int servban( int usr, String target, int duration, String message ){
        }
        int servunban( int usr, String target ){
        }
        int servbanlist( int usr ){
        }
        int banlist( int usr, String channel ){
        }
        int help( int usr ){
        }
        int echo( int usr, String tf ){
        }
        int names( int usr, String channel ){
        }
    }
}
