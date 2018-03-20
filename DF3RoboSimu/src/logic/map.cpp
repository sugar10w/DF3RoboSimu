
// 地图，包括障碍物判定等
#include "logic/map.h"

using namespace std;

static double cos_d(double theta) { return cos(theta / 180 * 3.14159); }
static double sin_d(double theta) { return sin(theta / 180 * 3.14159); }
static double tan_d(double theta) { return tan(theta / 180 * 3.14159); }
static double atan2_d(double y, double x) { return 180 / 3.14159*atan2(y, x); }
static double asin_d(double s) { return 180 / 3.14159*asin(s); }

// 获取道具状态
static TFrame get_prop_status(Prop p) {
    if (p.is_available()) {
        return p.get_ET();
    }
    else {
        return(p.getCD_count() - p.getCD());
    }
}
// 分解字符串，用于文件读取
static vector<string> split(string s, char c) {
    int len = s.length();
    int index_start = 0;
    int index_end = 0;
    vector<string> result;
    while (index_end < len) {
        if (s.c_str()[index_end] == c) {
            if (index_end != index_start)
                result.push_back(s.substr(index_start, index_end - index_start));
            index_start = index_end + 1;
            index_end = index_start;
        }
        else {
            index_end++;
        }
    }
    result.push_back(s.substr(index_start, index_end - index_start));
    return result;
}


//初始化
bool Map::init(const Game* _game, const char* filename)
{
    game = _game;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "未能成功打开地图文件" << filename << endl;
        return false;
    }
    else {
        string temp;
        vector<string> paras;

        getline(file, temp, '#');
        paras = split(temp, ' ');
        props.push_back(Prop(HP_PAK, atof(paras[0].c_str()), atof(paras[1].c_str()), P0));

        getline(file, temp); getline(file, temp, '#');
        paras = split(temp, ' ');
        props.push_back(Prop(HP_PAK, atof(paras[0].c_str()), atof(paras[1].c_str()), P1));

        getline(file, temp); getline(file, temp, '#');
        paras = split(temp, ' ');
        props.push_back(Prop(MP_PAK, atof(paras[0].c_str()), atof(paras[1].c_str()), P0));

        getline(file, temp); getline(file, temp, '#');
        paras = split(temp, ' ');
        props.push_back(Prop(MP_PAK, atof(paras[0].c_str()), atof(paras[1].c_str()), P1));

        getline(file, temp); getline(file, temp, '#');
        paras = split(temp, ' ');
        props.push_back(Prop(SPD_BUFF, atof(paras[0].c_str()), atof(paras[1].c_str()), UNKNOWN_PLAYER));

        getline(file, temp); getline(file, temp, '#');
        paras = split(temp, ' ');
        props.push_back(Prop(DEF_BUFF, atof(paras[0].c_str()), atof(paras[1].c_str()), UNKNOWN_PLAYER));

        getline(file, temp); getline(file, temp, '#');
        //paras = split(temp, ' ');
        //car1 = new Car(Point<TCoor>(atof(paras[0].c_str()), atof(paras[1].c_str())), atof(paras[3].c_str()), 1);
        //此处未初始化Car

        getline(file, temp); getline(file, temp);
        //paras = split(temp, ' ');
        //car2 = new Car(Point<TCoor>(atof(paras[0].c_str()), atof(paras[1].c_str())), atof(paras[3].c_str()), 2);
        //此处未初始化Car

        while (getline(file, temp))
        {
            paras = split(temp, ' ');
            obs_info t = { Point<TCoor>(atof(paras[0].c_str()), atof(paras[1].c_str())),atof(paras[2].c_str()) };
            Obstacle.push_back(t);
        }
        return true;
    }
}

//道具刷新
void Map::refreshProp(TFrame frame)
{
    //五秒后开始生成每种道具
    if (game->getTime() >= PROP_START_TIME) {
        for (int i = 0; i < props.size(); i++)
            props[i].round_operation();
    }
}

//返回初始化坐标
bool Map::getInitPos(PLAYER_ID id, Point<TCoor>& birth_point, TAngle & car_angle) const
{
    // TODO 不要重新读文件了，没有必要
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "未能成功打开地图文件" << filename << endl;
        return false;
    }
    else {
        string temp;
        vector<string> paras;
        for (int i = 0; i < 6; i++) {
            getline(file, temp, '#'); getline(file, temp); //跳过道具参数
        }
        getline(file, temp,'#');
        //car0
        if (id == P0) {
            paras = split(temp, ' ');
            birth_point = Point<TCoor>(atof(paras[0].c_str()), atof(paras[1].c_str()));
            car_angle = atof(paras[2].c_str());
            return true;
        }
        getline(file, temp); getline(file, temp,'#');
        //car1
        if (id == P1) {
            paras = split(temp, ' ');
            birth_point = Point<TCoor>(atof(paras[0].c_str()), atof(paras[1].c_str()));
            car_angle = atof(paras[2].c_str());
            return true;
        }
        return false;
    }
}

//命中判断
ATK_POS Map::aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target, TAngle target_angle)
{

    TAngle theta = car_angle + attack_angle;//与x正方向夹角
    //l: cos(theta)*(y-ya)+sin(theta)*（x-xa)=0
    TCoor distance = abs(cos_d(theta)*(P_target.y - P_attack.y) + sin_d(theta)*(P_target.x - P_attack.x));
    if (distance < RADIUS_CAR) {
        int len = Obstacle.size();
        bool is_obstacle = false;
        for (int i = 0; i < len; i++) {
            Point<TCoor> p = Obstacle[i].coor;
            if (abs(cos_d(theta)*(p.y - P_attack.y) + sin_d(theta)*(p.x - P_attack.x)) < Obstacle[i].radius) {
                is_obstacle = true;
                break;
            }
        }
        if (!is_obstacle) {
            TCoor l = P_attack.getDistance(P_target);//攻击者和目标圆心距离
            TCoor s = sqrt(l*l - distance*distance) - sqrt(RADIUS_CAR - distance);
            //命中点
            TCoor xx = P_attack.x + cos_d(theta)*s;
            TCoor yy = P_attack.y + sin_d(theta)*s;

            TAngle deta_phi = abs(atan2_d(yy, xx) - target_angle);
            if (deta_phi <= 45)
                return ATK_FRONT;//命中正面
            else if (deta_phi <= 135)
                return ATK_SIDE;//命中侧面
            else
                return ATK_BACK;//命中背面
        }
    }
    return ATK_MISS;//未命中  
}

//视野判断
void Map::getView(Car * car, vector<car_info>& cars,
        vector<car_info>& cars_saw,
        vector<prop_info>& props_saw,
        vector<obs_info>& obstacles_saw) const 
{    
    cars_saw.clear();
    props_saw.clear();
    obstacles_saw.clear();

    Point<TCoor> car_p = car->getCoor();
    //道具判断
    for (int i = 0; i < props.size(); i++) {
        Point<TCoor> prop_p = props[i].get_pos();
        bool be_cover = false;
        TAngle theta = atan2_d(car_p.y - prop_p.y, car_p.x - prop_p.x);//车与道具所在直线的角度
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
        TAngle theta = atan2_d(-car_p.y + obs_p.y, -car_p.x + obs_p.x);
        TAngle deta_phi = abs(theta - car->getAttackAngle());
        if (deta_phi <= 22.5 + abs(asin(Obstacle[i].radius / obs_p.getDistance(car_p))))
            obstacles_saw.push_back(Obstacle[i]);
    }
    //敌方小车判断
    Point<TCoor> enemy_p = cars[0].coor;
    TAngle theta = atan2_d(enemy_p.y - car_p.y, enemy_p.x - car_p.x);
    TAngle deta_phi = abs(theta - car->getAttackAngle());
    bool is_visible = true;
    if (deta_phi <= 22.5 + abs(asin(RADIUS_CAR / enemy_p.getDistance(car_p)))) {
        for (int i = 0; i < obstacles_saw.size(); i++) {
            Point<TCoor> obs_p = obstacles_saw[i].coor;
            TCoor dis_c2t = car_p.getDistance(enemy_p);//观察者到目标
            TCoor dis_c2o = car_p.getDistance(obs_p);//观察者到障碍
            TCoor l = abs(cos_d(theta)*(obs_p.y - car_p.y) + sin_d(theta)*(obs_p.x - car_p.x));//障碍到观察者-目标连线的距离
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
    //car->getView(cars_saw, obstacles_saw, props_saw);
}

//小车位置更新
Point<TCoor> Map::getNextPos(const Car * car, const Car* car_enemy) const
{
    
    Point<TCoor> coor_temp;
    TCoor l = 0.5*(car->getLeftSpeed() + car->getRightSpeed()) / FREQ;//位移距离
    TAngle beta = getNextAngle(car);
    coor_temp = Point<TCoor>{ car->getCoor().x + l*cos_d(beta), car->getCoor().y + l*sin_d(beta) };//指令想要移动的位置
    //障碍物判断
    for (int i = 0; i < Obstacle.size(); i++) {
        //其实只需要判断v*dt+R_c+R_o<dis_02c的障碍物
        TCoor dis_o2t = Obstacle[i].coor.getDistance(coor_temp);//预计位置和障碍物距离
        TCoor dis_o2c = Obstacle[i].coor.getDistance(car->getCoor());//原位置和障碍物距离
        if (dis_o2t < RADIUS_CAR + Obstacle[i].radius) {//如果在到达位置前已经和障碍物相碰
            //更新要到达位置
            double a = 0;
            double b = 1;
            double c;
            for (int j = 0; j < 10; j++) {//十次二分法
                c = (a + b) / 2;
                coor_temp = Point<TCoor>{ car->getCoor().x + l*c*cos_d(beta),car->getCoor().y + l*c*sin_d(beta) };
                dis_o2t = Obstacle[i].coor.getDistance(coor_temp);
                if (dis_o2t < RADIUS_CAR + Obstacle[i].radius)
                    b = c;
                else
                    a = c;
            }
            l = l*c;//更新前进距离
        }
    }
    //与另一辆小车的碰撞判断
    TCoor dis_c2c = car->getCoor().getDistance(car_enemy->getCoor());
    TCoor dis_c2t = coor_temp.getDistance(car_enemy->getCoor());
    if (dis_c2t<2*RADIUS_CAR){
        double a = 0;
        double b = 1;
        double c;
        for (int j = 0; j < 10; j++) {//十次二分法
            c = (a + b) / 2;
            coor_temp = Point<TCoor>{ car->getCoor().x + l*c*cos_d(beta), car->getCoor().y + l*c*sin_d(beta) };
            dis_c2t = coor_temp.getDistance(car_enemy->getCoor());
            if (dis_c2t < 2 * RADIUS_CAR)
                b = c;
            else
                a = c;
        }
        l = l*c;//更新前进距离
    }
    return coor_temp;
}

//小车角度更新
TAngle Map::getNextAngle(const Car * car) const
{
    TAngle deta = (car->getLeftSpeed() - car->getRightSpeed()) / 2 / RADIUS_CAR / FREQ * 180 / 3.14159;
    return fmod(car->getCarAngle() - deta + 360, 360);
}

//获取地图信息
MapInfo Map::getMapInfo() const
{
    MapInfo status{ 0,0,0,0,0,0 };
    status.HP1 = get_prop_status(props[0]);
    status.HP2 = get_prop_status(props[1]);
    status.MP1 = get_prop_status(props[2]);
    status.MP2 = get_prop_status(props[3]);
    status.speedBuf = get_prop_status(props[4]);
    status.defendBuff = get_prop_status(props[5]);
    return status;
}
