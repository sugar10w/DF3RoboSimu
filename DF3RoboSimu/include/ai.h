
#ifndef _DF2ROBOSIMU_AI_H__
#define _DF2ROBOSIMU_AI_H__

#include <vector>

#include "logic/common.h"

// 玩家获取的信息
struct Info {

    // 己方全套信息

    // ID
    int id;

    // 坐标
    Point<double> coor;
    // 车身朝向（deg）
    double car_angle;
    // 双目/枪口相对于车身的朝向（deg）
    double attack_angle;

    // 血量
    int hp;
    // 魔法量
    int mp;
    // 弹药数量
    int mag;
    
    // 己方是否处在 加速 / 减速 / 防御状态 
    bool spd_status, frz_status, shd_status;
    // 各项技能是否已经完成冷却，可以使用
    bool can_atk, can_spd, can_frz, can_shd;

    // 视野内目标
    std::vector<car_info> cars;
    std::vector<obs_info> obs;
    std::vector<prop_info> props;

};

// 玩家的特殊动作
enum PlayerAction {
    // 不行动
    NoAction,
    // 不同规格的攻击（威力和耗费弹药数不同）
    Attack1, Attack2, Attack3,
    // 填充弹夹
    ChangeMag,
    // 延缓射线
    FrozenRays,
    // 加速
    SpeedUp,
    // 能量护罩
    Shield
};

// 玩家控制
struct PlayerControl {
    // 左右轮转速
    float left_speed, right_speed;
    // 舵机目标角度
    float steer_angle;
    // 主动技能
    PlayerAction action;
};


#ifdef _MSC_VER

extern "C" _declspec(dllexport) PlayerControl player_ai(const Info info);

#endif


#ifdef __GNUC__

extern "C" PlayerControl player_ai(const Info info);

#endif


#endif
