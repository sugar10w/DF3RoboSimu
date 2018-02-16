// 地图，包括视野、命中判定、碰撞判定

//需要void slowdown(Point<double> coor, double car_angle)
//1.判断被攻击小车
//2.调用该car->slowedDown()


//需要void attack(Point<double> coor, double attack_angle, ATK_NUM_MAG mag_num)
//1.判断被攻击小车
//2.调用该car->setHP()

//需要void setNextPos(Car* car)
//1.计算下一处坐标与方向（考虑与障碍物或其他小车碰撞），出界惩罚
//2.判断道具拾取

//需要void getView(Car* car, std::vector<car_info>& cars, 
//    std::vector<obs_info>& obs, std::vector<prop_info>& props)
//获取小车视野内其余小车、障碍物、道具的信息

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
    std::vector<Point<TCoor>> Obstacle;//障碍物位置
    //car
    const Car* car[2];
};

