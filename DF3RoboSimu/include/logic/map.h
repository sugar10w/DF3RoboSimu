// ��ͼ��������Ұ�������ж�����ײ�ж�

//��Ҫvoid slowdown(Point<double> coor, double car_angle��)
//1.�жϱ�����С��
//2.���ø�car->slowedDown()


//��Ҫvoid attack(Point<double> coor, double attack_angle, ATK_NUM_MAG mag_num)
//1.�жϱ�����С��
//2.���ø�car->setHP()

//��Ҫvoid setNextPos(Car* car)
//1.������һ�������뷽�򣨿������ϰ��������С����ײ��������ͷ�
//2.�жϵ���ʰȡ

//��Ҫvoid getView(Car* car, std::vector<car_info>& cars, 
//    std::vector<obs_info>& obs, std::vector<prop_info>& props)
//��ȡС����Ұ������С�����ϰ�����ߵ���Ϣ

#include"common.h"
#include"car.h"
#include"prop.h"
class Map {
public:
    //Prop
    std::vector<Prop> props;
    //Obstacle
    std::vector<obs_info> Obstacle;
    //car
    Car* car1;
    Car* car2;
    
    //��ʼ����ͼ
    bool init();
    int aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target, TAngle target_angle);
    void getView(Car* car, std::vector<car_info>& cars);
};

