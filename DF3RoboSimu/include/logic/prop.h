#pragma once

class Prop;

#include"logic/common.h"
#include"logic/car.h"

class Prop {
private:
    PROP_TYPE tp;
    Point<TCoor> pos;
    // Ϊ��һ��������ͬʱ������С����ȡ��ÿ�α�ʰȡʱ����flag_temp_available=false��ÿ�غϺ��ٸ���flag_available���
    bool flag_available=false, flag_temp_available = true;
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
    bool if_team(PLAYER_ID player_id) const {

        if (UNKNOWN_PLAYER == owner)
            return true;
        else
            return (player_id == owner);
    }
    // �ж�Car�Ƿ�񵽱�����
    bool if_gotcha(Point<TCoor> point, PLAYER_ID player_id) const;

    //��������
    void regenerate() {
        flag_available = true;
        CD_count = 0;
        exist_time = 0;
    }

    //ÿ�غϴ���
    void round_operation();

    //��ʰȡ
    void get_pickedup() {
        flag_temp_available = false;
        CD_count = 0;
        exist_time = 0;
    }
};
