// 游戏主逻辑

#include "logic/game.h"

using namespace std;

Game::Game(char* recordFile, char* mapFile) {

    srand(time(NULL));

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
    //randframe = rand() % 2;

    valid = true;
}


PLAYER_ID Game::frameRoutine()
{

    if (NULL == player_list || !player_list[0]->isValid() || !player_list[1]->isValid()) {
        std::cout << "[Warning] some players not valid" << std::endl;
    }

    // 玩家控制指令
    PlayerControl pc[2];
    // 玩家信息(log输出用)
    PlayerInfo pi[2];
    // 地图信息(log输出用)
    MapInfo mi;
    // 随机决定本回合判定顺序
    int rid[2]; rid[0] = rand() % 2; rid[1] = 1 - rid[0];

    // 为计算视野准备的信息
    vector<car_info> cars;
    for (int id = 0; id < 2; ++id) {
        cars.push_back(car[id]->getCarInfo());
    }

    // 0.调用用户函数并解释

    for (int i = 0; i < 2; ++i) {

        int id = rid[i];
        Info info;

        info.id = id;
        info.round = frame;
        info.coor = car[id]->getCoor();
        info.car_angle = car[id]->getCarAngle();
        info.attack_angle = car[id]->getAttackAngle();
        info.hp = car[id]->getHP();
        info.mp = car[id]->getMP();
        info.mag = car[id]->getMAG();
        info.spd_status = car[id]->getSpdStatus() == SPD_HIGH;
        info.frz_status = car[id]->getSpdStatus() == SPD_LOW;
        info.shd_status = car[id]->getDefStatus() == DEF_ARM;
        info.can_atk = car[id]->getAtkCdStatus() == BUFF_NORM
            && car[id]->getMAG() > 0;
        info.can_spd = !info.spd_status
            && car[id]->getSpeedUpCdStatus() == BUFF_NORM
            && car[id]->getMP() >= BUFF_MP[BUFF_SPEEDUP];
        info.can_shd = !info.shd_status
            && car[id]->getDefCdStatus() == BUFF_NORM
            && car[id]->getMP() >= BUFF_MP[BUFF_DEFEND];
        info.can_frz = car[id]->getSlowDownCdStatus() == BUFF_NORM
            && car[id]->getMP() >= BUFF_MP[BUFF_SLOWDOWN];
        map->getView(
            car[id],
            cars,
            info.cars,
            info.props,
            info.obs);

        //pc[id] = player_list[id]->timedRun(info, 1000);
        pc[id] = player_list[id]->run(info);
        car[id]->setLeftSpeed(pc[id].left_speed);
        car[id]->setRightSpeed(pc[id].right_speed);
        car[id]->rotateAttack(pc[id].steer_angle);

        // 小车立即开始行动！
        car[id]->frameRoutine();

        switch (pc[id].action) {
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

        //cout << "id=" << id << "\tl=" << pc[id].left_speed << "\tr=" << pc[id].right_speed << "\ta=" << pc[id].steer_angle << "\tA=" << pc[id].action << endl;
    }
    
    //1.地图伤害：超时减伤，界外减伤
    for (int i = sizeof(TIMEOUT_TIME) / sizeof(TFrame) - 1; i >= 0; --i)
    {
        if (frame >= TIMEOUT_TIME[i])
        {
            car[0]->setHP(car[0]->getHP() - TIMEOUT_HP[i]);
            car[1]->setHP(car[1]->getHP() - TIMEOUT_HP[i]);
            break;
        }
    }

    for (int id = 0; id < 2; ++id)
    {
        if (car[id]->isOOR())
            car[id]->setHP(car[id]->getHP() - ATTACK_OOR);
    }

    //2.调用Map刷新道具
    map->refreshProp(frame);

    //4. TODO 检查道具拾取
    for (int id = 0; id < 2; ++id)
    {
        switch (map->getProp(car[id]))
        {
        case SPD_BUFF:
            car[id]->propSpeedUP();
            break;
        case DEF_BUFF:
            car[id]->propProtect();
            break;
        case HP_PAK:
            car[id]->setHP(car[id]->getHP() + PROP_HP_PAK_POINT);
            break;
        case MP_PAK:
            car[id]->setMP(car[id]->getMP() + PROP_MP_PAK_POINT);
            break;
        default:
            // 未获得道具
            break;
        }
    }

    //5.写回放文件
    for (int id = 0; id < 2; ++id) {
        pi[id] = car[id]->getPlayerInfo();
        record_file.write((char*)&(pi[id]), sizeof(pi[id]));
    }
    mi = map->getMapInfo();
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
        car[emy_id]->setPos(map->getKnockedPos(car[id], car[emy_id], mag_num));
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
