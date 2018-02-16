// 游戏主逻辑

//需要getTime()返回游戏当前帧号

//每一帧需要调用
//0.调用用户函数
//1.调用Map刷新道具
//2.奇偶交替调用小车的car->frameRoutine()
//      (1)判断冷却状态
//      (2)移动到下一位置
//      (3)释放技能并相互伤害
//      (4)返回小车回放文件结构体
//3.地图伤害：超时减伤
//4.判断是否结束

#pragma once

#include "map.h"
#include "car.h"

class Game {
public:

    // 构造函数；创造Map和Car
    Game();
    
    
    TFrame getTime() const { return frame; }

private:
    TFrame frame;
    Map *map;
    Car *car[2];
};