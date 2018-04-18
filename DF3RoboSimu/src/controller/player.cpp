
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

void* runPlayerFunc(void* param)
{
    THREAD_PARAM *p = (THREAD_PARAM *)param;
    *(p->pc) = p->player->player_ai(*(p->info));
    *(p->flag) = 0;
    return NULL;
}

PlayerControl Player::timedRun(const Info info, int msecond)
{
    bool isend = false;
    int flag = 1;
    PlayerControl pc;
    THREAD_PARAM temp = {this, &flag, &pc, &info};
    pthread_t pid;

    if (!isValid()) return pc;

    pthread_create(&pid, NULL, runPlayerFunc, &temp);
    int time_a = GetTickCount();

    while (GetTickCount() - time_a <= msecond)
    {
        if (0 == flag)
        {
            isend = true;
            break;
        }
    }

    if (!isend) {
        pthread_kill(pid, SIGTERM);
        kill();
    }
    
    return pc;
}


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
    PlayerControl pc{0, 0, 0, NoAction};

    if (!isValid()) return pc;


    try {
        pc = player_ai(info);
    }
    catch (exception e) {
        cout << "[ERROR] Player " << file_name << " raised an exception in run()." << endl;
        cout << e.what() << endl;
        kill(); return pc;
    }


    time_b = GetTickCount();
    //if (time_b - time_a > 2000) kill();

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
