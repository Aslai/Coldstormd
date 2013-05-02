Progress
========


###As of Wednesday, May 1, 2013, the following features have yet to be implemented on the server:

* General server things
 * Alias the IRC MODE command to related CSd commands:
  * MODE +b [user] -> BAN [user]
  * MODE +v [user] -> VOICE [user]
  * MODE +h [user] -> MOD [user]
  * MODE +o [user] -> OP [user]
  * MODE +a [user] -> SOP [user]
  * MODE +i        -> OPTIONS INVITEONLY TRUE
  * MODE +m [user] -> MUTE [user]
  * MODE -b [user] -> UNBAN [user]
  * MODE -v [user] -> STRIP [user]
  * MODE -h [user] -> STRIP [user]
  * MODE -o [user] -> STRIP [user]
  * MODE -a [user] -> STRIP [user]
  * MODE -i        -> OPTIONS INVITEONLY FALSE
  * MODE -m [user] -> UNMUTE [user]
  * More to be added
 * Implement IP discovery and country lookup
* Implement oper global commands 
 * SERVKICK [user]
 * SERVBAN [time]
 * SERVUNBAN
 * SERVMOD [user]
 * SERVOPER [user]
 * SERVSTRIP [user]
* Implement Moderator global commands 
 * GUESTPASS
 * SERVMUTE [time]
* Implement channel level sop commands
 * OPTIONS [mode]
 * SOP [user]
 * OP [user]
* Implement channel level operator commands 
 * STAR [user]
 * MOD [user]
* Implement channel level moderator commands 
 * MUTE [user]
 * KICK [user]
 * BAN [time]
 * UNBAN [user]
 * INVITE [user]
 * MOTD [motd]
 * STRIP [user]
 * VOICE [user]
* Implement Channel level user commands
 * PART
 * BANLIST
 * HELP
* Implement global user commands
 * NICK
 * SERVBANLIST
 * ECHO [t/f]
 * NAMES [channel]


###The following features have been implemented on the server:

* General server things
 * VALIDATE / SETPASS
 * Bandwidth limits
 * Connection failure handling
 * Limit number of sessions per account to one
 * WHOIS
* Oper global commands 
* Moderator global commands
* Channel level sop commands
* Channel level operator commands
* Channel level moderator commands
* Channel level user commands
 * JOIN
* Global user commands
