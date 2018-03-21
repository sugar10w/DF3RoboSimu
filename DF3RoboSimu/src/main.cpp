#include <iostream>


#include "logic/game.h"


using namespace std;


int main(int argc, char** argv) 
{
    
    if (argc != 3) {
        cout << "[Usage] " << argv[0] << " filename1.dll filename2.dll " << endl;
        system("pause"); exit(1);
    }

    PLAYER_ID tempid;

    // load Game
    char* recordFile = "record.log";
    char* mapFile = "../data/map.txt";
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
        if (game.getTime() % 100 == 0) 
        {
            cout << "simulation time: " << game.getTime() << endl;
        }

        cout << "t=" << game.getTime() << endl;
        cout << "x=" << game.getCar(P0)->getCoor().x << "\t\ty=" << game.getCar(P0)->getCoor().y << endl;
        cout << "a=" << game.getCar(P0)->getCarAngle() << "\t\tatk_a=" << game.getCar(P0)->getAttackAngle() << endl;
        //system("pause"); 

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