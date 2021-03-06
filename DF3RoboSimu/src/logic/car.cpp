// 小车
#include "logic/car.h"

static double minus_angle_d(double t1, double t2) {
    double u = fmod(t1 - t2 + 360, 360);
    if (u > 180) u -= 360;
    return u;
}

TFrame Car::getTime() const {
    return game ? game->getTime() : -1; 
}

void Car::slowedDown()
{
    spd_status = SPD_LOW;
    lspd /= 2;
    rspd /= 2;
    sloweddown_time = getTime();
}

void Car::setPos(Point<TCoor> _coor, TAngle _car_angle)
{
    coor = _coor; 
    car_angle = fmod(_car_angle, 360.0);
}

void Car::setPos(Point<TCoor> _coor)
{
    coor = _coor;
}

bool Car::isOOR()
{
    return coor.x < 0 || coor.x > MAP_WIDTH || coor.y < 0 || coor.y > MAP_HEIGHT;
}

TSpeed Car::setLeftSpeed(TSpeed _lspd)
{
    double spd_limit = SPD_BASE * SPD_VAL[spd_status];
    if (isnan(_lspd)) lspd = 0;
    else if (_lspd < -spd_limit) lspd = -spd_limit;
    else if (_lspd > spd_limit) lspd = spd_limit;
    else lspd = _lspd;

    return lspd;
}

TSpeed Car::setRightSpeed(TSpeed _rspd)
{
    double spd_limit = SPD_BASE * SPD_VAL[spd_status];
    if (isnan(_rspd)) rspd = 0;
    else if (_rspd < -spd_limit) rspd = -spd_limit;
    else if (_rspd > spd_limit) rspd = spd_limit;
    else rspd = _rspd;

    return rspd;
}

TAngle Car::rotateAttack(TAngle _target_attack_angle)
{
    if (isnan(_target_attack_angle)) _target_attack_angle = 0;

    TAngle _angle = minus_angle_d(_target_attack_angle, attack_angle);

    //旋转之
    if (_angle < -ROTATE_SPD)
        attack_angle -= ROTATE_SPD;
    else if (_angle > ROTATE_SPD)
        attack_angle += ROTATE_SPD;
    else
        attack_angle += _angle;

    attack_angle = fmod(attack_angle + 360, 360.0);
    
    return attack_angle;
}

bool Car::changeMag()
{
    if (changemag_cd_status == BUFF_NORM && mag < MAG_MAX)
    {
        mag = 0;
        changemag_cd_status = BUFF_CD;
        changemag_cd_time = getTime();
        buffRecord = PIInstruction_changemag;
        return true;
    }
    else {
        return false;
    }
    
}

bool Car::emitSlowdown()
{
    if (slowdown_cd_status == BUFF_NORM
        && mp >= BUFF_MP[BUFF_SLOWDOWN])
    {
        game->slowdown(getTeam()); 
        mp -= BUFF_MP[BUFF_SLOWDOWN];
        slowdown_cd_status = BUFF_CD;
        slowdown_cd_time = getTime();        
        buffRecord = PIInstruction_slowdown;
        return true;
    }
    else {
        return false;
    }
}

bool Car::speedUp()
{
    if ((spd_status == SPD_LOW || spd_status == SPD_NORM) && mp >= BUFF_MP[BUFF_SPEEDUP]
        && spdup_cd_status == BUFF_NORM)
    {
        mp -= BUFF_MP[BUFF_SPEEDUP];
        spd_status = SPD_HIGH;
        spdup_cd_time = getTime();
        spdup_valid_time = getTime();
        spdup_cd_status = BUFF_CD;
        buffRecord = PIInstruction_speedup;
        return true;
    }
    else {
        return false;
    }
}

bool Car::protect()
{
    if (def_status == DEF_NORM  && mp >= BUFF_MP[BUFF_DEFEND] && def_cd_status == BUFF_NORM)
    {
        mp -= BUFF_MP[BUFF_DEFEND];
        def_status = DEF_ARM;
        def_cd_time = getTime();
        def_valid_time = getTime();
        def_cd_status = BUFF_CD;
        buffRecord = PIInstruction_defend;
        return true;
    }
    else {
        return false;
    }
}

bool Car::attack(ATK_NUM_MAG num)
{
    if (atk_cd_status == BUFF_NORM && changemag_cd_status == BUFF_NORM && mag >= ATK_MAG_DEMAND[num])
    {
        game->attack(getTeam(), num);
        mag -= ATK_MAG_DEMAND[num];
        atk_cd_status = BUFF_CD;
        atk_cd_time = getTime();
        buffRecord = PIInstruction_attack;
        return true;
    }
    else {
        if (mag < ATK_MAG_DEMAND[num]) {
            return changeMag();
        }
        return false;
    }
}


PlayerInfo Car::frameRoutine()
{
    // 1.更新小车状态
    statusUpdate();

    // 2.移动到下一位置
    setPos(
        map->getNextPos(this, game->getCar((PLAYER_ID)(1 - team))),
        map->getNextAngle(this)
    );

    // 3.返回小车回放文件结构体
    return getPlayerInfo();
}

void Car::statusUpdate()
{
    int cur_time = game->getTime();
    buffRecord = PIInstruction_NULL;

    // defend cd status
    if (cur_time - def_cd_time == BUFF_CD_TIME[BUFF_DEFEND]) {
        def_cd_status = BUFF_NORM;
    }
    // defend status
    if (cur_time - def_valid_time == BUFF_VALID_TIME) {
        def_status = DEF_NORM;
    }

    // speed up cd status
    if (cur_time - spdup_cd_time == BUFF_CD_TIME[BUFF_SPEEDUP]) {
        spdup_cd_status = BUFF_NORM;
    }
    // speed up status, if not slowed after speedup, then back to SPD_NORM
    if (cur_time - spdup_valid_time == BUFF_VALID_TIME && spd_status == SPD_HIGH) {
        spd_status = SPD_NORM;
    }
    // slowed down status, if not speeded up after slowed down, then back to SPD_NORM
    if (cur_time - sloweddown_time == BUFF_VALID_TIME && spd_status == SPD_LOW) {
        spd_status = SPD_NORM;
    }

    // slow down cd status
    if (cur_time - slowdown_cd_time == BUFF_CD_TIME[BUFF_SLOWDOWN]) {
        spdup_cd_status = BUFF_NORM;
    }

    // attack cd status
    if (cur_time - atk_cd_time == ATK_CD_TIME) {
        atk_cd_status = BUFF_NORM;
    }

    // change magazine cd status
    if (cur_time - changemag_cd_time == MAG_CHANGETIME) {
        mag = MAG_MAX;
        changemag_cd_status = BUFF_NORM;
    }

    // auto add MP
    mp += MP_GAIN;
    if (mp < 0) mp = 0;
    if (mp > MP_MAX) mp = MP_MAX;
}

void Car::setMP(TMP _mp)
{
    if (_mp < 0)
        mp = 0;
    else if (_mp > MP_MAX)
        mp = MP_MAX;
    else
        mp = _mp;
}

void Car::setHP(THP _hp)
{
    if (_hp < 0)
        hp = 0;
    else if (_hp > HP_MAX)
        hp = HP_MAX;
    else
        hp = _hp;
}

PlayerInfo Car::getPlayerInfo()
{
    int curtime = game->getTime();
    PlayerInfo info;
    info.HP = (int)hp + 0.99;
    info.MP = mp;
    info.MAG = mag;
    info.x = (float)coor.x;
    info.y = (float)coor.y;
    info.angle = (float)car_angle;
    info.sightAngle = (float)attack_angle;
    info.speedState = (int)spd_status + 1;
    info.defenceState = (int)def_status + 1;
    info.instruction = buffRecord;

    if (changemag_cd_status == BUFF_CD)
        info.attackState = PIAttackState_changemag;
    else if (atk_cd_status == BUFF_CD)
        info.attackState = PIAttackState_CD;
    else
        info.attackState = PIAttackState_attack;

    if (slowdown_cd_status == BUFF_CD)
        info.instructionCD[PIInstruction_slowdown - 1] = BUFF_CD_TIME[BUFF_SLOWDOWN] - (curtime - slowdown_cd_time);
    else
        info.instructionCD[PIInstruction_slowdown - 1] = 0;

    if (atk_cd_status == BUFF_CD)
        info.instructionCD[PIInstruction_attack - 1] = ATK_CD_TIME - (curtime - atk_cd_time);
    else
        info.instructionCD[PIInstruction_attack - 1] = 0;

    if (spdup_cd_status == BUFF_CD)
        info.instructionCD[PIInstruction_speedup - 1] = BUFF_CD_TIME[BUFF_SPEEDUP] - (curtime - spdup_cd_time);
    else
        info.instructionCD[PIInstruction_speedup - 1] = 0;

    if (def_cd_status == BUFF_CD)
        info.instructionCD[PIInstruction_defend - 1] = BUFF_CD_TIME[BUFF_DEFEND] - (curtime - def_cd_time);
    else
        info.instructionCD[PIInstruction_defend - 1] = 0;

    return info;
}

void Car::getAttacked(ATK_NUM_MAG num_mag, ATK_POS atk_pos)
{
    // TODO 损血，判定存活等
    double atkratio = DEF_VAL[def_status];
    setHP(getHP() - atkratio * ATK_POINTS[num_mag][atk_pos]);
    // 不用判断存活，在game.cpp中frameRoutine最后会判断。

}


void Car::propSpeedUP()
{
    spd_status = SPD_HIGH;
    spdup_valid_time = getTime();
}

void Car::propProtect()
{
    def_status = DEF_ARM;
    def_valid_time = getTime();
}
