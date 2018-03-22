
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


PlayerControl rush_ai(const Info info) {

    // �ܵ� (200,75) ֮��תȦ

    PlayerControl pc;
    Point<TCoor> target_coor(200, 75);

    // �Ѿ�����
    if (target_coor.getDistance(info.coor) < 5) {
        // תȦ
        pc.action = NoAction;
        pc.left_speed = 50;
        pc.right_speed = -50;
        return pc;
    }

    TAngle target_angle = atan2_d(target_coor.y - info.coor.y, target_coor.x - info.coor.x);
    TAngle delta_angle = fmod(target_angle - info.car_angle + 360, 360);
    if (delta_angle > 180) delta_angle -= 360;


    if (abs(delta_angle) < 10) {
        // ֱ����
        pc.action = NoAction;
        pc.left_speed = 50;
        pc.right_speed = 50;
        return pc;
    }
    else {
        
        if (delta_angle > 0) {
            // ��ת
            pc.action = NoAction;
            pc.left_speed = -25;
            pc.right_speed = 25;
            return pc;
        }
        else {
            // ��ת
            pc.action = NoAction;
            pc.left_speed = 25;
            pc.right_speed = -25;
            return pc;
        }
    }

}


// Main ---------- ---------- ---------- ---------- ----------

PlayerControl player_ai(const Info info) {
    
    //return rush_ai(info);
    //return circle_ai(info);
    return dumb_ai(info);
}
