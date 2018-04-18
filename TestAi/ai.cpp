
#include "ai.h"

#include <cmath>

#include <iostream>

using namespace std;

static double cos_d(double theta) { return cos(theta / 180 * 3.14159); }
static double sin_d(double theta) { return sin(theta / 180 * 3.14159); }
static double tan_d(double theta) { return tan(theta / 180 * 3.14159); }
static double atan2_d(double y, double x) { return (abs(x)<1e-4 && abs(y)<1e-4) ? 0.0 : 180 / 3.14159*atan2(y, x); }
static double atan2_d(Point<TCoor> target, Point<TCoor> cur) { return (cur.getDistance(target) < 1e-4) ? 0.0 : 180 / 3.14159*atan2(target.y - cur.y, target.x - cur.x); }
static double asin_d(double s) { return 180 / 3.14159*asin(s); }
static double minus_angle_d(double t1, double t2) {
    double u = fmod(t1 - t2 + 360, 360);
    if (u > 180) u -= 360;
    return u;
}

// ��������AI --- --- --- --- 

// ֻ��תȦ��AI
PlayerControl circle_ai(const Info info) {
    // circle run
    return PlayerControl{ 50, 30, 0, Attack1 };
}

// ֻ��ԭ�ش�ת��AI
PlayerControl dumb_ai(const Info info) {
    return PlayerControl{ 10, -10, 0, Attack1 };
}

// ר������������ѭ���Ļ�AI
PlayerControl bad_ai(const Info info) {
    // time out of limit test
    while (1);  
    return PlayerControl{ 0, 0, 0, NoAction };
}

// �ܵ�AI --- --- --- --- 

// ˫��ѡ�ֵĹؼ���
Point<TCoor> target_coors[2][4] = {
    {
        Point<TCoor>(20, 20),   //HP
        Point<TCoor>(20, 280),  //MP
        Point<TCoor>(200, 225), //DEF
        Point<TCoor>(200, 75),  //SPD
    },{
        Point<TCoor>(380, 280), //HP
        Point<TCoor>(380, 20),  //MP
        Point<TCoor>(200, 225), //DEF
        Point<TCoor>(200, 75)   //SPD
    } };
const int CNT_TARGET = 4;

// ����ǰ��ָ���ص��ָ��
PlayerControl rush_target(const Info info, Point<TCoor> target_coor) {
    // �Ѿ�����
    if (target_coor.getDistance(info.coor) < RADIUS_PROP) {
        return PlayerControl{ -10, 10, 0, NoAction };
    }

    TSpeed max_speed = 50;
    if (info.spd_status) max_speed = 75;
    else if (info.frz_status) max_speed = 25;

    TAngle target_angle = atan2_d(target_coor.y - info.coor.y, target_coor.x - info.coor.x);
    TAngle delta_angle = minus_angle_d(target_angle, info.car_angle);

    TAngle delta_attack_angle = minus_angle_d(target_angle, info.car_angle + info.attack_angle);

    if (abs(delta_angle) < 10) {
        // ֱ����
        return PlayerControl{ max_speed, max_speed, delta_attack_angle, NoAction };
    }
    else {

        if (delta_angle > 0) {
            // ��ת
            return PlayerControl{ -25, 25, delta_attack_angle, NoAction };
        }
        else {
            // ��ת
            return PlayerControl{ 25, -25, delta_attack_angle, NoAction };
        }
    }

}
// ��������ǰ��ָ���ص��ָ��
PlayerControl back_target(const Info info, Point<TCoor> target_coor) {
    // �Ѿ�����
    if (target_coor.getDistance(info.coor) < RADIUS_PROP) {
        return PlayerControl{ -10, 10, 0, NoAction };
    }

    TSpeed max_speed = 50;
    if (info.spd_status) max_speed = 75;
    else if (info.frz_status) max_speed = 25;

    TAngle target_angle = atan2_d(target_coor.y - info.coor.y, target_coor.x - info.coor.x);
    TAngle delta_angle = minus_angle_d(target_angle, 180 + info.car_angle);

    TAngle delta_attack_angle = 0; //= minus_angle_d(target_angle, info.car_angle + info.attack_angle);

    if (abs(delta_angle) < 10) {
        // ֱ����
        return PlayerControl{ -max_speed, -max_speed, delta_attack_angle, NoAction };
    }
    else {

        if (delta_angle > 0) {
            // ��ת
            return PlayerControl{ +25, -25, delta_attack_angle, NoAction };
        }
        else {
            // ��ת
            return PlayerControl{ -25, +25, delta_attack_angle, NoAction };
        }
    }

}

// ѭ���ܵ��AI
PlayerControl rush_ai(const Info info) {

    static int target_id = 0;

    Point<TCoor> target_coor = target_coors[info.id][target_id];
    if (target_coor.getDistance(info.coor) < RADIUS_PROP) {
        ++target_id; target_id %= CNT_TARGET;
        target_coor = target_coors[info.id][target_id];
    }

    return rush_target(info, target_coor);

}

// ����������˾ͻṥ��һ�����AI
PlayerControl attack_ai(const Info info) {
    static int target_id = 0;
    PlayerControl pc;

    // �ƶ����з���ȡ���ߣ��ܵ�
    Point<TCoor> target_coor = info.coor;
    if (info.cars.size() > 0 && info.mag > 0)
    {
        target_coor = info.cars[0].coor;
    }
    else
    {
        target_coor = target_coors[info.id][target_id];
        if (target_coor.getDistance(info.coor) < RADIUS_PROP) {
            // ��һ����
            ++target_id; target_id %= CNT_TARGET;
            target_coor = target_coors[info.id][target_id];
        }
        
    }
    pc = rush_target(info, target_coor);

    // ����������������
    if (info.mp >= BUFF_MP[BUFF_DEFEND] && info.can_shd && !info.shd_status)
        pc.action = Shield;
    else if (info.mag > 0)
        pc.action = Attack2;
    else if (info.mag == 0)
        pc.action = ChangeMag;

    return pc;
}

// ��һ���״̬����AI
PlayerControl state_machine_ai(const Info info) {
    // ״̬
    static enum States {
        S_Attack, // ׷��
        S_Cruise, // Ѳ��
        S_Defend  // ����
    } state = S_Cruise;
    // Ѳ��Ŀ���
    Point<TCoor> target_coors[2][4] = {
        {
            Point<TCoor>(20, 20),   //HP
            Point<TCoor>(20, 280),  //MP
            Point<TCoor>(200, 225), //DEF
            Point<TCoor>(200, 75),  //SPD
        },{
            Point<TCoor>(380, 280), //HP
            Point<TCoor>(380, 20),  //MP
            Point<TCoor>(200, 225), //DEF
            Point<TCoor>(200, 75)   //SPD
        } };
    const int CNT_TARGET = 4;


    // �������ľ���
    PlayerControl pc;
    // ����
    static const THP LOW_HP_THRESHOLD = 25; // ��Ѫ������
    static const int CANNOT_FIND_ENEMY_MAX = 60; // �Ҳ������˵�����ʱ��
    // ���˿��ܵ�λ��
    static Point<TCoor> enemy_pos(200, 150); 
    // �����Ҳ������˵�ʱ��
    static int cannot_find_enemy_cnt = 0;
    // Ѳ��ʱ��Ŀ���
    static int cruise_target_id = 0;


    switch (state)
    {
    case S_Attack: {
        // ׷��״̬���ҵ����˾�׷��ȥ���Ҳ�������תѰ�ң�
        // Ѫ�����ͽ���S_Defend���ȴ���תʱ���������S_Cruise��

        if (info.cars.size() > 0) { // �ҵ�����
            cannot_find_enemy_cnt = 0;
            enemy_pos = info.cars[0].coor;

            pc = rush_target(info, enemy_pos);
            pc.action = Attack1;

        }
        else if (info.coor.getDistance(enemy_pos) > 20) // ����̫Զ�����ܹ�ȥ����
        {
            pc = rush_target(info, enemy_pos);
            pc.action = NoAction;
        }
        else { // ׷����
            pc = PlayerControl{ 25, -25, 0, NoAction };
            ++cannot_find_enemy_cnt;
        }

        // ״̬ת��
        if (info.hp < LOW_HP_THRESHOLD) {
            state = S_Defend;
        }
        else if (cannot_find_enemy_cnt >= CANNOT_FIND_ENEMY_MAX) {

            state = S_Cruise;
        }
    }
        break;
    case S_Cruise: {
        // Ѳ��״̬���ҵ��ߡ�
        // Ѫ�����ͽ���S_Defend���������˽���S_Attack��

        Point<TCoor> target_coor = target_coors[info.id][3]; // Ĭ����SPD
        for (int i = 0; i < info.props.size(); ++i) {
            target_coor = info.props[i].pos;
            if (info.props[i].tp == MP_PAK) break;  // ��MP
        }

        pc = rush_target(info, target_coor);
        if (info.can_spd && !info.spd_status) pc.action = SpeedUp;  // ��SPD

        // ״̬ת��
        if (info.hp < LOW_HP_THRESHOLD) {
            state = S_Defend;
        }
        else if (info.cars.size() > 0) {
            enemy_pos = info.cars[0].coor;
            cannot_find_enemy_cnt = 0;
            state = S_Attack;
        }
    }
        break;
    case S_Defend: {
        // ����״̬���ҵ��ߡ�
        // Ѫ���ָ�����S_Cruise��

        Point<TCoor> target_coor = target_coors[info.id][0]; // Ĭ��ȥ�Լ�Ѫ�����ڵ�         
        for (int i = 0; i < info.props.size(); ++i) {
            target_coor = info.props[i].pos;
            if (info.props[i].tp == HP_PAK) break;  // Ѫ����
        }

        pc = rush_target(info, target_coor);
        if (info.can_shd && !info.shd_status) pc.action = Shield;
        
        // ״̬ת��
        if (info.hp > LOW_HP_THRESHOLD) {
            state = S_Cruise;
        }
    }
        break;
    }

    return pc;

}

// Main ---------- ---------- ---------- ---------- ----------

PlayerControl player_ai(const Info info) {
    return state_machine_ai(info);
    //return attack_ai(info);
    //return rush_ai(info);
    //return circle_ai(info);
    //return dumb_ai(info);
}
