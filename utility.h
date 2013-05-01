#pragma once
#include "mystring.h"
#include "constants.h"
#include "room.h"
#include "user.h"

namespace ColdstormD{
    String I(int in);
    int getroombyname(String name);
    int getuserbyname(String name);
    int consumeguestpass( String pass );
    int validatechannelname(String name);
    int validateusername(String name);
}
