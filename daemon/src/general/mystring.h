#pragma once
class String;
#include<string>
#include<vector>
#include<stdio.h>
#include<ctype.h>
#include<stdint.h>
using namespace std;

class String : public string {
    public:
    String();
    String(string a);
    String(const char* a);
    String& operator=(string a);
    String& operator=(const char* a);
    int indexof( String what, int pos = 0 );
    vector<String> split(String delimiter=" ");
    vector<String> splitirc();
    String tolower();
    String toupper();
    unsigned long int gethash();
    int write( FILE* f );
    int read(FILE* f);
    String urldecode();
};
