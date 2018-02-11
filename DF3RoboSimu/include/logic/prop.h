#include"common.h"
#include"car.h"

class Prop {
private:
    Prop_type tp;
    Point<double> pos;
    bool flag_available=false;
    int CD;
    int CD_count;
    int owner;

public:
    Prop(Prop_type t, int _x, int _y, int _owner);
    Point<double> get_pos() { return pos; }
    
    bool if_gotcha(car c);
    
    bool is_available() { return flag_available; }

    //道具被取走
    void betaken(car c);

    //重新生成
    void regenerate() {
        flag_available = true;
    }

    //每回合处理
    void round_operation(car c1,car c2);
};