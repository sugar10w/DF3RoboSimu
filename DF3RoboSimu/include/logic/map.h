// 地图，包括障碍物判定等
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
    vector<Point<double>> Obstacle;//障碍物位置
    //car
    car car1;
    car car2;


};