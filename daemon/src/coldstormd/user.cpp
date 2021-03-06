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
            password.write(0) + writeint(0,access) + writeint(0, online) + writeint(0, id) +
            writeint(0,linestyped) + writeint(0,registered) + writeint( 0, rooms.size() ) +
            writeint( 0, offlinemax ) + writeint( 0, offlinemsgs.size() ) + writeint( 0, timeoffset );

        for( unsigned int i = 0; i < rooms.size(); ++i ) len += writeint( 0, rooms[i] );
        for( unsigned int i = 0; i < offlinemsgs.size(); ++i ) len +=  offlinemsgs[i].write(0);


        if( f != 0 ){
            writeint( f, len );
            writeint( f, timeoffset );
            len = name.write(f) + nick.write(f) + color.write(f) + country.write(f) + ip.write(f) + password.write(f) +
                writeint(f,access) + writeint(f,online) + writeint(f,id) + writeint(f,linestyped) + writeint(f,registered) + writeint( f, rooms.size() );
            for( unsigned int i = 0; i < rooms.size(); ++i ) len += writeint( f, rooms[i] );
            writeint( f, offlinemax );
            writeint( f, offlinemsgs.size() );
            for( unsigned int i = 0; i < offlinemsgs.size(); ++i ) len +=  offlinemsgs[i].write(f);

        }
        return len + sizeof(len);
    }
    int user::read( FILE* f ){
        uint32_t len;
        fread( &len, 1, 4, f );
        timeoffset = readint(f);
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
        offlinemax = readint(f);
        toread = readint( f );
        for( unsigned int i = 0; i < toread; ++i ){
            DEBUG;
            String t;
            t.read(f);
            offlinemsgs.push_back( t );
        }
        return len + sizeof(len);
    }
    int user::isignoring(int usr){
        for( unsigned int i = 0; i < ignores.size(); ++i ){
            if( usr == ignores[i] ) return true;
        }
        return false;
    }
    int user::privmsg( int usr, String msg ){
        if( isignoring(usr) )
            return ERROR_PERMISSION;
        if( online == false ){
            if( offlinemsgs.size() < offlinemax ){
                ColdstormD::users[usr].con->send(":"+alertname+" NOTICE "+ColdstormD::users[usr].nick+" :That user is offline and the message has been queued. They will recieve it when they sign on next.\r\n");
                offlinemsgs.push_back( ":"+ColdstormD::users[usr].getmask()+" PRIVMSG "+nick+" :"+timestamp()+": "+msg+"\r\n" );
            }
            else {
                ColdstormD::users[usr].con->send(":"+alertname+" NOTICE "+ColdstormD::users[usr].nick+" :That user is offline and their message box is full. Try again later.\r\n");

            }
            return ERROR_NONE;
        }
        con->send(":"+ColdstormD::users[usr].getmask()+" PRIVMSG "+nick+" :" +msg+"\r\n" );
        //linestyped++;
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
        a.starowner = -1;
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
            ColdstormD::rooms[rooms[i]].partuser(id,true, false);
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
        for(unsigned int j = 0; j < rooms.size(); ++j ){
            if( ColdstormD::rooms[rooms[j]].name.tolower() == room.tolower() ){
                rooms.erase(rooms.begin()+j);
                return ERROR_NONE;
            }
        }
        return ERROR_NOTFOUND;
    }
    int user::flushmessage(){
        while( offlinemsgs.size() > 0 ){
            con->send(offlinemsgs.front());
            offlinemsgs.pop_front();
        }
        return ERROR_NONE;
    }
    int user::ignore( int target ){
        for( unsigned int i = 0; i < ignores.size(); ++i ){
            if( ignores[i] == target ) return ERROR_ALREADYDONE;
        }
        ignores.push_back( target );
        con->send( ":"+alertname+" NOTICE "+nick+" :You have ignored "+ColdstormD::users[target].nick+"\r\n");
        return ERROR_NONE;
    }
    int user::listen( int target ){
        for( unsigned int i = 0; i < ignores.size(); ++i ){
            if( ignores[i] == target ){
                con->send( ":"+alertname+" NOTICE "+nick+" :You have stopped ignoring "+ColdstormD::users[target].nick+"\r\n");
                ignores.erase(ignores.begin()+i);
                return ERROR_NONE;
            }
        }
        return ERROR_NOTFOUND;
    }
    int user::ignorelist(){
        con->send(":"+alertname+" NOTICE "+nick+" :You have ignored:\r\n");
        for( unsigned int i = 0; i < ignores.size(); ++i ){
            con->send(":"+alertname+" NOTICE "+nick+" :"+ColdstormD::users[ignores[i]].nick+"\r\n");
        }
        con->send(":"+alertname+" NOTICE "+nick+" :End of ignore list\r\n");
        return ERROR_NONE;
    }
    int user::setcolor(String c){
        if( c.length() != 6 ){
            return ERROR_PARAM;
        }
        c = c.toupper();
        for( int i = 0; i < 6; ++i ){
            if( c[i] < '0' ) return ERROR_INVALID;
            if( c[i] > 'F') return ERROR_INVALID;
            if( c[i] >'9' && c[i] < 'A' ) return ERROR_INVALID;
        }
        color = c;
        broadcast("COLOR "+c);
        return ERROR_NONE;
    }
    int user::away(String reason){
        if( reason.length() > 12 ){
            return ERROR_PARAM;
        }
        awayreason = reason;
        broadcast("AWAY"+(reason!=""?" :"+reason:""));
        return ERROR_NONE;
    }
    int user::settimeoffset(String off){
        int mode=1;
        if( off[0] == '+' ){
            off = off.substr(1);
        }
        else if( off[0] == '-' ){
            mode=-1;
            off = off.substr(1);
        }
        if( off.length() < 3 || off.length() > 4 ){
            return ERROR_PARAM;
        }
        for( unsigned int i = 0; i < off.length(); ++i){
            if( off[i] < '0' || off[i] > '9' ){
                return ERROR_PARAM;
            }
        }
        int hour, minute;
        if( off.length() == 4 ){
            if( (off[0] == '2' && off[1] > '3') || off[0] > '2' || off[2] > '5' ) {
                return ERROR_PARAM;
            }
            hour = 10*(off[0]-'0')+(off[1]-'0');
            minute = 10*(off[2]-'0')+(off[3]-'0');

        }
        else{
            if( off[1] > '5' ) {
                return ERROR_PARAM;
            }
            hour = (off[0]-'0');
            minute = 10*(off[1]-'0')+(off[2]-'0');
        }
        printf("Minute: %i\tHour: %i\n", minute, hour);
        timeoffset = (60*hour+minute) * mode;
        printf("timeoffset %i\n", timeoffset);
        con->notice("You have successfully set your timezone to "+I(mode*hour)+(minute<10?":0":":")+I(minute));
        return ERROR_NONE;
    }
}
