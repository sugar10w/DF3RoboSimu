
#ifndef _DF2ROBOSIMU_AI_H__
#define _DF2ROBOSIMU_AI_H__


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

// 玩家获取的信息
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
