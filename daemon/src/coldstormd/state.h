#pragma once
#include<vector>
#include "general/mystring.h"
#include "coldstormd/room.h"
#include "coldstormd/user.h"
namespace ColdstormD{
    extern vector<room> rooms;
    extern vector<user> users;
    extern vector<String> guestpasses;
}
