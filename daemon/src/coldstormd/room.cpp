#include "room.h"

namespace ColdstormD{
    int room::write( FILE* f ){
        int len = name.write(0) + writeint(0,starowner) + writeint(0,options) + motd.write(0) + writeint(0, motdsetby) + writeint(0, motdseton) + writeint(0, motdseton) + writeint(0, motdseton) + writeint(0, motdseton) + writeint(0, motdseton);
        for( unsigned int i = 0; i < accesslist.size(); ++i ) len += writeint( 0, accesslist[i] );
        for( unsigned int i = 0; i < users.size(); ++i ) len += writeint( 0, users[i] );
        for( unsigned int i = 0; i < motdhist.size(); ++i ) len += motdhist[i].write(0);

        if( f != 0 ){
            fwrite(&len, 1, sizeof(uint32_t), f );
            writeint(f, id );
            writeint(f, accesslink );
            name.write(f);
            writeint(f,starowner);
            writeint(f,options);
            motd.write(f);
            writeint(f, motdsetby);
            writeint(f, motdseton);
            writeint( f, accesslist.size() );
            for( unsigned int i = 0; i < accesslist.size(); ++i ) len += writeint( f, accesslist[i] );
            writeint( f, users.size() );
            for( unsigned int i = 0; i < users.size(); ++i ) len += writeint( f, users[i] );
            writeint( f, motdhist.size() );
            for( unsigned int i = 0; i < motdhist.size(); ++i ) len += motdhist[i].write(f);


        }
        return len + sizeof(len);
    }
    int room::read( FILE* f ){
        DEBUG;
        uint32_t len;
        DEBUG;
        fread( &len, 1, 4, f );
        DEBUG;

        id = readint(f);
        accesslink = readint(f);

        name.read(f);
        DEBUG;
        starowner = readint(f);
        options = readint(f);
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
        toread = readint( f );
        DEBUG;
        for( unsigned int i = 0; i < toread; ++i ){
            DEBUG;
            users.push_back( readint( f ) );
            DEBUG;

        }
        toread = readint( f );
        DEBUG;
        for( unsigned int i = 0; i < toread; ++i ){
            DEBUG;
            String bean;
            bean.read( f );
            motdhist.push_back( bean );
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
                        return false;
                    return true;
                }
            }
            return true;
        }

    int room::sendwho(int usr){
        for( unsigned int i = 0; i < usershave.size(); ++i ){
            String prep="";
            for( unsigned int j = 0; j < users.size(); ++j ){
                if( users[j] == usershave[i] ){
                    switch( accesslist[j] ){
                        case ACCESS_VOP: prep = "+"; break;
                        case ACCESS_HOP: prep = "%"; break;
                        case ACCESS_AOP: prep = "@"; break;
                        case ACCESS_SOP: prep = "~"; break;
                    }
                }
            }
            ColdstormD::users[usr].con->send(":"+servername+" 352 "+ColdstormD::users[usr].nick+" "+name
                                   +" "+ColdstormD::users[usr].color+" "+ColdstormD::users[usr].ip+" "+servername+" "
                                   +ColdstormD::users[usr].nick+" H"+prep+" "+ColdstormD::users[usr].name+"\r\n");
        }
        return ERROR_NONE;
    }

    int room::sendinfo(int usr){
        ColdstormD::users[usr].con->send(":"+servername+" 332 "+ColdstormD::users[usr].nick+" "+name+" :"+motd+"\r\n");
        ColdstormD::users[usr].con->send(":"+servername+" 333 "+ColdstormD::users[usr].nick+" "+name+" "+ColdstormD::users[motdsetby].getmask()+" "+I(motdseton)+"\r\n");
        String s = ":"+servername+" 353 "+ColdstormD::users[usr].nick+" = "+name+" :";
        for( unsigned int i = 0; i < usershave.size(); ++i ){
            String prep="";
            for( unsigned int j = 0; j < users.size(); ++j ){
                if( users[j] == usershave[i] ){
                    int access = useraccess(users[j]);
                    if( (access & ACCESS_VOP) != 0 )  prep = "+";
                    if( (access & ACCESS_HOP) != 0 )  prep = "%";
                    if( (access & ACCESS_AOP) != 0 )  prep = "@";
                    if( (access & ACCESS_SOP) != 0 )  prep = "~";
                }
            }
            s += prep + ColdstormD::users[usershave[i]].nick+" ";
        }
        ColdstormD::users[usr].con->send(s+"\r\n");
        sendwho(usr);
        return ERROR_NONE;
    }

    int room::adduser( int usr, int access, bool forcejoin ){
        if( !canjoin(usr) ) return ERROR_PERMISSION;

        for( unsigned int i = 0; i < usershave.size(); ++i ){
                DEBUG;
            if( ColdstormD::users[usershave[i]].id == usr ){
                return ERROR_ALREADYDONE;
            }
        }
        for( unsigned int i = 0; i < users.size(); ++i ){
            DEBUG;
            if( ColdstormD::users[users[i]].id == usr ){
                if( forcejoin ){
                    usershave.push_back(usr);
                    DEBUG;
                    broadcast(usr, ":"+ColdstormD::users[usr].getmask()+" JOIN "+name+"\r\n", false);
                    sendinfo(usr);
                }
                return ERROR_NONE;
            }
        }

        users.push_back(usr);
        accesslist.push_back(access);
        if( forcejoin ){
            usershave.push_back(usr);
            DEBUG;
            broadcast(usr, ":"+ColdstormD::users[usr].getmask()+" JOIN "+name+"\r\n", false);
            sendinfo(usr);
        }
        //::users[usr].con->send( ":" + ::users[usr].nick + "!user@user JOIN "+name+"\r\n");
        return ERROR_NONE;
    }
    int room::partuser(int usr,bool supressmsg,bool extract){
        for( unsigned int i = 0; i < usershave.size(); ++i ){
                DEBUG;
            if( usershave[i] == usr ){
                if( !supressmsg )
                    broadcast(usr, ":" + ColdstormD::users[usr].getmask() + " PART "+name+" :Leaving\r\n", false);
                usershave.erase( usershave.begin() + i );
                if( extract )
                    ColdstormD::users[usr].extractroom(name);
                return ERROR_NONE;
            }
        }
        return ERROR_NOTFOUND;
    }
    int room::removeuser(int usr){
        partuser(usr);
        for( int i = 0; i < users[i]; ++i ){
                DEBUG;
            if( users[i] == usr ){
                users.erase( users.begin() + i );
                return ERROR_NONE;
            }
        }
        return ERROR_NOTFOUND;
    }
    int room::haveuser( int usr ){
        if( ( useraccess(usr) & (ACCESS_SOP | ACCESS_AOP ) ) != 0 )
            return true;
        for( unsigned int i = 0; i < usershave.size(); ++i ){
            if( usershave[i] == usr )
                return true;
        }
        return false;
    }
    int room::privmsg( int usr, String msg ){
        if( (useraccess(usr) & ACCESS_MUTED ) != 0 )
            return ERROR_PERMISSION;
        if( !haveuser(usr) )
            return ERROR_PERMISSION;
        broadcast(usr, ":"+ColdstormD::users[usr].getmask()+" PRIVMSG "+name+" :" + msg+"\r\n" );
        return ERROR_NONE;
    }
    int room::notice( int usr, String msg ){
        if( (useraccess(usr) & ACCESS_MUTED ) != 0 )
            return ERROR_PERMISSION;
        if( !haveuser(usr) )
            return ERROR_PERMISSION;
        broadcast(usr, ":"+ColdstormD::users[usr].getmask()+" NOTICE "+name+" :" + msg+"\r\n" );
        return ERROR_NONE;
    }
    int room::useraccess(int id){
        room* r;
        if( accesslink == -1 )
            r = this;
        else if( accesslink == -2 ){

        }
        else
            r = &ColdstormD::rooms[accesslink];
        for( unsigned int i = 0; i < r->users.size(); ++i ){
            if( r->users[i] == id ){
                if( (r->accesslist[i]&ACCESS_BANNED) != 0 )
                    return ACCESS_BANNED;
                return r->accesslist[i];
            }
        }
        return ACCESS_NONE;
    }
    int room::setmotd(int usr, String mo){
        if( ( useraccess(usr) & (ACCESS_HOP|ACCESS_AOP|ACCESS_SOP) ) == 0 ){
            return ERROR_PERMISSION;
        }
        if( !haveuser(usr) )
            return ERROR_PERMISSION;
        motd = mo;
        motdhist.push_back(I(time(0))+": "+mo);
        broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" TOPIC "+name+" :"+mo+"\r\n", false);
        return ERROR_NONE;
    }
    int room::invite(int usr, int target){
        if( ( useraccess(usr) & (ACCESS_HOP|ACCESS_AOP|ACCESS_SOP) ) == 0 ){
            return ERROR_PERMISSION;
        }
        if( !haveuser(usr) )
            return ERROR_PERMISSION;
        String msg = ":"+ColdstormD::users[usr].getmask()+" INVITE "+ColdstormD::users[target].nick+" "+name+"\r\n";
        broadcast( USER_SERVER, ":"+servername+" NOTICE "+name+" :*** "+ColdstormD::users[usr].nick+" invited "+ColdstormD::users[target].nick+" to "+name+"\r\n" );
        ColdstormD::users[target].con->send(msg);
        adduser( target, ACCESS_NONE, false );
        return ERROR_NONE;
    }
    int room::kick(int usr, int target, String reason){
        if( ( useraccess(usr) & (ACCESS_AOP|ACCESS_SOP) ) == 0 ){
            return ERROR_PERMISSION;
        }
        if( !haveuser(usr) )
            return ERROR_PERMISSION;
        int found = -1;
        for( unsigned int i = 0; i < users.size(); ++i ){
            if( users[i] == target ){
                found = i;
                break;
            }
        }
        if( found==-1 ) return ERROR_NOTFOUND;
        for( unsigned int i = 0; i < usershave.size(); ++i ){
            if( usershave[i] == target ){
                broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" KICK "+name+" "+ColdstormD::users[target].nick+" :"+reason+"\r\n", false );
                usershave.erase( usershave.begin() + i );
                ColdstormD::users[target].extractroom(name);
                return ERROR_NONE;
            }
        }
        return ERROR_NOTFOUND;
    }
    int room::setaccess(int usr, int target, int access, int requiredaccess){
        if( ( useraccess(usr) & requiredaccess ) == 0 ){
            return ERROR_PERMISSION;
        }
        if( !haveuser(usr) )
            return ERROR_PERMISSION;
        for( unsigned int i = 0; i < users.size(); ++i ){
            if( users[i] == target ){
                accesslist[i] |= access;
                return ERROR_NONE;
            }
        }
        return ERROR_NOTFOUND;
    }
    int room::revokeaccess(int usr, int target, int access, int requiredaccess){
        if( ( useraccess(usr) & requiredaccess ) == 0 ){
            return ERROR_PERMISSION;
        }
        if( !haveuser(usr) )
            return ERROR_PERMISSION;
        for( unsigned int i = 0; i < users.size(); ++i ){
            if( users[i] == target ){
                accesslist[i] &= ~access;
                return ERROR_NONE;
            }
        }
        return ERROR_NOTFOUND;
    }

    int room::ban(int usr, int target, String reason ){
        if( useraccess(target) == ACCESS_SOP ){
            return ERROR_PERMISSION;
        }
        int derp = setaccess( usr, target, ACCESS_BANNED, (ACCESS_AOP|ACCESS_SOP) );
        if( derp != ERROR_NONE )
            return derp;
        kick( usr, target, "Banned by "+ColdstormD::users[usr].nick+" - "+reason );
        return ERROR_NONE;
    }
    int room::unban(int usr, int target ){
        int derp = revokeaccess( usr, target, ACCESS_BANNED, (ACCESS_AOP|ACCESS_SOP) );
        if( derp != ERROR_NONE )
            return derp;
        broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" -b "+ColdstormD::users[target].nick+"\r\n", false );
        return ERROR_NONE;
    }

    int room::mute(int usr, int target, String reason ){
        int derp = setaccess( usr, target, ACCESS_MUTED, (ACCESS_HOP|ACCESS_AOP|ACCESS_SOP) );
        if( derp != ERROR_NONE )
            return derp;
        broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" +m "+ColdstormD::users[target].nick+" :"+reason+"\r\n", false );
        return ERROR_NONE;
    }
    int room::unmute(int usr, int target ){
        int derp = revokeaccess( usr, target, ACCESS_MUTED, (ACCESS_HOP|ACCESS_AOP|ACCESS_SOP) );
        if( derp != ERROR_NONE )
            return derp;
        broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" -m "+ColdstormD::users[target].nick+"\r\n", false );
        return ERROR_NONE;
    }


    int room::voice(int usr, int target){
        int derp = setaccess( usr, target, ACCESS_VOP, (ACCESS_HOP|ACCESS_AOP|ACCESS_SOP) );
        if( derp != ERROR_NONE )
            return derp;
        broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" +v "+ColdstormD::users[target].nick+"\r\n", false );
        return ERROR_NONE;
    }
    int room::mod(int usr, int target){
        int derp = setaccess( usr, target, ACCESS_HOP|ACCESS_VOP, (ACCESS_AOP|ACCESS_SOP) );
        if( derp != ERROR_NONE )
            return derp;
        broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" +h "+ColdstormD::users[target].nick+"\r\n", false );
        return ERROR_NONE;
    }
    int room::op(int usr, int target){
        int derp = setaccess( usr, target, ACCESS_AOP|ACCESS_HOP|ACCESS_VOP, (ACCESS_SOP) );
        if( derp != ERROR_NONE )
            return derp;
        broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" +o "+ColdstormD::users[target].nick+"\r\n", false );
        return ERROR_NONE;
    }
    int room::sop(int usr, int target){
        int derp = setaccess( usr, target, ACCESS_SOP|ACCESS_AOP|ACCESS_HOP|ACCESS_VOP, (ACCESS_SOP) );
        if( derp != ERROR_NONE )
            return derp;
        broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" +q "+ColdstormD::users[target].nick+"\r\n", false );
        return ERROR_NONE;
    }
    int room::star(int usr, int target){
        if( ( useraccess(usr) & (ACCESS_AOP|ACCESS_SOP) ) == 0 ){
            return ERROR_PERMISSION;
        }
        if( !haveuser(usr) )
            return ERROR_PERMISSION;
        if( starowner > 0 )
            broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" -S "+ColdstormD::users[starowner].nick+"\r\n", false );
        starowner = target;
        broadcast( usr, ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" +S "+ColdstormD::users[target].nick+"\r\n", false );
        return ERROR_NONE;
    }
    int room::setoptions(int usr, int option, int value){
        if( ( useraccess(usr) & ACCESS_SOP ) == 0 ){
            return ERROR_PERMISSION;
        }
        if( value == false ){
            options &= ~option;
        }
        else {
            options |= option;
        }
        return ERROR_NONE;
    }
    int room::strip(int usr, int target){
        int go = 0;
        if( !haveuser(usr) )
            return ERROR_PERMISSION;
        do{
            if( (useraccess(usr) & ACCESS_SOP) != 0 )
                { go = 4; break;}
            if((useraccess(usr)&(ACCESS_SOP|ACCESS_AOP))>(useraccess(target)&(ACCESS_SOP|ACCESS_AOP)))
                { go = 2; break;}
            if((useraccess(usr)&(ACCESS_SOP|ACCESS_AOP|ACCESS_HOP))>(useraccess(target)&(ACCESS_SOP|ACCESS_AOP|ACCESS_HOP)))
                { go = 1; break;}

        } while(1==0);
        if( go == 0 ){
            return ERROR_PERMISSION;
        }
        String n = ColdstormD::users[target].nick+" ";
        String m =  ":"+ColdstormD::users[usr].getmask()+" MODE "+name+" -";
        String mend = " ";
        int accesses[4]={ACCESS_SOP,ACCESS_AOP,ACCESS_HOP,ACCESS_VOP};
        for( int i = 0; i < go; ++i ){
            if( (useraccess(target) & (accesses[i])) != 0 ){
                m+=("qohv")[i];
                mend += n;
            }
        }
        m+=mend+"\r\n";
        if( mend == " " )
            return ERROR_INVALID;
        broadcast( usr, m, false );
        revokeaccess( usr, target, ACCESS_SOP|ACCESS_AOP|ACCESS_HOP|ACCESS_VOP,ACCESS_ANY);
        return ERROR_NONE;
    }
    int room::banlist(int usr){
        for( unsigned int i = 0; i < accesslist.size(); ++i ){
            if( (accesslist[i] & ACCESS_BANNED) ){
                ColdstormD::users[usr].con->send("");
            }
        }
        return ERROR_NONE;
    }



}
