
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

// 基本测试AI --- --- --- --- 

// 只会转圈的AI
PlayerControl circle_ai(const Info info) {
    // circle run
    return PlayerControl{ 50, 30, 0, Attack1 };
}

// 只会原地打转的AI
PlayerControl dumb_ai(const Info info) {
    return PlayerControl{ 10, -10, 0, Attack1 };
}

// 专门用来测试死循环的坏AI
PlayerControl bad_ai(const Info info) {
    // time out of limit test
    while (1);  
    return PlayerControl{ 0, 0, 0, NoAction };
}

// 跑点AI --- --- --- --- 

// 双方选手的关键点
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

// 给出前往指定地点的指令
PlayerControl rush_target(const Info info, Point<TCoor> target_coor) {
    // 已经到了
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
        // 直接跑
        return PlayerControl{ max_speed, max_speed, delta_attack_angle, NoAction };
    }
    else {

        if (delta_angle > 0) {
            // 左转
            return PlayerControl{ -25, 25, delta_attack_angle, NoAction };
        }
        else {
            // 右转
            return PlayerControl{ 25, -25, delta_attack_angle, NoAction };
        }
    }

}
// 给出后退前往指定地点的指令
PlayerControl back_target(const Info info, Point<TCoor> target_coor) {
    // 已经到了
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
        // 直接跑
        return PlayerControl{ -max_speed, -max_speed, delta_attack_angle, NoAction };
    }
    else {

        if (delta_angle > 0) {
            // 左转
            return PlayerControl{ +25, -25, delta_attack_angle, NoAction };
        }
        else {
            // 右转
            return PlayerControl{ -25, +25, delta_attack_angle, NoAction };
        }
    }

}

// 循环跑点的AI
PlayerControl rush_ai(const Info info) {

    static int target_id = 0;

    Point<TCoor> target_coor = target_coors[info.id][target_id];
    if (target_coor.getDistance(info.coor) < RADIUS_PROP) {
        ++target_id; target_id %= CNT_TARGET;
        target_coor = target_coors[info.id][target_id];
    }

    return rush_target(info, target_coor);

}

// 如果遇到敌人就会攻击一会儿的AI
PlayerControl attack_ai(const Info info) {
    static int target_id = 0;
    PlayerControl pc;

    // 移动：敌方，取道具，跑点
    Point<TCoor> target_coor = info.coor;
    if (info.cars.size() > 0 && info.mag > 0)
    {
        target_coor = info.cars[0].coor;
    }
    else
    {
        target_coor = target_coors[info.id][target_id];
        if (target_coor.getDistance(info.coor) < RADIUS_PROP) {
            // 下一个点
            ++target_id; target_id %= CNT_TARGET;
            target_coor = target_coors[info.id][target_id];
        }
        
    }
    pc = rush_target(info, target_coor);

    // 攻击，防御，换弹
    if (info.mp >= BUFF_MP[BUFF_DEFEND] && info.can_shd && !info.shd_status)
        pc.action = Shield;
    else if (info.mag > 0)
        pc.action = Attack2;
    else if (info.mag == 0)
        pc.action = ChangeMag;

    return pc;
}

// 带一点儿状态机的AI
PlayerControl state_machine_ai(const Info info) {
    // 状态
    static enum States {
        S_Attack, // 追击
        S_Cruise, // 巡航
        S_Defend  // 防守
    } state = S_Cruise;
    // 巡航目标点
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


    // 将作出的决策
    PlayerControl pc;
    // 常数
    static const THP LOW_HP_THRESHOLD = 25; // 低血量警告
    static const int CANNOT_FIND_ENEMY_MAX = 60; // 找不到敌人的容忍时间
    // 敌人可能的位置
    static Point<TCoor> enemy_pos(200, 150); 
    // 持续找不到敌人的时间
    static int cannot_find_enemy_cnt = 0;
    // 巡航时的目标点
    static int cruise_target_id = 0;


    switch (state)
    {
    case S_Attack: {
        // 追击状态。找到敌人就追上去打，找不到就自转寻找；
        // 血量过低进入S_Defend；等待自转时间过长进入S_Cruise；

        if (info.cars.size() > 0) { // 找到敌人
            cannot_find_enemy_cnt = 0;
            enemy_pos = info.cars[0].coor;

            pc = rush_target(info, enemy_pos);
            pc.action = Attack1;

        }
        else if (info.coor.getDistance(enemy_pos) > 20) // 距离太远就先跑过去看看
        {
            pc = rush_target(info, enemy_pos);
            pc.action = NoAction;
        }
        else { // 追丢了
            pc = PlayerControl{ 25, -25, 0, NoAction };
            ++cannot_find_enemy_cnt;
        }

        // 状态转移
        if (info.hp < LOW_HP_THRESHOLD) {
            state = S_Defend;
        }
        else if (cannot_find_enemy_cnt >= CANNOT_FIND_ENEMY_MAX) {

            state = S_Cruise;
        }
    }
        break;
    case S_Cruise: {
        // 巡航状态。找道具。
        // 血量过低进入S_Defend；看到敌人进入S_Attack；

        Point<TCoor> target_coor = target_coors[info.id][3]; // 默认抢SPD
        for (int i = 0; i < info.props.size(); ++i) {
            target_coor = info.props[i].pos;
            if (info.props[i].tp == MP_PAK) break;  // 抢MP
        }

        pc = rush_target(info, target_coor);
        if (info.can_spd && !info.spd_status) pc.action = SpeedUp;  // 上SPD

        // 状态转移
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
        // 防守状态。找道具。
        // 血量恢复进入S_Cruise；

        Point<TCoor> target_coor = target_coors[info.id][0]; // 默认去自己血包所在地         
        for (int i = 0; i < info.props.size(); ++i) {
            target_coor = info.props[i].pos;
            if (info.props[i].tp == HP_PAK) break;  // 血包！
        }

        pc = rush_target(info, target_coor);
        if (info.can_shd && !info.shd_status) pc.action = Shield;
        
        // 状态转移
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
