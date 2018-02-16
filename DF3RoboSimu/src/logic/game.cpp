// 游戏主逻辑

#include "logic/game.h"


Game::Game() {

    map = new Map(); //TODO: 导入文件名信息
    
    Point<TCoor> birth_point(0, 0);

    car[0] = new Car(this, map, birth_point, 0, P0);

}
