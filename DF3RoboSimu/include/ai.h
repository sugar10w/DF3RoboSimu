
#ifndef _DF2ROBOSIMU_AI_H__
#define _DF2ROBOSIMU_AI_H__


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

// ��һ�ȡ����Ϣ
struct Info {
    //TODO
};

#ifdef _MSC_VER

extern "C" _declspec(dllexport) PlayerControl player_ai(const Info info);

#endif


#ifdef __GNUC__

extern "C" PlayerControl player_ai(const Info info);

#endif


#endif
