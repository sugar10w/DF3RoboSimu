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

//构造函数中传递char* mapFile
//Map::Map(char* mapFile)

//地图文件中需要定义好两个小车的起始点和车头方向，并提供函数获取
//void Map::getInitPos(PLAYER_ID player_id, Point<TCoor>& coor, double& car_angle)

//需要刷新道具的函数
//void Map::refreshProp(TFrame frame)

//需要回放文件信息
//MapInfo Map::getMapInfo()

#pragma once

#include"common.h"
#include"car.h"
#include"prop.h"
#include "game.h"

class Map {

private:
    //Prop
    std::vector<Prop> props;
    //障碍物位置 Obstacle 
    std::vector<obs_info> Obstacle; 
    //car 其实只需要两个车的位置信息，考虑直接向Game索要
    const Game* game; //TODO

public:
    
    // 构造函数，导入const Game*和const char*输入文件名
    Map(const Game* _game, const char* filename = "../../data/map.txt") {
        init(_game, filename);
    }

    // 初始化地图
    bool init(const Game* _game, const char* filename = "../../data/map.txt");
    
    // 更新道具
    void refreshProp(TFrame frame);
    
    // 给出初始坐标，输出到参数内
    bool getInitPos(PLAYER_ID id, Point<TCoor>& birth_point, TAngle& car_angle) const;

    // 判断命中
    int aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target, TAngle target_angle);
    // 刷新小车视野 TODO 改成获得返回值，保留const设定
    void getView(Car* car, std::vector<car_info>& cars) const;
    
    // 获取下一瞬间坐标
    Point<TCoor> getNextPos(const Car* car) const;

    // 地图当前信息，用于导出
    MapInfo getMapInfo() const;

};

