// Ð¡³µ
#include "../../include/logic/car.h"

void Car::slowedDown()
{
    if (spd_status == SPD_HIGH)
        spd_status = SPD_NORM;
    else if (spd_status == SPD_NORM)
        spd_status = SPD_LOW;
    lspd /= 2;
    rspd /= 2;
    sloweddown_time = getTime();
}

void Car::setPos(Point<double> _coor, double _car_angle)
{
    coor = _coor; 
    car_angle = fmod(_car_angle, 360.0);
}

bool Car::isOOR()
{
    return (coor.x<0 || coor.x>MAP_WIDTH - 1 || coor.y<0 || coor.y >MAP_HEIGHT - 1) ? true : false;
}

double Car::setLeftSpeed(double _lspd)
{
    double spd_limit = SPD_BASE * SPD_VAL[spd_status];
    if (_lspd < -spd_limit)
        lspd = -spd_limit;
    else if (_lspd > spd_limit)
        lspd = spd_limit;
    else
        lspd = _lspd;

    return lspd;
}

double Car::setRightSpeed(double _rspd)
{
    double spd_limit = SPD_BASE * SPD_VAL[spd_status];
    if (_rspd < -spd_limit)
        rspd = -spd_limit;
    else if (_rspd > spd_limit)
        rspd = spd_limit;
    else
        rspd = _rspd;

    return rspd;
}

double Car::rotateAttack(double _angle)
{
    if (_angle < -ROTATE_SPD)
        attack_angle -= ROTATE_SPD;
    else if (_angle > ROTATE_SPD)
        attack_angle += ROTATE_SPD;
    else
        attack_angle += _angle;

    return attack_angle;
}

void Car::changeMag()
{
    atk_status = ATK_MAG;
    change_mag_time = getTime();
}

bool Car::emitSlowdown()
{
    if (slowdown_status == SLOWDOWN_NORM && mp >= BUFF_MP[BUFF_SLOWDOWN])
    {
        map->slowdown(coor, car_angle);
        mp -= BUFF_MP[BUFF_SLOWDOWN];
        slowdown_status = SLOWDOWN_CD;
        slowdown_cd_time = getTime();
        return true;
    }
    else {
        return false;
    }
}

bool Car::speedUp()
{
    if ((spd_status == SPD_LOW || spd_status == SPD_NORM) && mp >= BUFF_MP[BUFF_SPEEDUP])
    {
        mp -= BUFF_MP[BUFF_SPEEDUP];
        spd_status = SPD_HIGH;
        spdup_cd_time = getTime();
        return true;
    }
    else {
        return false;
    }
}