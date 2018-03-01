#pragma once

#include"common.h"
#include"Car.h"

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
    // ���캯��
    Prop(PROP_TYPE t, int _x, int _y, PLAYER_ID _owner);
    // ��������
    Point<double> get_pos() const { return pos; }
    // ��������
    PROP_TYPE get_type() const { return tp; }
    // �����Ƿ��ڳ���
    bool is_available() const { return flag_available; }

    TFrame getCD_count() { return CD_count; }
    TFrame getCD() { return CD; }
    TFrame get_ET() { return exist_time; }

    // �ж�Car�Ķ����Ƿ���ϱ�����
    bool if_team(const Car& c) const {

        if (UNKNOWN_PLAYER == owner)
            return true;
        else
            return (owner == c.getTeam());
    }
    // �ж�Car�Ƿ�񵽱�����
    bool if_gotcha(const Car& c) const;

    //��������
    void regenerate() {
        flag_available = true;
        CD_count = 0;
        exist_time = 0;
    }

    //ÿ�غϴ���
    void round_operation();
};
