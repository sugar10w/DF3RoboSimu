// ��Ϸ���߼�

#include "logic/game.h"


Game::Game() {

    map = new Map(); //TODO: �����ļ�����Ϣ
    
    Point<TCoor> birth_point(0, 0);

    car[0] = new Car(this, map, birth_point, 0, P0);

}
