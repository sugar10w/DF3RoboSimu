// µÀ¾ß
#include "logic/prop.h"


Prop::Prop(PROP_TYPE t, int _x, int _y, PLAYER_ID _owner)
    : tp(t), owner(_owner)
{
    pos.x = _x; pos.y = _y;
    CD = PROP_CD_TIME[tp];
}

bool Prop::if_gotcha(Point<TCoor> point, PLAYER_ID player_id) const
{   
    return is_available() &&
        if_team(player_id) && 
        point.getDistance(pos) < RADIUS_PROP;
}


void Prop::round_operation()
{
    if (!flag_temp_available)
    {
        flag_available = false;
        flag_temp_available = true;
    }
    if (!is_available()) {
        CD_count++;
        if (CD_count > CD)
            regenerate();
    }
    else {
        exist_time++;
    }
}
