// 道具
#include "..\..\include\logic\prop.h"


Prop::Prop(Prop_type t, double _x, double _y)
{
    pos.x = _x;
    pos.y = _y;
    tp = t;
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
        bool got_by_car1 = if_gotcha(c1.getCoor());//todo: point类型问题
        bool got_by_car2 = if_gotcha(c2.getCoor());
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
}
