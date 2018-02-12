// 小车
#pragma once
#include "common.h"
#include "prop.h"
#include "map.h"
#include <vector>

///    \brief  小车仿真类  
///  
///    每个小车的位置由坐标coor、前进方向car_angle、相对于前进方向的射击角度attack_angle确定。
///    包含血量hp、技能mp、弹夹数mag，通过左右轮速度lspd、rspd控制小车运动，通过旋转射击角度进行瞄准。
///
class Car {
public:
    friend class Map;
    Point<int> getCoor() { return Point<int>((int)coor.x, (int)coor.y); }
    double getHP() { return hp; }
    int getMP() { return mp; }
    int getMAG() { return mag; }
    int getTeam() { return team; }
    double getCarAngle() { return car_angle; }
    double getAttackAngle() { return attack_angle; }
    double getLeftSpeed() { return lspd; }
    double getRightSpeed() { return rspd; }
    SPD_STATUS getSpdStatus() { return spd_status; }
    DEF_STATUS getDefStatus() { return def_status; }
    ATK_STATUS getAtkStatus() { return atk_status; }
    SLOWDOWN_STATUS getSlowDownStatus() { return sd_status; }

    /// 判断是否在界外
    ///    
    ///     @return    是否在界外
    bool isOOR() { return (coor.x<0 || coor.x>MAP_WIDTH - 1 || coor.y<0 || coor.y >MAP_HEIGHT - 1) ? true : false; }

    /// 设置左轮速度
    ///    
    ///     轮速正为前进，负为后退，0为不动
    ///     若_lspd<-spd_status*SPD_BASE，则设置为-spd_status*SPD_BASE，若_lspd>spd_status*SPD_BASE，
    ///     则设置为spd_status*SPD_BASE，否则设置为_lspd。
    ///     @param _lspd 设置的左轮速度
    ///     @return    设置后的左轮速度
    double setLeftSpeed(double _lspd);

    /// 设置右轮速度
    ///    
    ///     轮速正为前进，负为后退，0为不动
    ///     若_rspd<-spd_status*SPD_BASE，则设置为-spd_status*SPD_BASE，若_rspd>spd_status*SPD_BASE，
    ///     则设置为spd_status*SPD_BASE，否则设置为_rspd。
    ///     @param _rspd 设置的右轮速度
    ///     @return    设置后的右轮速度
    double setRightSpeed(double _rspd);

    /// 旋转瞄准方向
    ///    
    ///     角度正为逆时针，负为顺时针，0为不动，该角度为相对前进方向的逆时针角度
    ///     若_angle<-ROTATE_SPD，则设置为-ROTATE_SPD，若_angle>ROTATE_SPD，
    ///     则设置为ROTATE_SPD，否则设置为_angle。
    ///     @param _angle 瞄准逆时针旋转角度
    ///     @return    旋转后后的瞄准方向相对前进方向的逆时针角度
    double rotateAttack(double _angle);

    /// 换弹夹
    ///    
    ///     更换弹夹，进入ATK_MAG状态，无法进行攻击，耗时MAG_CHANGETIME，之后进入ATK_NORM状态
    void changeMag();

    /// 发射延缓射线
    ///    
    ///     在mp>=BUFF_MP[BUFF_SLOWDOWN]且非技能冷却情况下，沿瞄准方向发射延缓射线。
    ///     @return    是否成功释放技能
    bool emitSlowdown();

    /// 加速
    ///    
    ///     在mp>=BUFF_MP[BUFF_SPEEDUP]且非技能冷却情况下释放，若原本为减速则恢复为常速，若原本为常速变为加速。
    ///     @return    是否成功释放技能
    bool speedUp();

    /// 释放能量护罩
    ///    
    ///     在mp>=BUFF_MP[BUFF_PROTECT]且非技能冷却情况下释放，受伤值减半。
    ///     @return    是否成功释放技能
    bool protect();

    /// 攻击
    ///    
    ///     在mag>=num且非技能冷却非换弹夹情况下沿瞄准方向攻击，否则不攻击。
    ///     @param num 子弹发射数量
    ///     @return    是否成功释放技能
    bool attack(ATK_NUM_MAG num);

    /// 获得视野内目标情况
    ///    
    ///     获得视野内小车、障碍物、道具的相关信息
    ///     @param cars 小车信息数组，引用传递
    ///     @param obs 障碍物信息数组，引用传递
    ///     @param props 道具信息数组，引用传递
    ///     @note    3个vector内容会先被清空。
    void getView(std::vector<car_info>& cars, std::vector<obs_info>& obs, std::vector<prop_info>& props);

protected:
    /// 小车构造函数
    ///    
    ///     设定初始位置、车头方向、所属队伍，该Car类只能由Map类创造实例。
    ///     @param _map 小车所处地图指针
    ///     @param _coor 初始坐标
    ///     @param _car_angle 初始车头朝向
    ///     @param _team 所属队伍
    Car(Map* _map, Point<int> _coor = Point<int>(0, 0), double _car_angle = 0.0, int _team = 0)
        :car_angle(_car_angle), team(_team), map(_map) { coor.x = (double)_coor.x; coor.y = (double)_coor.y; }

    /// 受到延缓射线攻击
    ///    
    ///     若原状态为常速则变为减速，若原状态为加速则变为常速。为防止小车运动方向突变，两个轮子速度整体减半，后续选手可自行调整。
    void slowDown();

    /// 设置小车位置
    ///    
    ///     由Map设定坐标和车头方向。
    ///     @param _coor 小车坐标
    ///     @param _tank_angle 车头方向
    void setPos(Point<double> _coor, double _car_angle) { coor = _coor; if (_car_angle >= 0 && _car_angle < 360) car_angle = _car_angle; }

    /// 设置小车技能值
    ///    
    ///     设置小车技能值
    ///     @param _mp 新技能值
    void setMP(int _mp) { if (_mp>0 && _mp<= MP_MAX) mp = _mp; }

    /// 设置小车生命值
    ///    
    ///     设置小车生命值
    ///     @param _hp 新生命值
    void setHP(int _hp) { if (_hp>0 && _hp<=HP_MAX) hp = _hp; }

private:
    double hp = HP_MAX,  /// 血量，范围[0, HP_MAX]
        lspd = 0.0,   /// 左轮速度，范围[-spd_status*SPD_BASE, spd_status*SPD_BASE]
        rspd = 0.0,   /// 右轮速度，范围[-spd_status*SPD_BASE, spd_status*SPD_BASE]
        car_angle = 0.0,  /// 车头方向，范围[0, 360)，以水平向右为起始，逆时针计算
        attack_angle = 0.0;  /// 相对于前进方向的射击角度，范围[0, 360)，以车头方向为起始，逆时针计算
    int mp = MP_MAX,  /// 技能，范围[0, MP_MAX]
        mag = MAG_MAX,  /// 弹夹，范围[0, MAG_MAX]
        team = 0,  /// 队伍id
        def_cd_time = 0,  /// 能量护罩释放时间
        atk_cd_time = 0,  /// 攻击释放时间
        change_mag_time = 0,  /// 换弹夹时间
        spdup_cd_time = 0,  /// 加速释放时间
        slowdown_cd_time = 0;  /// 延缓射线释放时间
    Map* map = NULL;  /// 所属地图
    Point<double> coor = Point<double>(0.0, 0.0);  /// 坐标，内部计算均采用double类型，对外呈现均为int整型
    SPD_STATUS spd_status = SPD_NORM;  /// 速度状态
    DEF_STATUS def_status = DEF_NORM;  /// 防御状态
    ATK_STATUS atk_status = ATK_NORM;  /// 攻击状态
    SLOWDOWN_STATUS sd_status = SLOWDOWN_NORM;  ///延缓射线状态
};