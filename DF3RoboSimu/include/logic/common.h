//���ÿ�
#pragma once

#include<cmath>

// ���ó���
const int FREQ = 30;
const int ROUND_MINUTE = 60 * FREQ;

//��ͼ����
const int MAP_WIDTH = 400;
const int MAP_HEIGHT = 300;

const double ATTACK_OOR = 5;//�����˺�

//���߲���
enum Prop_type { SPD_BUFF, DEF_BUFF, HP_PAK, MP_PAK };
const double RADIUS_PROP = 20;
const int PROP_START = 5 * FREQ;
const int BUFF_CD = 20 * FREQ;
const int PACKAGE_CD = 30 * FREQ;

template<class T>
class Point {
public:
    T x, y;

    Point() :x(0), y(0) {}
    Point(T _x, T _y) : x(_x), y(_y) {}
    Point() : x(0), y(0) {}
    Point(T _x, T _y) : x(_x), y(_y) {}

    double getDistance(Point _p) {
        return sqrt((x - _p.x)*(x - _p.x) + (y - _p.y)*(y - _p.y));

    }
    double getDistance(Point _p) {
        return sqrt((x - _p.x)*(x - _p.x) + (y - _p.y)*(y - _p.y));

    }
}