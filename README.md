Progress
========


###As of this commit, the following features have yet to be implemented on the server:

* General server things

  * More to be added
 * Implement IP discovery and country lookup
 * Logging
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
 * SERVUNMUTE [time]

* Implement channel level sop commands
 * OPTIONS [mode]
* Implement channel level operator commands

* Implement channel level moderator commands

* Implement Channel level user commands
 * BANLIST
 * HELP
* Implement global user commands
 * SERVBANLIST
 * WHO [channel]
 * IGNORE [user]
 * LISTEN [user]
 * IGNORELIST
 * AWAY [msg]

###The following features have been implemented on the server:

* General server things
 * VALIDATE / SETPASS
 * Bandwidth limits
 * Connection failure handling
 * Limit number of sessions per account to one
 * WHOIS
 * MOTD memory
 * Offline messages
 * Alias the IRC MODE command to related CSd commands:
* Oper global commands
* Moderator global commands
* Channel level sop commands
 * SOP [user]
 * OP [user]
* Channel level operator commands
 * KICK [user]
 * BAN [time]
 * STAR [user]
 * MOD [user]
 * UNBAN [user]
* Channel level moderator commands
 * MUTE [user]
 * UNMUTE [user]
 * INVITE [user]
 * MOTD [motd]
 * STRIP [user]
 * VOICE [user]
* Channel level user commands
 * JOIN
 * PART
 * NICK
 * ECHO [t/f]
