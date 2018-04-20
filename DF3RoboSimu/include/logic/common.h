// 公用常数//公用库
#pragma once

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>

// 类型定义 --- --- --- ---

// 坐标类型 单位cm
typedef double  TCoor;  
// 生命值类型
typedef double  THP;    
// 魔法值类型
typedef double  TMP;   
// 弹药数类型
typedef int     TMag;   
// 角度类型 单位deg
typedef double  TAngle; 
// 速度类型 单位cm/s
typedef double  TSpeed; 

// 时间类型 单位s
typedef float   TTime;  
// 帧数类型
typedef int     TFrame; 

// 玩家ID
enum PLAYER_ID {P0 = 0, P1 = 1, DRAW = 9, UNKNOWN_PLAYER = 10};  

// 公用常数 --- --- --- ---

// 每秒帧数（回合数）
const TFrame FREQ = 30;
// 一分钟等效的帧数（回合数）
const TFrame FRAME_MINUTE = 60 * FREQ;

//地图参数 --- --- --- ---

// 地图宽度，X最大值
const TCoor MAP_WIDTH = 400;
// 地图高度，Y最大值
const TCoor MAP_HEIGHT = 300;
// 出界伤害
const THP ATTACK_OOR = 5.0 / FREQ; 
// 给出超时额外伤害的时间点
const TFrame TIMEOUT_TIME[] = { 2 * FRAME_MINUTE, 3 * FRAME_MINUTE, 4 * FRAME_MINUTE };
// 超时额外伤害的数值，按回合计
const THP TIMEOUT_HP[] = { 1.0 / FREQ, 2.0 / FREQ, 3.0 / FREQ };

//小车参数 --- --- --- ---

// 小车半径
const TCoor RADIUS_CAR = 5;
// 小车生命值上限
const THP HP_MAX = 100.0;
// 小车魔法值上限
const TMP MP_MAX = 100.0;
// 小车每回合恢复生命值
const TMP MP_GAIN = 2.0 / FREQ;
// 小车弹夹上限
const TMag MAG_MAX = 12;
// 小车换弹所需回合数
const TFrame MAG_CHANGETIME = 2 * FREQ;
// 小车视角大小
const TAngle VIEW_DEG = 90.0;
// 小车旋转双目的速度（每回合可移动的角度）
const TAngle ROTATE_SPD = 90.0 / FREQ;

// 小车状态参数 --- --- --- ---

// 小车基础速度
const TSpeed SPD_BASE = 50.0;
// 小车速度状态
enum SPD_STATUS          { SPD_LOW, SPD_NORM, SPD_HIGH };
// 小车速度状态对应的速度变化比例
const double SPD_VAL[] = { 0.5,     1.0,      1.5 };

// 小车防御状态
enum DEF_STATUS          { DEF_NORM, DEF_ARM };
// 小车防御状态对应的收到伤害比例
const double DEF_VAL[] = { 1.0,     0.5 };

//PROP道具参数 --- --- --- ---

// 道具类型
enum PROP_TYPE                  { SPD_BUFF,     DEF_BUFF,   HP_PAK,     MP_PAK };
// 道具的冷却时间（回合数）
const TFrame PROP_CD_TIME[] =   { 15*FREQ,      15*FREQ,    20*FREQ,    20*FREQ};
// 道具的判定半径
const TCoor RADIUS_PROP = 5;
// 第一次出现道具的时间
const TFrame PROP_START_TIME = 2 * FREQ;
// HP道具恢复的生命值
const THP PROP_HP_PAK_POINT = 50;
// MP道具恢复的魔法值
const TMP PROP_MP_PAK_POINT = 50;

//BUFF技能参数 --- --- --- ---

// 技能是否在冷却
enum BUFF_CD_STATUS { BUFF_NORM, BUFF_CD };
// 技能类型
enum BUFF_TYPE                 { BUFF_SLOWDOWN,    BUFF_SPEEDUP,   BUFF_DEFEND };
// 释放技能所需魔法值
const TMP BUFF_MP[] =          { 30,               60,             60 };
// 技能冷却时间
const TFrame BUFF_CD_TIME[] =  { 10*FREQ,          20*FREQ,        20*FREQ };
// 技能持续时间
const TFrame BUFF_VALID_TIME = 10 * FREQ;

//ATK普攻参数 --- --- --- ---

// 命中位置
enum ATK_POS { ATK_FRONT, ATK_SIDE, ATK_BACK, ATK_MISS = 10 };
// 普攻等级
enum ATK_NUM_MAG { ATK_1MAG, ATK_2MAG, ATK_3MAG};
// 普攻等级对应的弹夹消耗
const TMag ATK_MAG_DEMAND[3] = { 1, 2, 3 };
// 普攻对应的伤害
const THP ATK_POINTS[3][3] = { {2,3,4},{4,6,8},{6,9,12} };
// 普攻冷却时间
const TFrame ATK_CD_TIME = 0.5 * FREQ;
// 最佳攻击范围（满命中距离）
const TCoor ATK_BEST_LEN = 30;
// 极限攻击范围（不可能命中临界距离，视野极限距离）
const TCoor ATK_MAX_LEN = 200;
// 击退长度
const TCoor ATK_KNOCKED_DIST[3] = { 5, 10, 15 };

// 地图坐标 --- --- --- ---

// 二维地图点
template<class T> class Point {
public:
    T x, y;

    Point() :x(0), y(0) {}
    Point(T _x, T _y) :x(_x), y(_y) {}

    double getDistance(const Point& _p) const {
        return sqrt((double)((x - _p.x)*(x - _p.x) + (y - _p.y)*(y - _p.y)));
    }

    Point operator+(const Point& _p) const {
        return Point(x + _p.x, y + _p.y);
    }
    Point operator-(const Point& _p) const {
        return Point(x - _p.x, y - _p.y);
    }
    Point operator*(double _d) const {
        return Point(x*_d, y*_d);
    }
    Point operator/(double _d) const {
        return Point(x/_d, y/_d);
    }
};

// 玩家视野内的小车信息
typedef struct
{
    Point<TCoor> coor;
    THP hp;
    TAngle tank_angle, attack_angle;
    PLAYER_ID team;
} car_info;
// 玩家视野内的障碍物信息
typedef struct
{
    Point<TCoor> coor;
    TCoor radius;
} obs_info;
// 玩家视野内的道具信息
typedef struct
{
    PROP_TYPE tp;
    Point<TCoor> pos;
} prop_info;

