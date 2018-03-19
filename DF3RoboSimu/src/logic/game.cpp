// 游戏主逻辑

#include "logic/game.h"


Game::Game(char* recordFile, char* mapFile) {

    map = new Map(this, mapFile); //TODO: 导入文件名信息

    valid = false;

    if (!map->isValid())
    {
        std::cerr << "failed to initialize map resources." << std::endl;
        system("pause");  exit(1);
    }
    
    Point<TCoor> birth_point0, birth_point1;
    double car_angle0, car_angle1;
    map->getInitPos(P0, birth_point0, car_angle0);
    map->getInitPos(P1, birth_point1, car_angle1);

    car[0] = new Car(this, map, birth_point0, car_angle0, P0);
    car[1] = new Car(this, map, birth_point1, car_angle1, P1);

    if (!car[0] || !car[1])
    {
        std::cerr << "failed to initialize resources." << std::endl;
        system("pause");  exit(1);
    }

    record_file.open(recordFile, std::ios_base::out | std::ios_base::binary);
    if (!record_file)
    {
        std::cerr << "failed to open file " << recordFile << std::endl;
        system("pause");  exit(1);
    }

    srand((unsigned)time(NULL));
    randframe = rand() % 2;

    valid = true;
}


PLAYER_ID Game::frameRoutine()
{
    if (NULL == player_list || !player_list[0]->isValid() || !player_list[1]->isValid()) {
        std::cout << "[Error] player_list not valid" << std::endl;
    }

    PlayerInfo pi0, pi1;
    PlayerControl pc0, pc1;
    Info info;
    MapInfo mi;

    // Generate info
    // info

    // 0.调用用户函数并登记
    pc0 = player_list[0]->run(info);
    pc1 = player_list[1]->run(info);
    car[0]->setLeftSpeed (pc0.left_speed);
    car[0]->setRightSpeed(pc0.right_speed);
    //car[0]->rotateAttack (pc0.steer_angle); //TODO ??
    car[1]->setLeftSpeed (pc1.left_speed);
    car[1]->setRightSpeed(pc1.right_speed);    
    //car[1]->rotateAttack (pc1.steer_angle); //TODO ??

    //1.地图伤害：超时减伤
    for (int i = sizeof(TIMEOUT_TIME) / sizeof(TFrame) - 1; i >= 0; --i)
    {
        if (frame >= TIMEOUT_TIME[i])
        {
            car[0]->setHP(car[0]->getHP() - TIMEOUT_HP[i]);
            car[1]->setHP(car[1]->getHP() - TIMEOUT_HP[i]);
            break;
        }
    }

    //2.调用Map刷新道具
    map->refreshProp(frame);

    //3.调用小车的car->frameRoutine()
    pi0 = car[0]->frameRoutine();
    pi1 = car[1]->frameRoutine();

    //4.写回放文件
    mi = map->getMapInfo();
    record_file.write((char*)&pi0, sizeof(pi0));
    record_file.write((char*)&pi1, sizeof(pi1));
    record_file.write((char*)&mi, sizeof(mi));
    record_file.flush();

    frame += 1;

    //5.判断是否结束
    if (car[0]->getHP() <= 0.0 && car[1]->getHP() <= 0.0)
    {
        record_file.close();
        return DRAW;
    }
    else if (car[0]->getHP() <= 0.0)
    {
        record_file.close();
        return P1;
    }
    else if (car[1]->getHP() <= 0.0)
    {
        record_file.close();
        return P0;
    }
    else
        return UNKNOWN_PLAYER;
    
}

bool Game::attack(PLAYER_ID id) {
    // TODO
    return false;
}

bool Game::slowdown(PLAYER_ID id) {
    // TODO
    return false;
}
