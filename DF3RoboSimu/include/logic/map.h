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
    Prop HpPackage_1 ;
    Prop HpPackage_2 ;
    Prop MpPackage_1 ;
    Prop MpPackage_2 ;
    Prop SpeedBuff ;
    Prop DefendBuff ;
    //Obstacle
    TCoor ObstacleRadius;
    std::vector<Point<TCoor>> Obstacle;//�ϰ���λ��
    //car
    Car* car1;
    Car* car2;
    
    //��ʼ����ͼ
    bool init();
    bool aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target);
    std::vector<Point<TCoor>> get_view(Point<TCoor> p_car);
};

