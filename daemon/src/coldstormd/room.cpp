#include "room.h"

namespace ColdstormD{
    int room::write( FILE* f ){
        int len = name.write(0) + starowner.write(0) + motd.write(0) + writeint(0, motdsetby) + writeint(0, motdseton) ;
        for( unsigned int i = 0; i < accesslist.size(); ++i ) len += writeint( 0, accesslist[i] );
        if( f != 0 ){
            fwrite(&len, 1, sizeof(uint32_t), f );
            name.write(f);
            starowner.write(f);
            motd.write(f);
            writeint(f, motdsetby);
            writeint(f, motdseton);
            writeint( f, accesslist.size() );
            for( unsigned int i = 0; i < accesslist.size(); ++i ) len += writeint( f, accesslist[i] );
        }
        return len + sizeof(len);
    }
    int room::read( FILE* f ){
        DEBUG;
        uint32_t len;
        DEBUG;
        fread( &len, 1, 4, f );
        DEBUG;

        name.read(f);
        DEBUG;
        starowner.read(f);
        DEBUG;
        motd.read(f);
        motdsetby = readint(f);
        motdseton = readint(f);

        DEBUG;
        uint32_t toread = readint( f );
        DEBUG;
        for( unsigned int i = 0; i < toread; ++i ){
            DEBUG;
            accesslist.push_back( readint( f ) );
            DEBUG;

        }
        DEBUG;
        return len + sizeof(len);
    }

    void room::broadcast(int usr, String message, bool supressecho){
        printf("ba\n");
        for( unsigned int i = 0; i < usershave.size(); ++i ){
            if( supressecho )
                if( usershave[i] == usr && ColdstormD::users[usershave[i]].echo == false ) continue;
            printf("Broadcast from %s to %s\n", name.c_str(), ColdstormD::users[usershave[i]].nick.c_str() );
            ColdstormD::users[usershave[i]].con->send(message);
        }
    }
    int room::canjoin( int usr ){
            for( unsigned int i = 0; i < users.size(); ++i ){
                DEBUG;
                if( ColdstormD::users[users[i]].id == usr ){
                    if( (accesslist[i] & ACCESS_BANNED) != 0 )
                        return 0;
                    return 1;
                }
            }
            return 1;
        }

    int room::sendwho(int usr){
        for( unsigned int i = 0; i < usershave.size(); ++i ){
            String prep="";
            for( unsigned int j = 0; j < users.size(); ++j ){
                if( users[j] == usershave[i] ){
                    switch( accesslist[j] ){
                        case ACCESS_VOP: prep = "+"; break;
                        case ACCESS_HOP: prep = "#"; break;
                        case ACCESS_AOP: prep = "@"; break;
                        case ACCESS_SOP: prep = "~"; break;
                    }
                }
            }
            ColdstormD::users[usr].con->send(":"+servername+" 352 "+ColdstormD::users[usr].nick+" "+name
                                   +" "+ColdstormD::users[usr].color+" "+ColdstormD::users[usr].ip+" "+servername+" "
                                   +ColdstormD::users[usr].nick+" H"+prep+" "+ColdstormD::users[usr].name+"\r\n");
        }
        return 1;
    }

    int room::sendinfo(int usr){
        ColdstormD::users[usr].con->send(":"+servername+" 332 "+ColdstormD::users[usr].nick+" "+name+" :"+motd+"\r\n");
        ColdstormD::users[usr].con->send(":"+servername+" 333 "+ColdstormD::users[usr].nick+" "+name+" "+ColdstormD::users[motdsetby].getmask()+" "+I(motdseton)+"\r\n");
        String s = ":"+servername+" 353 "+ColdstormD::users[usr].nick+" = "+name+" :";
        for( unsigned int i = 0; i < usershave.size(); ++i ){
            String prep="";
            for( unsigned int j = 0; j < users.size(); ++j ){
                if( users[j] == usershave[i] ){
                    switch( accesslist[j] ){
                        case ACCESS_VOP: prep = "+"; break;
                        case ACCESS_HOP: prep = "#"; break;
                        case ACCESS_AOP: prep = "@"; break;
                        case ACCESS_SOP: prep = "~"; break;
                    }
                }
            }
            s += prep + ColdstormD::users[usershave[i]].nick+" ";
        }
        ColdstormD::users[usr].con->send(s+"\r\n");
        sendwho(usr);
        return 1;
    }

    int room::adduser( int usr, int access ){
        if( !canjoin(usr) ) return 0;

        for( unsigned int i = 0; i < usershave.size(); ++i ){
                DEBUG;
            if( ColdstormD::users[usershave[i]].id == usr ){
                return 0;
            }
        }
        for( unsigned int i = 0; i < users.size(); ++i ){
            DEBUG;
            if( ColdstormD::users[users[i]].id == usr ){
                usershave.push_back(usr);
                DEBUG;


                broadcast(usr, ":"+ColdstormD::users[usr].getmask()+" JOIN "+name+"\r\n", false);
                sendinfo(usr);
                return 1;
            }
        }

        users.push_back(usr);
        accesslist.push_back(access);
        usershave.push_back(usr);
        DEBUG;
        broadcast(usr, ":"+ColdstormD::users[usr].getmask()+" JOIN "+name+"\r\n", false);
        sendinfo(usr);
        //::users[usr].con->send( ":" + ::users[usr].nick + "!user@user JOIN "+name+"\r\n");
        return 1;
    }
    int room::partuser(int usr,bool supressmsg){
        for( unsigned int i = 0; i < usershave.size(); ++i ){
                DEBUG;
            if( usershave[i] == usr ){
                if( !supressmsg )
                    broadcast(usr, ":" + ColdstormD::users[usr].getmask() + " PART "+name+" :Leaving\r\n", false);
                usershave.erase( usershave.begin() + i );
                return 1;
            }
        }
        return 0;
    }
    int room::removeuser(int usr){
        partuser(usr);
        for( int i = 0; i < users[i]; ++i ){
                DEBUG;
            if( users[i] == usr ){
                users.erase( users.begin() + i );
                return 1;
            }
        }
        return 0;
    }

    int room::privmsg( int usr, String msg ){
        broadcast(usr, ":"+ColdstormD::users[usr].getmask()+" PRIVMSG "+name+" :" + msg+"\r\n" );
        return 1;
    }
    int room::notice( int usr, String msg ){
        broadcast(usr, ":"+ColdstormD::users[usr].getmask()+" NOTICE "+name+" :" + msg+"\r\n" );
        return 1;
    }
}
