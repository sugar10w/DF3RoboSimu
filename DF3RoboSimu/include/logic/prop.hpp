#include"common.h"
#include"car.h"

class Prop {
private:
    Prop_type tp;
    Point pos;
    bool flag_available;
    int CD;
    int CD_count;
public:
    Prop(Prop_type t, double _x, double _y) {
        pos.x = _x;
        pos.y = _y;
        tp = t;
        switch (tp)
        {
        case SPD_BUFF:
            CD = BUFF_CD;
            break;
        case DEF_BUFF:
            CD = BUFF_CD;
            break;
        case HP_PAK:
            CD = PACKAGE_CD;
            break;
        case MP_PAK:
            CD = PACKAGE_CD;
            break;
        default:
            break;
        }
    }
    Point get_pos() { return pos; }
    
    bool if_gotcha(Point p) {
        return (p.getDistance(pos) < RADIUS_PROP);
    } 
    
    bool is_available() { return flag_available; }

    //道具被取走
    void betaken(car c) {
        flag_available = false;
        CD_count = 0;
        //对车的处理
        switch (tp)
        {
        case SPD_BUFF:
            break;
        case DEF_BUFF:
            break;
        case HP_PAK:
            break;
        case MP_PAK:
            break;
        default:
            break;
        }
    }

    //重新生成
    void regenerate() {
        flag_available = true;
    }
    //每回合处理
    void round_operation(car c1,car c2) {
        if (is_available) {
            if (if_gotcha(c1.pos) && if_gotcha(c2.pos)) {
                if (if_gotcha(c1.pos))
                    betaken(c1);
                if (if_gotcha(c2.pos))
                    betaken(c2);
            }
        }
        else
        {
            CD_count++;
            if (CD_count > CD)
                regenerate();
        }
    }
};