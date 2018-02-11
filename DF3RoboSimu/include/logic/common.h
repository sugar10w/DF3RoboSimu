//公用库
#pragma once

#include <cmath>

// 公用常数
const int FREQ = 30;
const int ROUND_MINUTE = 60 * FREQ;

//地图参数
const int MAP_WIDTH = 400;
const int MAP_HEIGHT = 300;
const double ATTACK_OOR = 5 / FREQ;  //出界伤害


//小车参数
const double HP_MAX = 100.0;
const int MP_MAX = 100;
const int MP_GAIN = 2;
const int MAG_MAX = 12;
const int MAG_CHANGETIME = 2 * FREQ;
const double VIEW_DEG = 45;

const double SPD_BASE = 1.0;
enum SPD_STATUS { SPD_LOW, SPD_NORM, SPD_HIGH, SPD_STATUS_NUM };
const double SPD_VAL[SPD_STATUS_NUM] = { 0.5, 1.0, 1.5 };

enum DEF_STATUS { DEF_ARM, DEF_NORM, DEF_STATUS_NUM };
const double DEF_VAL[DEF_STATUS_NUM] = { 0.5, 1.0 };

enum ATK_STATUS { ATK_NORM, ATK_CD, ATK_MAG };
enum ATK_NUM_MAG { ATK_1MAG, ATK_2MAG, ATK_3MAG, ATK_NUM_MAG_NUM };
enum ATK_POS { ATK_FRONT, ATK_SIDE, ATK_BACK, ATK_POS_NUM };

//道具参数
enum Prop_type { SPD_BUFF, DEF_BUFF, HP_PAK, MP_PAK };

const double RADIUS_PROP = 20;
const int PROP_START_TIME = 5 * FREQ;
const int BUFF_CD_TIME = 20 * FREQ;
const int BUFF_VALID_TIME = 10 * FREQ;
const int PACKAGE_VAL = 40;
const int PACKAGE_CD_TIME = 30 * FREQ;
const int ATK_CD_TIME = 0.5 * FREQ;
enum BUFF_TYPE { BUFF_SLOWDOWN, BUFF_SPEEDUP, BUFF_PROTECT, BUFF_TYPE_NUM };
const int BUFF_MP[BUFF_TYPE_NUM] = { 40, 60, 60 };
const double ATK_POINTS[ATK_NUM_MAG_NUM][ATK_POS_NUM] = { { 5,6,7 },{ 12,14,16 },{ 21,24,27 } };


//地图坐标
template<class T>
class Point {
public:
    T x, y;

    Point() :x(0), y(0) {}
    Point(T _x, T _y) :x(_x), y(_y) {}

    double getDistance(Point _p) {
        return sqrt((x - _p.x)*(x - _p.x) + (y - _p.y)*(y - _p.y));
    }

};