#pragma once

#include"common.h"
#include"Car.h"

class Prop {
private:
    PROP_TYPE tp;
    Point<TCoor> pos;
    bool flag_available=false;
    TFrame CD;
    TFrame CD_count;
    PLAYER_ID owner;

public:
    // 构造函数
    Prop(PROP_TYPE t, int _x, int _y, PLAYER_ID _owner);
    // 道具坐标
    Point<double> get_pos() const { return pos; }
    // 道具类型
    PROP_TYPE get_type() const { return tp; }
    // 道具是否在场上
    bool is_available() const { return flag_available; }

    // 判断Car的队伍是否符合本道具
    bool if_team(const Car& c) const;
    // 判断Car是否捡到本道具
    bool if_gotcha(const Car& c) const;

    //重新生成
    void regenerate() {
        flag_available = true;
        // TODO CD_count
    }

    //每回合处理
    void round_operation();
};
