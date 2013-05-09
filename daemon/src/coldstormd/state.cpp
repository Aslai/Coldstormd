#include "state.h"
namespace ColdstormD{
    vector<room> rooms;
    vector<user> users;
    vector<String> guestpasses;
    String servername = "Coldstorm.TK";
    String alertname = "Info!Informaster@"+servername;
    unsigned int frist = 0;
    map<String, vector<int(*)(connection&,vector<String>)> > functions;
}
