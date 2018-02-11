// 道具
#include "..\..\include\logic\prop.h"


Prop::Prop(Prop_type t, int _x, int _y,int _owner)
{
    pos.x = _x;
    pos.y = _y;
    tp = t;
    owner = _owner;

    switch (tp)
    {
    case SPD_BUFF:
        CD = BUFF_CD_TIME;
        break;
    case DEF_BUFF:
        CD = BUFF_CD_TIME;
        break;
    case HP_PAK:
        CD = PACKAGE_CD_TIME;
        break;
    case MP_PAK:
        CD = PACKAGE_CD_TIME;
        break;
    default:
        break;
    }
}

bool Prop::if_gotcha(car c)
{
    Point <double> p = c.getCoor();
    if (owner == 0 && owner == c.getSide())
        return (p.getDistance(pos) < RADIUS_PROP);
    else
        return false;
}

void Prop::betaken(car c)
{
    flag_available = false;
    CD_count = 0;
    //对车的处理
    switch (tp)
    {
    case SPD_BUFF:
        c.speedUp();
        break;
    case DEF_BUFF:
        c.protect();
        break;
    case HP_PAK:
        //c.HPrecover();
        break;
    case MP_PAK:
        //c.MPrecover();
        break;
    default:
        break;
    }
}

void Prop::round_operation(car c1, car c2)
{
    if (is_available) {
        bool got_by_car1 = if_gotcha(c1);
        bool got_by_car2 = if_gotcha(c2);
        if (got_by_car1&&got_by_car2) {
            if (got_by_car1)
                betaken(c1);
            if (got_by_car2)
                betaken(c2);
        }
    }
    else {
        CD_count++;
        if (CD_count > CD)
            regenerate();
    }
}
