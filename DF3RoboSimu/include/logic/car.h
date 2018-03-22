// 小车
#pragma once

#include <vector>

class Car;

#include "logic/common.h"
#include "logic/game.h"
#include "logic/map.h"
#include "logic/log_format.h"

//    \brief  小车仿真类  
//  
//    每个小车的位置由坐标coor、前进方向car_angle、相对于前进方向的射击角度attack_angle确定。
//    包含血量hp、技能mp、弹夹数mag，通过左右轮速度lspd、rspd控制小车运动，通过旋转射击角度进行瞄准。
//    useBuff控制每帧只能释放一个主动技能(攻击、换弹夹、加速、延缓射线、护罩)
//
class Car {
public:
    //friend class Map;
    friend class Game;

    Point<TCoor> getCoor() const { return Point<TCoor>((TCoor)coor.x, (TCoor)coor.y); }
    THP getHP() const { return hp; }
    TMP getMP() const { return mp; }
    TMag getMAG() const { return mag; }
    PLAYER_ID getTeam() const { return team; }
    TAngle getCarAngle() const { return car_angle; }
    TAngle getAttackAngle() const { return attack_angle; }
    TSpeed getLeftSpeed() const { return lspd; }
    TSpeed getRightSpeed() const { return rspd; }
    SPD_STATUS getSpdStatus() const { return spd_status; }
    DEF_STATUS getDefStatus() const { return def_status; }
    BUFF_CD_STATUS getAtkCdStatus() const { return atk_cd_status; }
    BUFF_CD_STATUS getSpeedUpCdStatus() const { return spdup_cd_status; }
    BUFF_CD_STATUS getDefCdStatus() const { return def_cd_status; }
    BUFF_CD_STATUS getSlowDownCdStatus() const { return slowdown_cd_status; }
    BUFF_CD_STATUS getChangeMagCdStatus() const { return changemag_cd_status; }
    TFrame getSlowedDownTime() const { return sloweddown_time; }

    TFrame getTime() const; 

    car_info getCarInfo() const {
        car_info ci;
        ci.coor = coor;
        ci.hp = hp;
        ci.tank_angle = car_angle;
        ci.attack_angle = attack_angle;
        ci.team = team;
        return ci;
    }

    // 判断是否在界外
    //    
    //     @return    是否在界外
    bool isOOR();

    // 设置左轮速度
    //    
    //     轮速正为前进，负为后退，0为不动
    //     若_lspd<-spd_status*SPD_BASE，则设置为-spd_status*SPD_BASE，若_lspd>spd_status*SPD_BASE，
    //     则设置为spd_status*SPD_BASE，否则设置为_lspd。
    //     @param _lspd 设置的左轮速度
    //     @return    设置后的左轮速度
    TSpeed setLeftSpeed(TSpeed _lspd);

    // 设置右轮速度
    //    
    //     轮速正为前进，负为后退，0为不动
    //     若_rspd<-spd_status*SPD_BASE，则设置为-spd_status*SPD_BASE，若_rspd>spd_status*SPD_BASE，
    //     则设置为spd_status*SPD_BASE，否则设置为_rspd。
    //     @param _rspd 设置的右轮速度
    //     @return    设置后的右轮速度
    TSpeed setRightSpeed(TSpeed _rspd);

    // 旋转瞄准方向
    //    
    //     角度正为逆时针，负为顺时针，0为不动，该角度为相对前进方向的逆时针角度
    //     若_angle<-ROTATE_SPD，则设置为-ROTATE_SPD，若_angle>ROTATE_SPD，
    //     则设置为ROTATE_SPD，否则设置为_angle。
    //     @param _target_attack_angle 想要转到的角度
    //     @return    旋转后后的瞄准方向相对前进方向的逆时针角度
    TAngle rotateAttack(TAngle _target_attack_angle);

    // 换弹夹
    //    
    //     更换弹夹，进入ATK_MAG状态，无法进行攻击，耗时MAG_CHANGETIME，之后进入ATK_NORM状态
    //     @return    是否成功释放技能
    bool changeMag();

    // 发射延缓射线
    //    
    //     在mp>=BUFF_MP[BUFF_SLOWDOWN]且非技能冷却情况下，沿瞄准方向发射延缓射线。
    //     @return    是否成功释放技能
    bool emitSlowdown();

    // 加速
    //    
    //     在mp>=BUFF_MP[BUFF_SPEEDUP]且非技能冷却情况下释放，若原本为减速则恢复为常速，若原本为常速变为加速。
    //     @return    是否成功释放技能
    bool speedUp();

    // 释放能量护罩
    //    
    //     在mp>=BUFF_MP[BUFF_PROTECT]且非技能冷却情况下释放，受伤值减半。
    //     @return    是否成功释放技能
    bool protect();

    // 攻击
    //    
    //     在mag>=num且非技能冷却非换弹夹情况下沿瞄准方向攻击，否则不攻击。
    //     @param num 子弹发射数量
    //     @return    是否成功释放技能
    bool attack(ATK_NUM_MAG num);

    // 获得视野内目标情况
    //    
    //     获得视野内小车、障碍物、道具的相关信息
    //     @param cars 小车信息数组，引用传递
    //     @param obs 障碍物信息数组，引用传递
    //     @param props 道具信息数组，引用传递
    //     @note    3个vector内容会先被清空。
    //void getView(std::vector<car_info>& cars, std::vector<obs_info>& obs, std::vector<prop_info>& props);

    // 小车构造函数
    //    
    //     设定初始位置、车头方向、所属队伍，该Car类只能由Game类创造实例。
    //     @param _game 
    //     @param _map   
    //     @param _coor 初始坐标
    //     @param _car_angle 初始车头朝向
    //     @param _team 所属队伍
    Car(Game* _game, Map* _map,
        Point<TCoor> _coor = Point<TCoor>(0, 0), TAngle _car_angle = 0.0, PLAYER_ID _team = UNKNOWN_PLAYER)
        : game(_game), map(_map), coor(_coor), car_angle(_car_angle), team(_team)
    { }

protected:
    
    // TODO
    // 小车使用道具加速
    bool propSpeedUP();
    // 小车使用道具防御
    bool propProtect();

    // 导出回放文件结构体
    //    
    //     填充小车的当前帧信息
    //     @return    小车的当前帧信息
    PlayerInfo getPlayerInfo();

    // 收到攻击
    //
    //      扣除指定的血量；判断存活
    void getAttacked(ATK_NUM_MAG mag_num, ATK_POS hit_pos);

    // 受到延缓射线攻击
    //    
    //     若原状态为常速则变为减速，若原状态为加速则变为常速。为防止小车运动方向突变，两个轮子速度整体减半，后续选手可自行调整。
    void slowedDown();

    // 设置小车位置
    //    
    //     由Map设定坐标和车头方向。
    //     @param _coor 小车坐标
    //     @param _tank_angle 车头方向
    void setPos(Point<TCoor> _coor, TAngle _car_angle);

    // 设置小车技能值
    //    
    //     设置小车技能值
    //     @param _mp 新技能值
    void setMP(TMP _mp);

    // 设置小车生命值
    //    
    //     设置小车生命值
    //     @param _hp 新生命值
    void setHP(THP _hp);

    // 每帧刷新状态
    //    
    //     更新小车状态，调用选手函数，判断下一帧位置，返回回放文件结构体
    PlayerInfo frameRoutine();

private:
    // 内部状态更新
    //    
    //     更新小车状态
    void statusUpdate();

    THP hp = HP_MAX;  // 血量，范围[0, HP_MAX]
    TSpeed lspd = 0.0;   // 左轮速度，范围[-spd_status*SPD_BASE, spd_status*SPD_BASE]
    TSpeed rspd = 0.0;   // 右轮速度，范围[-spd_status*SPD_BASE, spd_status*SPD_BASE]
    TAngle car_angle = 0.0;  // 车头方向，范围[0, 360)，以水平向右为起始，逆时针计算
    TAngle attack_angle = 0.0;  // 相对于前进方向的射击角度，范围[0, 360)，以车头方向为起始，逆时针计算
    TMP mp = MP_MAX;  // 技能，范围[0, MP_MAX]
    TMag mag = MAG_MAX;  // 弹夹，范围[0, MAG_MAX]
    PLAYER_ID team = UNKNOWN_PLAYER;  // 队伍id
    TFrame def_cd_time = -FRAME_MINUTE,  // 能量护罩释放时间
        atk_cd_time = -FRAME_MINUTE,  // 攻击释放时间
        changemag_cd_time = -FRAME_MINUTE,  // 换弹夹时间
        spdup_cd_time = -FRAME_MINUTE,  // 加速释放时间
        slowdown_cd_time = -FRAME_MINUTE,  // 延缓射线释放时间
        sloweddown_time = -FRAME_MINUTE;  // 开始被减速时间
    PIInstruction buffRecord = PIInstruction_NULL;  // 记录该帧内所使用buff

    Map* map = NULL;  // 所属地图
    Game* game = NULL;  //所属游戏控制类
    Point<TCoor> coor = Point<TCoor>(0.0, 0.0);  // 坐标，内部计算均采用double类型，对外呈现均为int整型
    SPD_STATUS spd_status = SPD_NORM;  // 速度状态
    DEF_STATUS def_status = DEF_NORM;  // 防御状态
    BUFF_CD_STATUS slowdown_cd_status = BUFF_NORM, spdup_cd_status = BUFF_NORM, 
        atk_cd_status = BUFF_NORM, def_cd_status = BUFF_NORM, changemag_cd_status = BUFF_NORM;  // 各技能冷却状态

};