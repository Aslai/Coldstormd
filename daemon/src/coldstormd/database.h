#pragma once
#include<cstdio>
#include "general/mystring.h"
#include "coldstormd/state.h"

namespace ColdstormD{
    void writedb(String file);
    void readdb( String file );
    int writeint( FILE* f, uint32_t v );
    uint32_t readint( FILE* f );
}
