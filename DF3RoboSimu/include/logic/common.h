// ���ó���//���ÿ�
#pragma once

#include <cmath>

// ���Ͷ���
typedef double  TCoor;  // ����
typedef float   THP;    // ����ֵ
typedef int     TMP;    // ħ��ֵ
typedef int     TMag;   // ��ҩ��
typedef double  TAngle; // �Ƕ�
typedef double  TSpeed; // �ٶ�

typedef float   TTime;  // ʱ��
typedef int     TFrame; // ֡��

enum PLAYER_ID {P0 = 0, P1 = 1, UNKNOWN_PLAYER = 10};  // ���ID

// ���ó���
const TFrame FREQ = 30;
const TFrame FRAME_MINUTE = 60 * FREQ;

//��ͼ����
const TCoor MAP_WIDTH = 400;
const TCoor MAP_HEIGHT = 300;
const THP ATTACK_OOR = 5 / FREQ;  //�����˺�

//С������
const THP HP_MAX = 100.0;
const THP MP_MAX = 100;
const THP MP_GAIN = 2;
const THP MAG_MAX = 12;
const THP MAG_CHANGETIME = 2 * FREQ;
const TAngle VIEW_DEG = 45;
const TAngle ROTATE_SPD = 90.0 / FREQ;

// ״̬����
const TSpeed SPD_BASE = 1.0;
enum SPD_STATUS          { SPD_LOW, SPD_NORM, SPD_HIGH };
const double SPD_VAL[] = { 0.5,     1.0,      1.5 };

enum DEF_STATUS          { DEF_NORM, DEF_ARM };
const double DEF_VAL[] = { 0.5,     1.0 };

//PROP���߲���
enum PROP_TYPE                  { SPD_BUFF,     DEF_BUFF,   HP_PAK,     MP_PAK };
const TFrame PROP_CD_TIME[] =   { 20*FREQ,      20*FREQ,    30*FREQ,    30*FREQ};
const TCoor RADIUS_PROP = 20;
const TFrame PROP_START_TIME = 5 * FREQ;
//BUFF���ܲ���
enum BUFF_CD_STATUS { BUFF_NORM, BUFF_CD };
enum BUFF_TYPE                 { BUFF_SLOWDOWN,    BUFF_SPEEDUP,   BUFF_DEFEND };
const TMP BUFF_MP[] =          { 40,               60,             60 };
const TFrame BUFF_CD_TIME[] =  { 10*FREQ,          20*FREQ,        20*FREQ };
const TFrame BUFF_VALID_TIME = 10 * FREQ;
//ATK�չ�����
enum ATK_POS { ATK_FRONT, ATK_SIDE, ATK_BACK, ATK_POS_NUM };
enum ATK_NUM_MAG { ATK_1MAG, ATK_2MAG, ATK_3MAG, ATK_NUM_MAG_NUM };
const THP ATK_POINTS[ATK_NUM_MAG_NUM][ATK_POS_NUM] = { {5,6,7},{12,14,16},{21,24,27} };
const TFrame ATK_CD_TIME = 0.5 * FREQ;


//��ͼ����
template<class T> class Point {
public:
    T x, y;

    Point() :x(0), y(0) {}
    Point(T _x, T _y) :x(_x), y(_y) {}

    double getDistance(Point _p) {
        return sqrt((double)((x - _p.x)*(x - _p.x) + (y - _p.y)*(y - _p.y)));
    }

};

typedef struct
{
    Point<TCoor> coor;
    THP hp;
    TAngle tank_angle, attack_angle;
    PLAYER_ID team;
} car_info;

typedef struct
{
    Point<TCoor> coor;
    TCoor radius;
} obs_info;

typedef struct
{
    PROP_TYPE tp;
    Point<TCoor> pos;
} prop_info;


//����λ��
const TCoor SPEED_BUFF_X = 0.5*MAP_WIDTH;
const TCoor SPEED_BUFF_Y = 0.25*MAP_HEIGHT;

const TCoor DEF_BUFF_X = 0.5*MAP_WIDTH;
const TCoor DEF_BUFF_Y = 0.75*MAP_HEIGHT;

const TCoor HP_PAK1_X = 20;
const TCoor HP_PAK1_Y = 20;

const TCoor MP_PAK1_X = 20;
const TCoor MP_PAK1_Y = MAP_HEIGHT - 20;

const TCoor HP_PAK2_X = MAP_WIDTH - 20;
const TCoor HP_PAK2_Y = MAP_HEIGHT- 20;

const TCoor MP_PAK2_X = MAP_WIDTH - 20;
const TCoor MP_PAK2_Y = 20;

