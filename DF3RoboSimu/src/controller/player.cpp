
#include "controller/player.h"

#include <iostream>

using namespace std;

#ifndef WIN32
// Implement GetTickCount() for *nix systems

#include <sys/time.h>

static unsigned GetTickCount()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0)
        return 0;

    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

#endif

Player::Player(string _file_name)
    : file_name(_file_name)
{
    player_ai = NULL;
    hDLL = NULL;
    valid = load();
}

Player::~Player() {

}

PlayerControl Player::run(const Info info) {
    
    int time_a = GetTickCount();
    int time_b;
    PlayerControl pc;
    
    if (!isValid()) return pc;

#if (!defined _MSC_VER) || (defined _DEBUG)
    try {
        pc = player_ai(info);
    }
    catch (exception e) {
        cout << "[ERROR] Player " << file_name << " raised an exception in run()." << endl;
        cout << e.what() << endl;
        kill(); return pc;
    }
#else 
    __try {
        pc = player_ai(info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        cout << "[ERROR] Player " << file_name << " raised an exception in run()." << endl;
        kill(); return pc;
    }
#endif

    time_b = GetTickCount();
    if (time_b - time_a > 2000) kill();

    return pc;
}

bool Player::load() {
    
    // close first 
    if (NULL != hDLL) {
        __MY_CLOSEDLL(hDLL);
        player_ai = NULL;
    }

    hDLL = __MY_LOADDLL(file_name.c_str());
    if (NULL != hDLL) {
        player_ai = (TPlayerAi)__MY_GETFUNC(hDLL, "player_ai");
    }
    valid = (NULL != player_ai);


    if (NULL == hDLL) cout << "[Error] Failed to load \"" << file_name << "\"" << endl;
        else if (!isValid()) cout << "[Error] \"" << file_name << "\" loaded but player_ai not found" << endl;
        else cout << "[Info] Succeeded to load \"" << file_name << "\"" << endl;

    return valid;

}

void Player::kill() {
    valid = false;
}
