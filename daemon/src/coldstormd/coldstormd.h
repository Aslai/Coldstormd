#include "coldstormd/connection.h"
#include "coldstormd/user.h"
#include "coldstormd/room.h"
#include "general/threading.h"
#include "coldstormd/callbacks.h"
#include "coldstormd/database.h"
#include "coldstormd/state.h"
#include "coldstormd/cmdcallbacks.h"

using namespace std;

namespace ColdstormD{
    String guestpass();
    int run();
}
