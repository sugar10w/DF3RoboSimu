// 地图，包括视野、命中判定、碰撞判定


#pragma once

class Map;

#include "logic/common.h"
#include "logic/car.h"
#include "logic/prop.h"
#include "logic/game.h"
#include "logic/log_format.h"

static const char* DEFAULT_MAP_FILENAME = "../data/map.txt";

class Map {

private:
    //Prop
    std::vector<Prop> props;
    //障碍物位置 Obstacle 
    std::vector<obs_info> Obstacle; 
    // 是否有效
    bool valid;
    //car 其实只需要两个车的位置信息，考虑直接向Game索要
    const Game* game; //TODO
    const char* filename = DEFAULT_MAP_FILENAME;

public:
    
    // 是否有效
    inline bool isValid() const { return valid; };

    // 构造函数，导入const Game*和const char*输入文件名
    Map(const Game* _game, const char* _filename = DEFAULT_MAP_FILENAME)
        : game(_game)
    {
        filename = _filename;
        valid = init(_game, _filename);
    }

    // 初始化地图
    bool init(const Game* _game, const char* filename = DEFAULT_MAP_FILENAME);
    
    // 更新道具
    void refreshProp(TFrame frame);
    
    // 给出初始坐标，输出到参数内
    bool getInitPos(PLAYER_ID id, Point<TCoor>& birth_point, TAngle& car_angle) const;

    // 判断命中
    ATK_POS aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target, TAngle target_angle);
    
    // 刷新小车视野 TODO 改成获得返回值，保留const设定
    void getView(Car* car, std::vector<car_info>& cars,
        std::vector<car_info>& cars_saw,
        std::vector<prop_info>& props_saw,
        std::vector<obs_info>& obstacles_saw) const;
    
    // 获取下一瞬间坐标
    Point<TCoor> getNextPos(const Car* car, const Car* car_enemy) const; 
    TAngle getNextAngle(const Car* car) const;

    // 地图当前信息，用于导出
    MapInfo getMapInfo() const;

    // 小车获取哪个道具，未获取返回-1
    PROP_TYPE getProp(const Car* car);

};

