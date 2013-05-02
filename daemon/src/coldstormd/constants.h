#pragma once

#ifdef DEBUGSTATEMENTS
#define DEBUG printf("%s - %i\n", __FILE__, __LINE__)
#else
#define DEBUG ;
#endif

#include<cstdio>
#include "general/mystring.h"

namespace ColdstormD{
    enum {
        LOGIN_NONE=0,
        LOGIN_GUEST,
        LOGIN_FULL
    };

    enum {
        ACCESS_NONE=0,
        ACCESS_VOP=1,
        ACCESS_HOP=2,
        ACCESS_AOP=4,
        ACCESS_SOP=8,
        ACCESS_BANNED=16
    };
}
