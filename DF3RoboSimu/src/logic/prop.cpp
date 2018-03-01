// µÀ¾ß
#include "../../include/logic/prop.h"


Prop::Prop(PROP_TYPE t, int _x, int _y, PLAYER_ID _owner)
    : tp(t), owner(_owner)
{
    pos.x = _x; pos.y = _y;
    CD = PROP_CD_TIME[tp];
}

bool Prop::if_gotcha(const Car& c) const
{   
    return 
        if_team(c) && 
        c.getCoor().getDistance(pos) < RADIUS_PROP;
}


void Prop::round_operation()
{
    if (!is_available) {
        CD_count++;
        if (CD_count > CD)
            regenerate();
    }
    else {
        exist_time++;
    }
}
