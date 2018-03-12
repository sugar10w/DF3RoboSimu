#pragma once

class Prop;

#include"logic/common.h"
#include"logic/car.h"

class Prop {
private:
    PROP_TYPE tp;
    Point<TCoor> pos;
    bool flag_available=false;
    TFrame CD;
    TFrame CD_count = 0;
    TFrame exist_time = 0;
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

    TFrame getCD_count() { return CD_count; }
    TFrame getCD() { return CD; }
    TFrame get_ET() { return exist_time; }

    // 判断Car的队伍是否符合本道具
    bool if_team(PLAYER_ID player_id) const {

        if (UNKNOWN_PLAYER == owner)
            return true;
        else
            return (player_id == owner);
    }
    // 判断Car是否捡到本道具
    bool if_gotcha(Point<TCoor> point, PLAYER_ID player_id) const;

    //重新生成
    void regenerate() {
        flag_available = true;
        CD_count = 0;
        exist_time = 0;
    }

    //每回合处理
    void round_operation();
};
