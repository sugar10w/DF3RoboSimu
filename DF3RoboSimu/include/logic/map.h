// 地图，包括视野、命中判定、碰撞判定

//需要void slowdown(Point<double> coor, double car_angle，)
//1.判断被减速小车
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
    std::vector<Point<TCoor>> Obstacle;//障碍物位置
    //car
    Car* car1;
    Car* car2;
    
    //初始化地图
    bool init();
    bool aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target);
    std::vector<Point<TCoor>> get_view(Point<TCoor> p_car);
};

