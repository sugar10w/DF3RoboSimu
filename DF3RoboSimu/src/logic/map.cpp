
// ��ͼ�������ϰ����ж���
#include "logic/map.h"

using namespace std;

static double cos_d(double theta) { return cos(theta / 180 * 3.14159); }
static double sin_d(double theta) { return sin(theta / 180 * 3.14159); }
static double tan_d(double theta) { return tan(theta / 180 * 3.14159); }
static double atan2_d(double y, double x) { return 180 / 3.14159*atan2(y, x); }
static double asin_d(double s) { return 180 / 3.14159*asin(s); }
static double minus_angle_d(double t1, double t2) {
    double u = fmod(t1 - t2 + 360, 360);
    if (u > 180) u -= 360;
    return u;
}

// ��ȡ����״̬
static TFrame get_prop_status(Prop p) {
    if (p.is_available()) {
        return p.get_ET();
    }
    else {
        return(p.getCD_count() - p.getCD());
    }
}
// �ֽ��ַ����������ļ���ȡ
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


//��ʼ��
bool Map::init(const Game* _game, const char* filename)
{
    game = _game;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "δ�ܳɹ��򿪵�ͼ�ļ�" << filename << endl;
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
        //�˴�δ��ʼ��Car

        getline(file, temp); getline(file, temp);
        //paras = split(temp, ' ');
        //car2 = new Car(Point<TCoor>(atof(paras[0].c_str()), atof(paras[1].c_str())), atof(paras[3].c_str()), 2);
        //�˴�δ��ʼ��Car

        while (getline(file, temp))
        {
            paras = split(temp, ' ');
            if (paras.size() < 3) continue;
            obs_info t = { Point<TCoor>(atof(paras[0].c_str()), atof(paras[1].c_str())),atof(paras[2].c_str()) };
            Obstacle.push_back(t);
        }
        return true;
    }
}

//����ˢ��
void Map::refreshProp(TFrame frame)
{
    //�����ʼ����ÿ�ֵ���
    if (game->getTime() >= PROP_START_TIME) {
        for (int i = 0; i < props.size(); i++)
            props[i].round_operation();
    }
}

//���س�ʼ������
bool Map::getInitPos(PLAYER_ID id, Point<TCoor>& birth_point, TAngle & car_angle) const
{
    // TODO ��Ҫ���¶��ļ��ˣ�û�б�Ҫ
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "δ�ܳɹ��򿪵�ͼ�ļ�" << filename << endl;
        return false;
    }
    else {
        string temp;
        vector<string> paras;
        for (int i = 0; i < 6; i++) {
            getline(file, temp, '#'); getline(file, temp); //�������߲���
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

//�����ж�
ATK_POS Map::aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target, TAngle target_angle)
{
    bool hit = true;

    TAngle center_c = fmod(car_angle + attack_angle + 360, 360);

    TCoor distance_t = P_attack.getDistance(P_target);
    TAngle center_t = atan2_d(P_target.y - P_attack.y, P_target.x - P_attack.x);
    TAngle width_t = abs(asin_d(RADIUS_CAR / distance_t));

    if (distance_t > ATK_MAX_LEN) return ATK_MISS;
    if (abs(minus_angle_d(center_c, center_t)) > width_t) return ATK_MISS;

    for (int i = 0; i < Obstacle.size(); ++i) {
        TCoor distance_o = Obstacle[i].coor.getDistance(P_attack);
        if (distance_o > distance_t) continue;

        TAngle center_o = atan2_d(Obstacle[i].coor.y - P_attack.y, Obstacle[i].coor.x - P_attack.x);
        TAngle width_o = abs(asin_d(Obstacle[i].radius / distance_o));

        hit = hit && abs(minus_angle_d(center_c, center_o)) > width_o;
        if (!hit) break;
    }

    if (hit) {
        TAngle deta_phi = abs(minus_angle_d(center_c, target_angle));
        if (deta_phi <= 45)
            return ATK_BACK;//���б���
        else if (deta_phi <= 135)
            return ATK_SIDE;//���в���
        else
            return ATK_FRONT;//��������
    }
    return ATK_MISS;//δ����  

}

//��Ұ�ж�
void Map::getView(Car * car, vector<car_info>& cars,
        vector<car_info>& cars_saw,
        vector<prop_info>& props_saw,
        vector<obs_info>& obstacles_saw) const 
{    
    cars_saw.clear();
    props_saw.clear();
    obstacles_saw.clear();

    int id = (int)car->getTeam();
    Point<TCoor> car_p = cars[id].coor;
    Point<TCoor> enemy_p = cars[1 - id].coor;

    //����
    for (int i = 0; i < props.size(); i++) {
        if (props[i].is_available() && props[i].if_team(car->getTeam())) {
            prop_info prop_i = { props[i].get_type(), Point<TCoor>(TCoor(props[i].get_pos().x),TCoor(props[i].get_pos().y)) };
            props_saw.push_back(prop_i);
        }
    }
    //�ϰ���
    for (int i = 0; i < Obstacle.size(); i++) {
        obstacles_saw.push_back(Obstacle[i]);
    }
    //�з�С���ж�
    {
        // ֻ���з�С���е㡢���Ե���ұ�Ե�ܷ񿴼���
        // ����һ�������ܿ�����

        TAngle center_c = car->getViewAngle();
        TAngle width_c = VIEW_DEG / 2;

        TCoor distance_t = car_p.getDistance(enemy_p);
        TAngle center_t = atan2_d(enemy_p.y - car_p.y, enemy_p.x - car_p.x);
        TAngle width_t = abs(asin_d(RADIUS_CAR / distance_t));
        TAngle left_t = center_t - width_t, 
            right_t = center_t + width_t;
        
        bool left_visible = abs(minus_angle_d(left_t, center_c)) < width_c, 
            right_visible = abs(minus_angle_d(right_t, center_c)) < width_c,
            center_visible = abs(minus_angle_d(center_t, center_c)) < width_c;
        
        for (int i = 0; i < Obstacle.size(); ++i) {
            TCoor distance_o = Obstacle[i].coor.getDistance(car_p);
            if (distance_o > distance_t) continue;

            TAngle center_o = atan2_d(Obstacle[i].coor.y - car_p.y, Obstacle[i].coor.x - car_p.x);
            TAngle width_o = abs(asin_d(Obstacle[i].radius / distance_o));

            left_visible = left_visible && abs(minus_angle_d(left_t, center_o)) > width_o,
            right_visible = right_visible && abs(minus_angle_d(right_t, center_o)) > width_o,
            center_visible = center_visible && abs(minus_angle_d(center_t, center_o)) > width_o;

        }

        if (left_visible || right_visible || center_visible) 
            cars_saw.push_back(cars[1 - id]);

    }

}

// Բ�μ���ģ��
static Point<TCoor> squeezeOut(Point<TCoor> raw, Point<TCoor> obs, TCoor radius) {
    TCoor d = raw.getDistance(obs);
    Point<TCoor> delta;
    double ratio;

    if (d > radius) return raw;

    delta = raw - obs;
    ratio = radius / d;
    return obs + delta * ratio;

}
//С��λ�ø���
Point<TCoor> Map::getNextPos(const Car * car, const Car* car_enemy) const
{
    
    Point<TCoor> coor_temp;
    TCoor l = 0.5*(car->getLeftSpeed() + car->getRightSpeed()) / FREQ;//λ�ƾ���
    TAngle beta = getNextAngle(car);
    coor_temp = Point<TCoor>{ car->getCoor().x + l*cos_d(beta), car->getCoor().y + l*sin_d(beta) };//ָ����Ҫ�ƶ���λ��
    
    // �ϰ���
    for (int i = 0; i < Obstacle.size(); i++) {
        coor_temp = squeezeOut(coor_temp, Obstacle[i].coor, Obstacle[i].radius + RADIUS_CAR);
    }
    // �Է�С��
    coor_temp = squeezeOut(coor_temp, car_enemy->getCoor(), 2 * RADIUS_CAR);

    return coor_temp;
}

//С���Ƕȸ���
TAngle Map::getNextAngle(const Car * car) const
{
    TAngle deta = (car->getLeftSpeed() - car->getRightSpeed()) / 2 / RADIUS_CAR / FREQ * 180 / 3.14159;
    return fmod(car->getCarAngle() - deta + 360, 360);
}

//��ȡ��ͼ��Ϣ
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


PROP_TYPE Map::getProp(const Car* car)
{
    for (int i = 0; i < props.size();++i)
    {
        if (props[i].if_gotcha(car->getCoor(), car->getTeam()))
        {
            props[i].get_pickedup();
            return props[i].get_type();
        }
    }
    return (PROP_TYPE)-1;
}