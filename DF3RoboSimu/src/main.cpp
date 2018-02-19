#include <iostream>


#include "logic/game.h"


using namespace std;


int main(int argc, char** argv) 
{
    PLAYER_ID tempid;
    char* recordFile = "record.data";
    char* mapFile = "test.map";
    Game game(recordFile, mapFile);

    while (true)
    {
        if (game.getTime() % 100 == 0)
            std::cout << "simulation time: " << game.getTime() << std::endl;
        tempid = game.frameRoutine();
        if (tempid == P0)
        {
            std::cout << "player 0 won!" << std::endl;
            break;
        }
        else if (tempid == P1)
        {
            std::cout << "player 1 won!" << std::endl;
            break;
        }
        else if (tempid == DRAW)
        {
            std::cout << "Game tied!" << std::endl;
            break;
        }
    }

}