
#include "controller/player.h"

#include <iostream>

using std::string;
using std::cout;
using std::endl;

Player::Player() {
    player_ai = NULL;
    hDLL = NULL;
    valid = false;
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
