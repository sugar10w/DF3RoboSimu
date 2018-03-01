#include "..\..\include\logic\map.h"

// ��ͼ�������ϰ����ж���

bool Map::init(const Game* _game, const char* filename)
{
    game = _game;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "δ�ܳɹ��򿪵�ͼ�ļ�" << filename << std::endl;
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
        //toDo: �޸�


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

void Map::refreshProp(TFrame frame)
{
    //�����ʼ����ÿ�ֵ���
    if (game->getTime() >= PROP_START_TIME) {
        for (int i = 0; i < props.size(); i++)
            props[i].round_operation();
    }
}

bool Map::getInitPos(PLAYER_ID id, Point<TCoor>& birth_point, TAngle & car_angle) const
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "δ�ܳɹ��򿪵�ͼ�ļ�" << filename << std::endl;
        return false;
    }
    else {
        std::string temp;
        std::vector<std::string> paras;
        for (int i = 0; i < 6; i++) {
            std::getline(file, temp);//�������߲���
        }
        std::getline(file, temp);
        //car1
        if (id == 1) {
            paras = split(temp, ',');
            birth_point = Point<TCoor>(atof(paras[1].c_str()), atof(paras[2].c_str()));
            car_angle = atof(paras[3].c_str());
            return true;
        }
        std::getline(file, temp);
        //car2
        if (id == 2) {
            paras = split(temp, ',');
            birth_point = Point<TCoor>(atof(paras[1].c_str()), atof(paras[2].c_str()));
            car_angle = atof(paras[3].c_str());
            return true;
        }
        return false;
    }
}

hit_status Map::aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target, TAngle target_angle)
{

    TAngle theta = car_angle + attack_angle;//��x������н�
    //l: cos(theta)*(y-ya)+sin(theta)*��x-xa)=0
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
            TCoor l = P_attack.getDistance(P_target);//�����ߺ�Ŀ��Բ�ľ���
            TCoor s = sqrt(l*l - distance*distance) - sqrt(RADIUS_CAR - distance);
            //���е�
            TCoor xx = P_attack.x + cos_d(theta)*s;
            TCoor yy = P_attack.y + sin_d(theta)*s;

            TAngle deta_phi = abs(atan2_d(yy, xx) - target_angle);
            if (deta_phi <= 45)
                return front;//��������
            else if (deta_phi <= 135)
                return side;//���в���
            else
                return back;//���б���
        }
    }
    return miss;//δ����  
}

void Map::getView(Car * car, std::vector<car_info>& cars) const {
    std::vector<car_info> cars_saw;
    std::vector<prop_info> props_saw;
    std::vector<obs_info>obstacles_saw;

    Point<TCoor> car_p = car->getCoor();
    //�����ж�
    for (int i = 0; i < props.size(); i++) {
        Point<TCoor> prop_p = props[i].get_pos();
        bool be_cover = false;
        TAngle theta = atan2_d(car_p.y - prop_p.y, car_p.x - prop_p.x);//�����������ֱ�ߵĽǶ�
        TAngle deta_phi = abs(theta - car->getAttackAngle());//����Ұ���ߵļн�

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
    //�ϰ����ж�
    for (int i = 0; i < Obstacle.size(); i++) {
        Point<TCoor> obs_p = Obstacle[i].coor;
        TAngle theta = atan2_d(-car_p.y + obs_p.y, -car_p.x + obs_p.x);
        TAngle deta_phi = abs(theta - car->getAttackAngle());
        if (deta_phi <= 22.5 + abs(asin(Obstacle[i].radius / obs_p.getDistance(car_p))))
            obstacles_saw.push_back(Obstacle[i]);
    }
    //�з�С���ж�
    Point<TCoor> enemy_p = cars[0].coor;
    TAngle theta = atan2_d(enemy_p.y - car_p.y, enemy_p.x - car_p.x);
    TAngle deta_phi = abs(theta - car->getAttackAngle());
    bool is_visible = true;
    if (deta_phi <= 22.5 + abs(asin(RADIUS_CAR / enemy_p.getDistance(car_p)))) {
        for (int i = 0; i < obstacles_saw.size(); i++) {
            Point<TCoor> obs_p = obstacles_saw[i].coor;
            TCoor dis_c2t = car_p.getDistance(enemy_p);//�۲��ߵ�Ŀ��
            TCoor dis_c2o = car_p.getDistance(obs_p);//�۲��ߵ��ϰ�
            TCoor l = abs(cos_d(theta)*(obs_p.y - car_p.y) + sin_d(theta)*(obs_p.x - car_p.x));//�ϰ����۲���-Ŀ�����ߵľ���
            TCoor h = abs((enemy_p.x - car_p.x)*(obs_p.x - car_p.x) + (enemy_p.y - car_p.y)*(obs_p.y - car_p.y)) / dis_c2o;//�����߾���
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

Point<TCoor> Map::getNextPos(const Car * car) const
{
    
    Point<TCoor> coor_temp;
    TCoor l = 0.5*(car->getLeftSpeed() + car->getRightSpeed()) / FREQ;//λ�ƾ���
    TAngle beta = getNextAngle(car);
    coor_temp = Point<TCoor>{ car->getCoor().x + l*cos_d(beta),car->getCoor().y + l*sin_d(beta) };//ָ����Ҫ�ƶ���λ��
    for (int i = 0; i < Obstacle.size(); i++) {
        TCoor dis_o2t = Obstacle[i].coor.getDistance(coor_temp);//Ԥ��λ�ú��ϰ������
        TCoor dis_o2c = Obstacle[i].coor.getDistance(car->getCoor());//ԭλ�ú��ϰ������
        if (dis_o2t< RADIUS_CAR + Obstacle[i].radius) {//����ڵ���λ��ǰ�Ѿ����ϰ�������
            //����Ҫ����λ��
            double deta = (dis_o2t*dis_o2t - l*l - dis_o2c*dis_o2c)*(dis_o2t*dis_o2t - l*l - dis_o2c*dis_o2c) - 4 * l*l*((RADIUS_CAR + Obstacle[i].radius)*(RADIUS_CAR + Obstacle[i].radius) - dis_o2c*dis_o2c);//��ʽʮ�ָ��ӣ����ܳ���
            if (deta >= 0) {
                TCoor l = (dis_o2t*dis_o2t - l*l - dis_o2c*dis_o2c - sqrt(deta)) / 2 / l;
                coor_temp = Point<TCoor>{ car->getCoor().x + l*cos_d(beta),car->getCoor().y + l*sin_d(beta) };
            }
        }
    }
    return coor_temp;
}

TAngle Map::getNextAngle(const Car * car) const
{
    TAngle deta = (car->getLeftSpeed() - car->getRightSpeed()) / 4 / RADIUS_CAR / FREQ * 180 / 3.14159;
    return (car->getCarAngle()-deta);
}

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

TFrame get_prop_status(Prop p){
    if (p.is_available) {
        return p.get_ET();
    }
    else {
        return(p.getCD_count() - p.getCD());
    }
}


// �ֽ��ַ����������ļ���ȡ
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