// ���ó���//���ÿ�
#pragma once

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>

// ���Ͷ��� --- --- --- ---

// �������� ��λcm
typedef double  TCoor;  
// ����ֵ����
typedef double  THP;    
// ħ��ֵ����
typedef double  TMP;   
// ��ҩ������
typedef int     TMag;   
// �Ƕ����� ��λdeg
typedef double  TAngle; 
// �ٶ����� ��λcm/s
typedef double  TSpeed; 

// ʱ������ ��λs
typedef float   TTime;  
// ֡������
typedef int     TFrame; 

// ���ID
enum PLAYER_ID {P0 = 0, P1 = 1, DRAW = 9, UNKNOWN_PLAYER = 10};  

// ���ó��� --- --- --- ---

// ÿ��֡�����غ�����
const TFrame FREQ = 30;
// һ���ӵ�Ч��֡�����غ�����
const TFrame FRAME_MINUTE = 60 * FREQ;

//��ͼ���� --- --- --- ---

// ��ͼ��ȣ�X���ֵ
const TCoor MAP_WIDTH = 400;
// ��ͼ�߶ȣ�Y���ֵ
const TCoor MAP_HEIGHT = 300;
// �����˺�
const THP ATTACK_OOR = 5.0 / FREQ; 
// ������ʱ�����˺���ʱ���
const TFrame TIMEOUT_TIME[] = { 2 * FRAME_MINUTE, 3 * FRAME_MINUTE, 4 * FRAME_MINUTE };
// ��ʱ�����˺�����ֵ�����غϼ�
const THP TIMEOUT_HP[] = { 1.0 / FREQ, 2.0 / FREQ, 3.0 / FREQ };

//С������ --- --- --- ---

// С���뾶
const TCoor RADIUS_CAR = 5;
// С������ֵ����
const THP HP_MAX = 100.0;
// С��ħ��ֵ����
const TMP MP_MAX = 100.0;
// С��ÿ�غϻָ�����ֵ
const TMP MP_GAIN = 2.0 / FREQ;
// С����������
const TMag MAG_MAX = 12;
// С����������غ���
const TFrame MAG_CHANGETIME = 2 * FREQ;
// С���ӽǴ�С
const TAngle VIEW_DEG = 90.0;
// С����ת˫Ŀ���ٶȣ�ÿ�غϿ��ƶ��ĽǶȣ�
const TAngle ROTATE_SPD = 90.0 / FREQ;

// С��״̬���� --- --- --- ---

// С�������ٶ�
const TSpeed SPD_BASE = 50.0;
// С���ٶ�״̬
enum SPD_STATUS          { SPD_LOW, SPD_NORM, SPD_HIGH };
// С���ٶ�״̬��Ӧ���ٶȱ仯����
const double SPD_VAL[] = { 0.5,     1.0,      1.5 };

// С������״̬
enum DEF_STATUS          { DEF_NORM, DEF_ARM };
// С������״̬��Ӧ���յ��˺�����
const double DEF_VAL[] = { 1.0,     0.5 };

//PROP���߲��� --- --- --- ---

// ��������
enum PROP_TYPE                  { SPD_BUFF,     DEF_BUFF,   HP_PAK,     MP_PAK };
// ���ߵ���ȴʱ�䣨�غ�����
const TFrame PROP_CD_TIME[] =   { 15*FREQ,      15*FREQ,    20*FREQ,    20*FREQ};
// ���ߵ��ж��뾶
const TCoor RADIUS_PROP = 6;
// ��һ�γ��ֵ��ߵ�ʱ��
const TFrame PROP_START_TIME = 2 * FREQ;
// HP���߻ָ�������ֵ
const THP PROP_HP_PAK_POINT = 50;
// MP���߻ָ���ħ��ֵ
const TMP PROP_MP_PAK_POINT = 50;

//BUFF���ܲ��� --- --- --- ---

// �����Ƿ�����ȴ
enum BUFF_CD_STATUS { BUFF_NORM, BUFF_CD };
// ��������
enum BUFF_TYPE                 { BUFF_SLOWDOWN,    BUFF_SPEEDUP,   BUFF_DEFEND };
// �ͷż�������ħ��ֵ
const TMP BUFF_MP[] =          { 30,               60,             60 };
// ������ȴʱ��
const TFrame BUFF_CD_TIME[] =  { 10*FREQ,          20*FREQ,        20*FREQ };
// ���ܳ���ʱ��
const TFrame BUFF_VALID_TIME = 10 * FREQ;

//ATK�չ����� --- --- --- ---

// ����λ��
enum ATK_POS { ATK_FRONT, ATK_SIDE, ATK_BACK, ATK_MISS = 10 };
// �չ��ȼ�
enum ATK_NUM_MAG { ATK_1MAG, ATK_2MAG, ATK_3MAG};
// �չ��ȼ���Ӧ�ĵ�������
const TMag ATK_MAG_DEMAND[3] = { 1, 2, 3 };
// �չ���Ӧ���˺�
const THP ATK_POINTS[3][3] = { {2,3,4},{4,6,8},{6,9,12} };
// �չ���ȴʱ��
const TFrame ATK_CD_TIME = 0.5 * FREQ;
// ��ѹ�����Χ�������о��룩
const TCoor ATK_BEST_LEN = 30;
// ���޹�����Χ�������������ٽ���룬��Ұ���޾��룩
const TCoor ATK_MAX_LEN = 200;
// ���˳���
const TCoor ATK_KNOCKED_DIST[3] = { 5, 10, 15 };

// ��ͼ���� --- --- --- ---

// ��ά��ͼ��
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

// �����Ұ�ڵ�С����Ϣ
typedef struct
{
    Point<TCoor> coor;
    THP hp;
    TAngle tank_angle, attack_angle;
    PLAYER_ID team;
} car_info;
// �����Ұ�ڵ��ϰ�����Ϣ
typedef struct
{
    Point<TCoor> coor;
    TCoor radius;
} obs_info;
// �����Ұ�ڵĵ�����Ϣ
typedef struct
{
    PROP_TYPE tp;
    Point<TCoor> pos;
} prop_info;

