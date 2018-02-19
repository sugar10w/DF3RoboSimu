#include "..\..\include\logic\map.h"

// 地图，包括障碍物判定等

bool Map::init(const Game* _game, const char* filename)
{
    game = _game;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "未能成功打开地图文件" << filename << std::endl;
        return false;
    }
    else {
        std::string temp;
        std::vector<std::string> paras;

        std::getline(file, temp);
        paras = split(temp, ',');
        props.push_back(Prop(HP_PAK, atof(paras[1].c_str()), atof(paras[2].c_str()), P0));

        std::getline(file, temp);
        paras = split(temp, ',');
        props.push_back(Prop(HP_PAK, atof(paras[1].c_str()), atof(paras[2].c_str()), P1));

        std::getline(file, temp);
        paras = split(temp, ',');
        props.push_back(Prop(MP_PAK, atof(paras[1].c_str()), atof(paras[2].c_str()), P0));

        std::getline(file, temp);
        paras = split(temp, ',');
        props.push_back(Prop(MP_PAK, atof(paras[1].c_str()), atof(paras[2].c_str()), P1));

        std::getline(file, temp);
        paras = split(temp, ',');
        props.push_back(Prop(SPD_BUFF, atof(paras[1].c_str()), atof(paras[2].c_str()), UNKNOWN_PLAYER));

        std::getline(file, temp);
        paras = split(temp, ',');
        props.push_back(Prop(DEF_BUFF, atof(paras[1].c_str()), atof(paras[2].c_str()), UNKNOWN_PLAYER));

        std::getline(file, temp);
        paras = split(temp, ',');
        //car1 = new Car(Point<TCoor>(atof(paras[1].c_str()), atof(paras[2].c_str())), atof(paras[3].c_str()), 1);
        //toDo: 修改
        std::getline(file, temp);
        paras = split(temp, ',');
        //car2 = new Car(Point<TCoor>(atof(paras[1].c_str()), atof(paras[2].c_str())), atof(paras[3].c_str()), 2);

        while (std::getline(file, temp))
        {
            paras = split(temp, ',');
            obs_info t = { Point<TCoor>(atof(paras[0].c_str()), atof(paras[1].c_str())),atof(paras[2].c_str()) };
            Obstacle.push_back(t);
        }
        return true;
        }
}

int Map::aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target, TAngle target_angle)
{
    //TODO: 
    // - 改成enum格式输出

    TAngle theta = car_angle + attack_angle;//与x正方向夹角
    //l: cos(theta)*(y-ya)+sin(theta)*（x-xa)=0
    TCoor distance = abs(cos(theta)*(P_target.y - P_attack.y) + sin(theta)*(P_target.x - P_attack.x));
    if (distance < RADIUS_CAR) {
        int len = Obstacle.size();
        bool is_obstacle = false;
        for (int i = 0; i < len; i++) {
            Point<TCoor> p = Obstacle[i].coor;
            if (abs(cos(theta)*(p.y - P_attack.y) + sin(theta)*(p.x - P_attack.x)) < Obstacle[i].radius) {
                is_obstacle = true;
                break;
            }
        }
        if (!is_obstacle) {
            TCoor l = P_attack.getDistance(P_target);//攻击者和目标圆心距离
            TCoor s = sqrt(l*l - distance*distance) - sqrt(RADIUS_CAR - distance);
            //命中点
            TCoor xx = P_attack.x + cos(theta)*s;
            TCoor yy = P_attack.y + sin(theta)*s;

            TAngle deta_phi = abs(atan2(yy, xx) - target_angle);
            if (deta_phi <= 45)
                return 1;//命中正面
            else if (deta_phi <= 135)
                return 2;//命中侧面
            else
                return 3;//命中背面
        }
    }
    return 0;//未命中  
}

void Map::getView(Car * car, std::vector<car_info>& cars) const {
    std::vector<car_info> cars_saw;
    std::vector<prop_info> props_saw;
    std::vector<obs_info>obstacles_saw;

    Point<TCoor> car_p = car->getCoor();
    //道具判断
    for (int i = 0; i < props.size(); i++) {
        Point<TCoor> prop_p = props[i].get_pos();
        bool be_cover = false;
        TAngle theta = atan2(car_p.y - prop_p.y, car_p.x - prop_p.x);//车与道具所在直线的角度
        TAngle deta_phi = abs(theta - car->getAttackAngle());//与视野中线的夹角

        if (deta_phi <= 22.5) {
            for (int j = 0; j < Obstacle.size(); j++) {
                TCoor distance = abs(cos(theta)*(Obstacle[i].coor.y - prop_p.y) + sin(theta)*(Obstacle[i].coor.x - prop_p.x));
                if (distance < Obstacle[i].radius) {
                    be_cover = true;
                    break;
                }
            }
            if (!be_cover) {
                prop_info prop_i = { props[i].get_type(), Point<TCoor>(TCoor(props[i].get_pos().x),TCoor(props[i].get_pos().y)) };
                props_saw.push_back(prop_i);
            }
        }
    }
    //障碍物判断
    for (int i = 0; i < Obstacle.size(); i++) {
        Point<TCoor> obs_p = Obstacle[i].coor;
        TAngle theta = atan2(-car_p.y + obs_p.y, -car_p.x + obs_p.x);
        TAngle deta_phi = abs(theta - car->getAttackAngle());
        if (deta_phi <= 22.5 + abs(asin(Obstacle[i].radius / obs_p.getDistance(car_p))))
            obstacles_saw.push_back(Obstacle[i]);
    }
    //敌方小车判断
    Point<TCoor> enemy_p = cars[0].coor;
    TAngle theta = atan2(enemy_p.y - car_p.y, enemy_p.x - car_p.x);
    TAngle deta_phi = abs(theta - car->getAttackAngle());
    bool is_visible = true;
    if (deta_phi <= 22.5 + abs(asin(RADIUS_CAR / enemy_p.getDistance(car_p)))) {
        for (int i = 0; i < obstacles_saw.size(); i++) {
            Point<TCoor> obs_p = obstacles_saw[i].coor;
            TCoor dis_c2t = car_p.getDistance(enemy_p);//观察者到目标
            TCoor dis_c2o = car_p.getDistance(obs_p);//观察者到障碍
            TCoor l = abs(cos(theta)*(obs_p.y - car_p.y) + sin(theta)*(obs_p.x - car_p.x));//障碍到观察者-目标连线的距离
            TCoor h = abs((enemy_p.x - car_p.x)*(obs_p.x - car_p.x) + (enemy_p.y - car_p.y)*(obs_p.y - car_p.y)) / dis_c2o;//沿连线距离
            if (h*RADIUS_CAR / dis_c2o + l < obstacles_saw[i].radius) {
                is_visible = false;
                break;
            }
        }
    }
    else {
        is_visible = false;
    }
    if (is_visible) {
        cars_saw.push_back(cars[0]);
    }
    car->getView(cars_saw, obstacles_saw, props_saw);
}

// 分解字符串，用于文件读取
static std::vector<std::string> split(std::string s, char c) {
    int len = s.length;
    int index_start = 0;
    int index_end = 0;
    std::vector<std::string> result;
    while (index_end < len) {
        if (s.c_str()[index_end] == c) {
            if (index_end != index_start)
                result.push_back(s.substr(index_start, index_end - 1));
            index_start = index_end + 1;
            index_end = index_start;
        }
        else {
            index_end++;
        }
    }
    result.push_back(s.substr(index_start, index_end - 1));
    return result;
}