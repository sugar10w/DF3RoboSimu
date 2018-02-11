#include"common.h"
#include"car.h"

class Prop {
private:
    Prop_type tp;
    Point<double> pos;
    bool flag_available;
    int CD;
    int CD_count;
public:
    Prop(Prop_type t, double _x, double _y);
    Point<double> get_pos() { return pos; }
    
    bool if_gotcha(Point<double> p) {
        return (p.getDistance(pos) < RADIUS_PROP);
    } 
    
    bool is_available() { return flag_available; }

    //���߱�ȡ��
    void betaken(car c);

    //��������
    void regenerate() {
        flag_available = true;
    }

    //ÿ�غϴ���
    void round_operation(car c1,car c2);
};