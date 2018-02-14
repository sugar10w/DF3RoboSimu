#include"common.h"
#include"Car.h"

class Prop {
private:
    PROP_TYPE tp;
    Point<TCoor> pos;
    bool flag_available=false;
    TFrame CD;
    TFrame CD_count;
    int owner;

public:
    Prop(PROP_TYPE t, int _x, int _y, int _owner);
    Point<double> get_pos() { return pos; }
    
    bool if_gotcha(const Car& c);
    
    bool is_available() const { return flag_available; }

    //重新生成
    void regenerate() {
        flag_available = true;
    }

    //每回合处理
    void round_operation();
};
