// ����
#include "..\..\include\logic\prop.h"


Prop::Prop(PROP_TYPE t, int _x, int _y,int _owner)
{
    pos.x = _x;
    pos.y = _y;
    tp = t;
    owner = _owner;

    CD = PROP_CD_TIME[tp];
    
}

bool Prop::if_gotcha(const Car& c)
{   
    // TODO  1 ������ 2 HP/MP�������˼��
    return false;
}


void Prop::round_operation()
{
    if (!is_available) {
        CD_count++;
        if (CD_count > CD)
            regenerate();
    }
}
