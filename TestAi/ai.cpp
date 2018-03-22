
#include "ai.h"

#include <cmath>

#include <iostream>

using namespace std;

static double cos_d(double theta) { return cos(theta / 180 * 3.14159); }
static double sin_d(double theta) { return sin(theta / 180 * 3.14159); }
static double tan_d(double theta) { return tan(theta / 180 * 3.14159); }
static double atan2_d(double y, double x) { return 180 / 3.14159*atan2(y, x); }
static double asin_d(double s) { return 180 / 3.14159*asin(s); }


PlayerControl circle_ai(const Info info) {
    // circle run
    PlayerControl pc;
    pc.action = Attack1;
    pc.left_speed = 50;
    pc.right_speed = 30;
    pc.steer_angle = -90;
    return pc;
}

PlayerControl dumb_ai(const Info info) {
    PlayerControl pc;
    pc.action = Attack1;
    pc.left_speed = 50;
    pc.right_speed = -50;
    pc.steer_angle = 0;
    return pc;
}

PlayerControl bad_ai(const Info info) {
    // time out of limit test
    PlayerControl pc;
    while (1);  
    return pc;
}


Point<TCoor> target_coors[] = {
    Point<TCoor>(200, 75),
    Point<TCoor>(200, 150),
    Point<TCoor>(200, 20),
    Point<TCoor>(20, 20),
    Point<TCoor>(200, 20)
};
int target_id = 0;
const int CNT_TARGET = 5;
PlayerControl rush_ai(const Info info) {

    // 跑到 (200,75) 之后转圈

    PlayerControl pc;
    
    Point<TCoor> target_coor = target_coors[target_id];

    // 已经到了
    if (target_coor.getDistance(info.coor) < 30) {
        // 下一个点
        pc.action = NoAction;
        pc.left_speed = 0;
        pc.right_speed = 0;
        pc.steer_angle = 0;
        ++target_id; target_id %= CNT_TARGET;
        cout << target_id << endl;
        return pc;
    }

    TAngle target_angle = atan2_d(target_coor.y - info.coor.y, target_coor.x - info.coor.x);
    TAngle delta_angle = fmod(target_angle - info.car_angle + 360, 360);
    if (delta_angle > 180) delta_angle -= 360;


    if (abs(delta_angle) < 10) {
        // 直接跑
        pc.action = NoAction;
        pc.left_speed = 50;
        pc.right_speed = 50;
        pc.steer_angle = 0;
        return pc;
    }
    else {
        
        if (delta_angle > 0) {
            // 左转
            pc.action = NoAction;
            pc.left_speed = -20;
            pc.right_speed = 20;
            pc.steer_angle = 0;
            return pc;
        }
        else {
            // 右转
            pc.action = NoAction;
            pc.left_speed = 20;
            pc.right_speed = -20;
            pc.steer_angle = 0;
            return pc;
        }
    }

}


// Main ---------- ---------- ---------- ---------- ----------

PlayerControl player_ai(const Info info) {
    
    return rush_ai(info);
    //return circle_ai(info);
    //return dumb_ai(info);
}
