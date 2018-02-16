// ��ͼ��������Ұ�������ж�����ײ�ж�

//��Ҫvoid slowdown(Point<double> coor, double car_angle)
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

#pragma once

#include"common.h"
#include"car.h"
#include"prop.h"

class Map {
public:
    //Prop
    Prop HpPackage_1 = Prop(HP_PAK, HP_PAK1_X, HP_PAK1_Y, 1);
    Prop HpPackage_2 = Prop(HP_PAK, HP_PAK2_X, HP_PAK2_Y, 2);
    Prop MpPackage_1 = Prop(MP_PAK, MP_PAK1_X, MP_PAK1_Y, 1);
    Prop MpPackage_2 = Prop(MP_PAK, MP_PAK2_X, MP_PAK2_Y, 2);
    Prop SpeedBuff = Prop(SPD_BUFF, SPEED_BUFF_X, SPEED_BUFF_Y, 0);
    Prop DefendBuff = Prop(DEF_BUFF, DEF_BUFF_X, DEF_BUFF_Y, 0);
    //Obstacle
    std::vector<Point<TCoor>> Obstacle;//�ϰ���λ��
    //car
    const Car* car[2];
};

