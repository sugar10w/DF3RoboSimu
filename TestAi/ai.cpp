
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


PlayerControl circle_ai(const Info info) {
    // circle run
    return PlayerControl{ 50, 30, 0, Attack1 };
}

PlayerControl dumb_ai(const Info info) {
    return PlayerControl{ 10, -10, 0, Attack1 };
}

PlayerControl bad_ai(const Info info) {
    // time out of limit test
    while (1);  
    return PlayerControl{ 0, 0, 0, NoAction };
}


Point<TCoor> target_coors[] = {
    Point<TCoor>(200, 75),
    Point<TCoor>(200, 150),
    Point<TCoor>(200, 20),
    Point<TCoor>(200, 200),
    Point<TCoor>(20, 20)
};
int target_id = 0;
const int CNT_TARGET = 5;
PlayerControl rush_ai(const Info info) {

    // 跑到 (200,75) 之后转圈
    
    Point<TCoor> target_coor = target_coors[target_id];

    // 已经到了
    if (target_coor.getDistance(info.coor) < 30) {
        // 下一个点
        ++target_id; target_id %= CNT_TARGET;
        cout << target_id << endl;
        return PlayerControl{ 0, 0, 0, NoAction };
    }

    TAngle target_angle = atan2_d(target_coor.y - info.coor.y, target_coor.x - info.coor.x);
    TAngle delta_angle = fmod(target_angle - info.car_angle + 360, 360);
    if (delta_angle > 180) delta_angle -= 360;


    if (abs(delta_angle) < 10) {
        // 直接跑
        return PlayerControl{ 50, 50, 0, NoAction };
    }
    else {
        
        if (delta_angle > 0) {
            // 左转
            return PlayerControl{ -20, 20, 0, NoAction};
        }
        else {
            // 右转
            return PlayerControl{ 20, -20, 0, NoAction };
        }
    }

}


PlayerControl rush_target(const Info info, Point<TCoor> coor) {
    // 已经到了
    if (coor.getDistance(info.coor) < 30) {
        return PlayerControl{ 0, 0, 0, NoAction };
    }

    TAngle target_angle = atan2_d(coor.y - info.coor.y, coor.x - info.coor.x);
    TAngle delta_angle = fmod(target_angle - info.car_angle + 360, 360);
    if (delta_angle > 180) delta_angle -= 360;

    float delta_attack_angle = fmod(target_angle - info.car_angle - info.attack_angle + 360, 360);
    if (delta_attack_angle > 180) delta_attack_angle -= 360;

    if (abs(delta_angle) < 10) {
        // 直接跑
        return PlayerControl{ 50, 50, delta_attack_angle, NoAction };
    }
    else {

        if (delta_angle > 0) {
            // 左转
            return PlayerControl{ -20, 20, delta_attack_angle, NoAction };
        }
        else {
            // 右转
            return PlayerControl{ 20, -20, delta_attack_angle, NoAction };
        }
    }

}

PlayerControl attack_ai(const Info info) {
    static int target_index = 0;
    PlayerControl pc;

    // 移动：敌方，取道具，跑点
    Point<TCoor> target_coor = info.coor;
    if (info.cars.size() > 0 && info.mag > 0)
    {
        target_coor = info.cars[0].coor;
    }
    else if (info.props.size() > 0)
    {
        target_coor = info.props[0].pos;
    }
    else
    {
        ++target_id; target_id %= CNT_TARGET;
        cout << target_id << endl;
        target_coor = target_coors[target_id];
    }
    pc = rush_target(info, target_coor);

    // 攻击，防御，换弹
    if (info.cars.size() > 0)
    {
        TAngle target_angle = atan2_d(info.cars[0].coor, info.coor) + 360.0 - info.attack_angle - info.car_angle;

        if (info.mag > 0 && abs(target_angle) < 3.0 / 180.0*3.1416)
            pc.action = Attack1;
        else if (info.mp >= BUFF_MP[BUFF_DEFEND])
            pc.action = Shield;
        else if (info.mag == 0)
            pc.action = ChangeMag;
    }
    else {
        if (info.mag < MAG_MAX)
            pc.action = ChangeMag;
    }

    return pc;
}

// Main ---------- ---------- ---------- ---------- ----------

PlayerControl player_ai(const Info info) {
    return attack_ai(info);
    //return rush_ai(info);
    //return circle_ai(info);
    //return dumb_ai(info);
}
