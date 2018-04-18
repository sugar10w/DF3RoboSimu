
#ifndef _DF2ROBOSIMU_AI_H__
#define _DF2ROBOSIMU_AI_H__

#include <vector>

#include "logic/common.h"

// ��һ�ȡ����Ϣ
struct Info {

    // ����ȫ����Ϣ

    // ID
    int id;

    // ����
    Point<double> coor;
    // ������deg��
    double car_angle;
    // ˫Ŀ/ǹ������ڳ���ĳ���deg��
    double attack_angle;

    // Ѫ��
    int hp;
    // ħ����
    int mp;
    // ��ҩ����
    int mag;
    
    // �����Ƿ��� ���� / ���� / ����״̬ 
    bool spd_status, frz_status, shd_status;
    // ������Ƿ��Ѿ������ȴ������ʹ��
    bool can_atk, can_spd, can_frz, can_shd;

    // ��Ұ��Ŀ��
    std::vector<car_info> cars;
    std::vector<obs_info> obs;
    std::vector<prop_info> props;

};

// ��ҵ����⶯��
enum PlayerAction {
    // ���ж�
    NoAction,
    // ��ͬ���Ĺ����������ͺķѵ�ҩ����ͬ��
    Attack1, Attack2, Attack3,
    // ��䵯��
    ChangeMag,
    // �ӻ�����
    FrozenRays,
    // ����
    SpeedUp,
    // ��������
    Shield
};

// ��ҿ���
struct PlayerControl {
    // ������ת��
    float left_speed, right_speed;
    // ���Ŀ��Ƕ�
    float steer_angle;
    // ��������
    PlayerAction action;
};


#ifdef _MSC_VER

extern "C" _declspec(dllexport) PlayerControl player_ai(const Info info);

#endif


#ifdef __GNUC__

extern "C" PlayerControl player_ai(const Info info);

#endif


#endif
