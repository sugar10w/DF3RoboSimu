// 游戏主逻辑

#include "logic/game.h"

using namespace std;

Game::Game(char* recordFile, char* mapFile) {

    map = new Map(this, mapFile); 

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
    int32_t version = 322;
    record_file.write((char*)&version ,sizeof(int32_t));
    record_file.flush();

    srand((unsigned)time(NULL));
    randframe = rand() % 2;

    valid = true;
}


PLAYER_ID Game::frameRoutine()
{

    if (NULL == player_list || !player_list[0]->isValid() || !player_list[1]->isValid()) {
        std::cout << "[Warning] some players not valid" << std::endl;
    }

    PlayerInfo pi0, pi1;
    MapInfo mi;
    
    vector<car_info> cars;
    for (int id = 0; id < 2; ++id) {
        cars.push_back(car[id]->getCarInfo());
    }
    
    // 0.调用用户函数并解释
    for (int id = 0; id < 2; ++id) {

        PlayerControl pc;
        Info info;

        info.coor = car[id]->getCoor();
        info.car_angle = car[id]->getCarAngle();
        info.attack_angle = car[id]->getAttackAngle();
        info.hp = car[id]->getHP();
        info.mp = car[id]->getMP();
        info.mag = car[id]->getMAG();
        info.spd_status = car[id]->getSpdStatus() == SPD_HIGH;
        info.frz_status = car[id]->getSpdStatus() == SPD_LOW;
        info.shd_status = car[id]->getDefStatus() == DEF_ARM;
        info.can_atk = car[id]->getAtkCdStatus() == BUFF_NORM;
        info.can_spd = car[id]->getSpeedUpCdStatus() == BUFF_NORM;
        info.can_frz = car[id]->getSlowDownCdStatus() == BUFF_NORM;
        info.can_shd = car[id]->getDefCdStatus() == BUFF_NORM;
        map->getView(
            car[id],
            cars,
            info.cars,
            info.props,
            info.obs);

        // TODO: getview获得视野并填充info结构体

        pc = player_list[id]->timedRun(info, 1000);
        //pc = player_list[id]->run(info);
        car[id]->setLeftSpeed(pc.left_speed);
        car[id]->setRightSpeed(pc.right_speed);
        car[id]->rotateAttack(pc.steer_angle); 

        switch (pc.action) {
        case NoAction: break;
        case Attack1: car[id]->attack(ATK_1MAG); break;
        case Attack2: car[id]->attack(ATK_2MAG); break;
        case Attack3: car[id]->attack(ATK_3MAG); break;
        case ChangeMag: car[id]->changeMag(); break;
        case FrozenRays: car[id]->emitSlowdown(); break;
        case SpeedUp: car[id]->speedUp(); break;
        case Shield: car[id]->protect(); break;
        default: break;
        }

    }


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

    //4. TODO 检查道具使用

    //5.写回放文件
    mi = map->getMapInfo();
    record_file.write((char*)&pi0, sizeof(pi0));
    record_file.write((char*)&pi1, sizeof(pi1));
    record_file.write((char*)&mi, sizeof(mi));
    record_file.flush();

    frame += 1;

    //5.判断是否结束
    if (car[0]->getHP() <= 0.0 && car[1]->getHP() <= 0.0 
        || (!player_list[0]->isValid() && !player_list[1]->isValid()))
    {
        record_file.close();
        return DRAW;
    }
    else if (car[0]->getHP() <= 0.0 || !player_list[0]->isValid())
    {
        record_file.close();
        return P1;
    }
    else if (car[1]->getHP() <= 0.0 || !player_list[1]->isValid())
    {
        record_file.close();
        return P0;
    }
    else
        return UNKNOWN_PLAYER;
    
}

bool Game::attack(PLAYER_ID id, ATK_NUM_MAG mag_num) {

    PLAYER_ID emy_id = (PLAYER_ID)(1 - id);

    ATK_POS hit = map->aim_check(
        getCar(id)->getCoor(),
        getCar(id)->getCarAngle(),
        getCar(id)->getAttackAngle(),
        getCar(emy_id)->getCoor(),
        getCar(emy_id)->getCarAngle());
    
    switch (hit)
    {
    case ATK_FRONT: case ATK_SIDE: case ATK_BACK:
        car[emy_id]->getAttacked(mag_num, hit);
        break;
    case ATK_MISS: break;
    default: break;
    }

    return ATK_MISS != hit;
}

bool Game::slowdown(PLAYER_ID id) {
    PLAYER_ID emy_id = (PLAYER_ID)(1 - id);

    ATK_POS hit = map->aim_check(
        getCar(id)->getCoor(),
        getCar(id)->getCarAngle(),
        getCar(id)->getAttackAngle(),
        getCar(emy_id)->getCoor(),
        getCar(emy_id)->getCarAngle());

    switch (hit)
    {
    case ATK_FRONT: case ATK_SIDE: case ATK_BACK:
        car[emy_id]->slowedDown();
        break;
    case ATK_MISS: break;
    default: break;
    }

    return ATK_MISS != hit;
}
