#pragma once
#include<cstdio>
#include "mystring.h"
#include "state.h"

namespace ColdstormD{
    void writedb(String file);
    void readdb( String file );
    int writeint( FILE* f, uint32_t v );
    uint32_t readint( FILE* f );
}
