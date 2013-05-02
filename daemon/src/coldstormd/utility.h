#pragma once
#include "general/mystring.h"
#include "coldstormd/constants.h"
#include "coldstormd/room.h"
#include "coldstormd/user.h"

namespace ColdstormD{
    String I(int in);
    int getroombyname(String name);
    int getuserbyname(String name);
    int consumeguestpass( String pass );
    int validatechannelname(String name);
    int validateusername(String name);
}