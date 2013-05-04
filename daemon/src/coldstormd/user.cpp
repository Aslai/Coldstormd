#include "user.h"
namespace ColdstormD{
    String user::getmask(){
        printf("%s\n", nick.c_str() );
        printf("%s\n", color.c_str() );
        printf("%s\n", country.c_str() );
        printf("%s\n", ip.c_str() );

        return nick+"!"+color+country+"@"+ip;
    }

    int user::write( FILE* f ){
        int len = name.write(0) + nick.write(0) + color.write(0) + country.write(0) + ip.write(0) +
            password.write(0) + writeint(0,access) + writeint(0, online) + writeint(0, id) + writeint(0,linestyped) + writeint(0,registered) + writeint( 0, rooms.size() );
        for( unsigned int i = 0; i < rooms.size(); ++i ) len += writeint( 0, rooms[i] );

        if( f != 0 ){
            writeint( f, len );
            len = name.write(f) + nick.write(f) + color.write(f) + country.write(f) + ip.write(f) + password.write(f) +
                writeint(f,access) + writeint(f,online) + writeint(f,id) + writeint(f,linestyped) + writeint(f,registered) + writeint( f, rooms.size() );
            for( unsigned int i = 0; i < rooms.size(); ++i ) len += writeint( f, rooms[i] );
        }
        return len + sizeof(len);
    }
    int user::read( FILE* f ){
        uint32_t len;
        fread( &len, 1, 4, f );
        name.read(f);
        nick.read(f);
        color.read(f);
        country.read(f);
        ip.read(f);
        password.read(f);
        access = readint(f);
        online = readint(f);
        online = false;

        id = readint(f);
        linestyped = readint(f);
        registered = readint(f);

        uint32_t toread = readint( f );
        for( unsigned int i = 0; i < toread; ++i ){
            DEBUG;
            rooms.push_back( readint( f ) );
        }
        return len + sizeof(len);
    }

    int user::privmsg( int usr, String msg ){
        con->send(":"+ColdstormD::users[usr].getmask()+" PRIVMSG "+nick+" :" + msg+"\r\n" );
        linestyped++;
        return ERROR_NONE;
    }
    int user::notice( int usr, String msg ){
        con->send(":"+ColdstormD::users[usr].getmask()+" NOTICE "+nick+" :" + msg+"\r\n" );
        linestyped++;
        return ERROR_NONE;
    }



    int user::joinroom(String room, bool overrideblocks){
        if( !overrideblocks ){
            if( validatechannelname(room) == 0 ){
                con->send( ":" + nick + "!user@user ERROR CHANNAME "+room+
                    " :Erroneous Channel Name :"+room+"\r\n");
                return ERROR_INVALID;
            }
            for( unsigned int i = 0; i < rooms.size();++i){
                    DEBUG;
                if( rooms[i] < ColdstormD::rooms.size() ){
                    if( ColdstormD::rooms[rooms[i]].name.tolower() == room.tolower() ){

                        con->send( ":" + nick + "!user@user ERROR PRESENT "+ColdstormD::rooms[rooms[i]].name+
                                 " :Cannot join "+ColdstormD::rooms[rooms[i]].name+" for you are already there\r\n");
                        return ERROR_ALREADYDONE;
                    }
                }
                else break;
            }
        }
        for( unsigned int i = 0; i < rooms.size();++i){
                DEBUG;
            if( rooms[i] < ColdstormD::rooms.size() ){
                if( ColdstormD::rooms[rooms[i]].name.tolower() == room.tolower() ){
                    struct room &r = ColdstormD::rooms[rooms[i]];
                    for( unsigned int j = 0; j <r.usershave.size(); ++j ){
                        if( r.usershave[j] == id ){
                            printf("USER: %i\n", r.usershave[j]);
                            con->send( ":" + nick + "!user@user ERROR PRESENT "+ColdstormD::rooms[rooms[i]].name+
                                    " :Cannot join "+ColdstormD::rooms[rooms[i]].name+" for you are already there\r\n");
                            return ERROR_ALREADYDONE;
                        }
                    }
                    break;

                }
            }
            else break;
        }

        for( unsigned int i = 0; i < ColdstormD::rooms.size();++i){
                DEBUG;
            if( ColdstormD::rooms[i].name.tolower() == room.tolower() ){
                if( ColdstormD::rooms[i].canjoin(id) ){
                    rooms.push_back( i );
                    //con->send( ":" + nick + "!user@user JOIN "+::rooms[i].name+"\r\n");
                    return ColdstormD::rooms[i].adduser(id);
                }
                else {
                    con->send( ":" + nick + "!user@user ERROR BAN "+ColdstormD::rooms[i].name+
                    " :Cannot join "+ColdstormD::rooms[i].name+" for you are banned\r\n");
                    return ERROR_PERMISSION;
                }
            }
        }
        struct room a;
        a.motd = "";
        //a.users.push_back( id );
        //a.accesslist.push_back( ACCESS_SOP );
        a.name = room;
        a.starowner = "";
        a.motdsetby = 0;
        a.motdseton = 0;
        a.id = ColdstormD::rooms.size();
        a.accesslink = -1;
        ColdstormD::rooms.push_back( a );
        rooms.push_back( ColdstormD::rooms.size()-1 );
        return ColdstormD::rooms[ColdstormD::rooms.size()-1].adduser(id,ACCESS_SOP);
    }


    int user::inroom( unsigned int roomname ){
        for( unsigned int i = 0; i < rooms.size(); ++i ){
            if( rooms[i] == roomname ){
                return true;
            }
        }
        return false;
    }

    int user::partroom(String room, String message){
        int rm = getroombyname( room );
        if( rm < 0 ) return ERROR_NOTFOUND;
        if( !inroom( rm ) ) return ERROR_INVALID;
        for( unsigned int i = 0; i < rooms.size(); ++i ){
            if( rooms[i] == (unsigned)rm ){
                rooms.erase(rooms.begin() + i );
                break;
            }
        }
        message[0] = 'a';
        //ColdstormD::rooms[rm].broadcast(id, ":"+getmask()+" PART "+room+" :"+message+"\r\n");
        ColdstormD::rooms[rm].partuser(id);

        return ERROR_NONE;
    }

    int user::quit(String message){
        DEBUG;
        for( unsigned int i = 0; i < rooms.size(); ++i ){
            ColdstormD::rooms[rooms[i]].broadcast(id, ":"+getmask()+" QUIT :"+message+"\r\n");
            ColdstormD::rooms[rooms[i]].partuser(id,true);
        }
        online = false;
        return ERROR_NONE;
    }
    int user::broadcast(String message){
        DEBUG;
        for( unsigned int i = 0; i < rooms.size(); ++i ){
            ColdstormD::rooms[rooms[i]].broadcast(id, ":"+getmask()+" "+message+"\r\n");
        }
        return ERROR_NONE;
    }
    int user::extractroom(String room){
        for(int j = 0; j < rooms.size(); ++j ){
            if( ColdstormD::rooms[rooms[j]].name.tolower() == room.tolower() ){
                rooms.erase(rooms.begin()+j);
                return ERROR_NONE;
            }
        }
        return ERROR_NOTFOUND;
    }
}
