#include <iostream>


#include "logic/game.h"


using namespace std;


char* getTimeStamp(char* s) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf_s(s, 256,
        "%4d%02d%02d-%02d%02d%02d-%03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    return s;
}

char* getFileName(char* s, char* full_path) {
    _splitpath(full_path, NULL, NULL, s, NULL);
    return s;
}

char* getRecordFileName(char* s, char* p0, char* p1) {

    char s_time[256];
    char s_p0[256], s_p1[256];

    getTimeStamp(s_time);
    getFileName(s_p0, p0);
    getFileName(s_p1, p1);

    sprintf_s(s, 1024,
        "record.[%s].[%s].[%s].log",
        s_time, s_p0, s_p1);

    return s;

}


int main(int argc, char** argv) 
{
    
    if (argc != 3) {
        cout << "[Usage] " << argv[0] << " filename1.dll filename2.dll " << endl;
        system("pause"); exit(1);
    }

    PLAYER_ID tempid;

    // load Game
    char recordFile[1024];
    char* mapFile = "../data/map.txt";
    getRecordFileName(recordFile, argv[1], argv[2]);

    Game game(recordFile, mapFile);

    if (!game.isValid()) {
        cout << "[Error] Failed to init Game." << endl;
        system("pause"); exit(1);
    }

    // load Player
    Player* player_list[2];
    player_list[0] = new Player(argv[1]);
    player_list[1] = new Player(argv[2]);
    if (!player_list[0]->isValid() || !player_list[1]->isValid()) {
        cout << "[Error] Fialed to load player." << endl;
        cout << "[Info] player_0: " << argv[1] << "; valid: " << player_list[0]->isValid() << endl;
        cout << "[Info] player_1: " << argv[2] << "; valid: " << player_list[1]->isValid() << endl;
        system("pause"); exit(1);
    }
    game.setPlayerList(player_list);

    while (true)
    {
        if (game.getTime() % 600 == 0) 
        {
            cout << "simulation time: " << game.getTime() << endl;
            //system("pause");
        }

        /*cout << endl << "t=" << game.getTime() << endl;
        cout << "x=" << game.getCar(P0)->getCoor().x << "\t\ty=" << game.getCar(P0)->getCoor().y << "\t\ta=" << game.getCar(P0)->getCarAngle() << endl;
        cout << "x=" << game.getCar(P1)->getCoor().x << "\t\ty=" << game.getCar(P1)->getCoor().y << "\t\ta=" << game.getCar(P1)->getCarAngle() << endl;
        */

        tempid = game.frameRoutine();
        if (tempid == P0)
        {
            cout << "player 0 won!" << endl;
            break;
        }
        else if (tempid == P1)
        {
            cout << "player 1 won!" << endl;
            break;
        }
        else if (tempid == DRAW)
        {
            cout << "Game tied!" << endl;
            break;
        }

    }

    system("pause");
    return 0;
}