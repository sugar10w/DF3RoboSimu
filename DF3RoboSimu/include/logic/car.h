// Ð¡³µ
#pragma once
#include "common.h"

class car {
public:
    car() {}
    car(Point<int> _coor, double _tank_angle,int _side) :tank_angle(_tank_angle) ,side(_side){ coor.x = (double)_coor.x; coor.y = (double)_coor.y; }
    Point<double> getCoor() { return Point<double>((int)coor.x, (int)coor.y); }
    double getHP() { return hp; }
    int getMP() { return mp; }
    int getMAG() { return mag; }
    double getTankAngle() { return tank_angle; }
    double getAttackAngle() { return attack_angle; }
    int isOOR();

    int setLeftSpeed(double _lspd);
    int setRightSpeed(double _rspd);
    int rotateAttack(double _angle);

    int emitSlowdown();
    int speedUp();
    int protect();
    int attack(ATK_NUM_MAG num);
    void getView();

    int getSide() { return side; }

protected:
    double attacked(ATK_NUM_MAG atk_num_mag, ATK_POS atk_pos);
    void slowedDown();
    void setPos(Point<double> _coor, double _tank_angle, double _attack_angle);

private:
    double hp = 100.0, lspd = 0.0, rspd = 0.0, tank_angle = 0.0, attack_angle = 0.0;
    int mp = 100, mag = 12;
    int side;
    Point<double> coor = Point<double>(0.0, 0.0);
    SPD_STATUS spd_status = SPD_NORM;
    DEF_STATUS def_status = DEF_NORM;
    ATK_STATUS atk_status = ATK_NORM;
};