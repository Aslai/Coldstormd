#pragma once
#include<vector>
#include "general/mystring.h"
#include "coldstormd/room.h"
#include "coldstormd/user.h"
#include<map>
namespace ColdstormD{
    extern vector<room> rooms;
    extern vector<user> users;
    extern vector<String> guestpasses;
    extern String servername;
    extern map<String, vector<int(*)(connection&,vector<String>)> > functions;
}
