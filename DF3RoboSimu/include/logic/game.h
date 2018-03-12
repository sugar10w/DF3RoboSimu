// 游戏主逻辑

//需要getTime()返回游戏当前帧号

//每一帧需要调用
//0.奇偶交替调用用户函数
//1.地图伤害：超时减伤
//2.调用Map刷新道具
//3.调用小车的car->frameRoutine()
//      (1)判断冷却状态
//      (2)移动到下一位置
//      (3)返回小车回放文件结构体
//4.写回放文件
//5.判断是否结束

#pragma once

#include <iostream>
#include <fstream>
#include <ctime>

class Game;

#include "logic/map.h"
#include "logic/car.h"


class Game {
public:
    friend int main(int argc, char** argv);

    // 构造函数；创造Map和Car
    Game(char* recordFile, char* mapFile);
    // 获取当前的帧数
    TFrame getTime() const { return frame; }

    /* TODO 
    bool 交互动作 {
        if (map->判断命中) {
            对方被攻击; 
            return true;
        } 
        return false;
    } 
    */

    // Car的交互动作：攻击
    bool attack(PLAYER_ID id);
    // Car的交互动作：减速
    bool slowdown(PLAYER_ID id);
    

protected:
    // 每帧更新状态
    //     @return    P0/P1 为获胜方，DRAW为平局，UNKNOWN_PLAYER为还未分出胜负
    PLAYER_ID frameRoutine();

private:
    TFrame frame = 0;
    Map *map = NULL;
    Car *car[2] = { NULL, NULL };
    std::fstream record_file;
    int randframe = 0; // 随机数确定第一帧先后顺序
};