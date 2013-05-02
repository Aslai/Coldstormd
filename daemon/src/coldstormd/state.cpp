#include "state.h"
namespace ColdstormD{
    vector<room> rooms;
    vector<user> users;
    vector<String> guestpasses;
    String servername = "Coldstorm.TK";
    map<String, vector<int(*)(connection&,vector<String>)> > functions;
}
