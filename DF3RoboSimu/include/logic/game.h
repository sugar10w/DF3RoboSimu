// 游戏主逻辑

//需要getTime()返回游戏当前帧号

//每一帧需要调用
//1.调用Map刷新道具
//2.随机顺序调用不同小车的car->frameRoutine()
//      (1)判断冷却状态
//      (2)调用用户函数
//      (3)移动到下一位置
//      (4)返回小车回放文件结构体
//3.超时减伤
//4.判断是否结束

#include "car.h"

class Game {
public:
    Game();
    int getTime() { return frame; }

private:
    int frame;
    Car *car1, *car2;
};