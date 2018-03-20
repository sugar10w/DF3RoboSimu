
#include "ai.h"

PlayerControl player_ai(const Info info) {

    PlayerControl pc;

    pc.action = Attack1;
    pc.left_speed = 31.4;
    pc.right_speed = 0;
    pc.steer_angle = -90;

    return pc;

}
