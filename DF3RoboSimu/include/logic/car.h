// С��
#pragma once

#include <vector>

class Car;

#include "logic/common.h"
#include "logic/game.h"
#include "logic/map.h"
#include "logic/log_format.h"

//    \brief  С��������  
//  
//    ÿ��С����λ��������coor��ǰ������car_angle�������ǰ�����������Ƕ�attack_angleȷ����
//    ����Ѫ��hp������mp��������mag��ͨ���������ٶ�lspd��rspd����С���˶���ͨ����ת����ǶȽ�����׼��
//    useBuff����ÿֻ֡���ͷ�һ����������(�����������С����١��ӻ����ߡ�����)
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

    // �ж��Ƿ��ڽ���
    //    
    //     @return    �Ƿ��ڽ���
    bool isOOR();

    // ���������ٶ�
    //    
    //     ������Ϊǰ������Ϊ���ˣ�0Ϊ����
    //     ��_lspd<-spd_status*SPD_BASE��������Ϊ-spd_status*SPD_BASE����_lspd>spd_status*SPD_BASE��
    //     ������Ϊspd_status*SPD_BASE����������Ϊ_lspd��
    //     @param _lspd ���õ������ٶ�
    //     @return    ���ú�������ٶ�
    TSpeed setLeftSpeed(TSpeed _lspd);

    // ���������ٶ�
    //    
    //     ������Ϊǰ������Ϊ���ˣ�0Ϊ����
    //     ��_rspd<-spd_status*SPD_BASE��������Ϊ-spd_status*SPD_BASE����_rspd>spd_status*SPD_BASE��
    //     ������Ϊspd_status*SPD_BASE����������Ϊ_rspd��
    //     @param _rspd ���õ������ٶ�
    //     @return    ���ú�������ٶ�
    TSpeed setRightSpeed(TSpeed _rspd);

    // ��ת��׼����
    //    
    //     �Ƕ���Ϊ��ʱ�룬��Ϊ˳ʱ�룬0Ϊ�������ýǶ�Ϊ���ǰ���������ʱ��Ƕ�
    //     ��_angle<-ROTATE_SPD��������Ϊ-ROTATE_SPD����_angle>ROTATE_SPD��
    //     ������ΪROTATE_SPD����������Ϊ_angle��
    //     @param _target_attack_angle ��Ҫת���ĽǶ�
    //     @return    ��ת������׼�������ǰ���������ʱ��Ƕ�
    TAngle rotateAttack(TAngle _target_attack_angle);

    // ������
    //    
    //     �������У�����ATK_MAG״̬���޷����й�������ʱMAG_CHANGETIME��֮�����ATK_NORM״̬
    //     @return    �Ƿ�ɹ��ͷż���
    bool changeMag();

    // �����ӻ�����
    //    
    //     ��mp>=BUFF_MP[BUFF_SLOWDOWN]�ҷǼ�����ȴ����£�����׼�������ӻ����ߡ�
    //     @return    �Ƿ�ɹ��ͷż���
    bool emitSlowdown();

    // ����
    //    
    //     ��mp>=BUFF_MP[BUFF_SPEEDUP]�ҷǼ�����ȴ������ͷţ���ԭ��Ϊ������ָ�Ϊ���٣���ԭ��Ϊ���ٱ�Ϊ���١�
    //     @return    �Ƿ�ɹ��ͷż���
    bool speedUp();

    // �ͷ���������
    //    
    //     ��mp>=BUFF_MP[BUFF_PROTECT]�ҷǼ�����ȴ������ͷţ�����ֵ���롣
    //     @return    �Ƿ�ɹ��ͷż���
    bool protect();

    // ����
    //    
    //     ��mag>=num�ҷǼ�����ȴ�ǻ��������������׼���򹥻������򲻹�����
    //     @param num �ӵ���������
    //     @return    �Ƿ�ɹ��ͷż���
    bool attack(ATK_NUM_MAG num);

    // �����Ұ��Ŀ�����
    //    
    //     �����Ұ��С�����ϰ�����ߵ������Ϣ
    //     @param cars С����Ϣ���飬���ô���
    //     @param obs �ϰ�����Ϣ���飬���ô���
    //     @param props ������Ϣ���飬���ô���
    //     @note    3��vector���ݻ��ȱ���ա�
    //void getView(std::vector<car_info>& cars, std::vector<obs_info>& obs, std::vector<prop_info>& props);

    // С�����캯��
    //    
    //     �趨��ʼλ�á���ͷ�����������飬��Car��ֻ����Game�ഴ��ʵ����
    //     @param _game 
    //     @param _map   
    //     @param _coor ��ʼ����
    //     @param _car_angle ��ʼ��ͷ����
    //     @param _team ��������
    Car(Game* _game, Map* _map,
        Point<TCoor> _coor = Point<TCoor>(0, 0), TAngle _car_angle = 0.0, PLAYER_ID _team = UNKNOWN_PLAYER)
        : game(_game), map(_map), coor(_coor), car_angle(_car_angle), team(_team)
    { }

protected:
    
    // TODO
    // С��ʹ�õ��߼���
    bool propSpeedUP();
    // С��ʹ�õ��߷���
    bool propProtect();

    // �����ط��ļ��ṹ��
    //    
    //     ���С���ĵ�ǰ֡��Ϣ
    //     @return    С���ĵ�ǰ֡��Ϣ
    PlayerInfo getPlayerInfo();

    // �յ�����
    //
    //      �۳�ָ����Ѫ�����жϴ��
    void getAttacked(ATK_NUM_MAG mag_num, ATK_POS hit_pos);

    // �ܵ��ӻ����߹���
    //    
    //     ��ԭ״̬Ϊ�������Ϊ���٣���ԭ״̬Ϊ�������Ϊ���١�Ϊ��ֹС���˶�����ͻ�䣬���������ٶ�������룬����ѡ�ֿ����е�����
    void slowedDown();

    // ����С��λ��
    //    
    //     ��Map�趨����ͳ�ͷ����
    //     @param _coor С������
    //     @param _tank_angle ��ͷ����
    void setPos(Point<TCoor> _coor, TAngle _car_angle);

    // ����С������ֵ
    //    
    //     ����С������ֵ
    //     @param _mp �¼���ֵ
    void setMP(TMP _mp);

    // ����С������ֵ
    //    
    //     ����С������ֵ
    //     @param _hp ������ֵ
    void setHP(THP _hp);

    // ÿ֡ˢ��״̬
    //    
    //     ����С��״̬������ѡ�ֺ������ж���һ֡λ�ã����ػط��ļ��ṹ��
    PlayerInfo frameRoutine();

private:
    // �ڲ�״̬����
    //    
    //     ����С��״̬
    void statusUpdate();

    THP hp = HP_MAX;  // Ѫ������Χ[0, HP_MAX]
    TSpeed lspd = 0.0;   // �����ٶȣ���Χ[-spd_status*SPD_BASE, spd_status*SPD_BASE]
    TSpeed rspd = 0.0;   // �����ٶȣ���Χ[-spd_status*SPD_BASE, spd_status*SPD_BASE]
    TAngle car_angle = 0.0;  // ��ͷ���򣬷�Χ[0, 360)����ˮƽ����Ϊ��ʼ����ʱ�����
    TAngle attack_angle = 0.0;  // �����ǰ�����������Ƕȣ���Χ[0, 360)���Գ�ͷ����Ϊ��ʼ����ʱ�����
    TMP mp = MP_MAX;  // ���ܣ���Χ[0, MP_MAX]
    TMag mag = MAG_MAX;  // ���У���Χ[0, MAG_MAX]
    PLAYER_ID team = UNKNOWN_PLAYER;  // ����id
    TFrame def_cd_time = -FRAME_MINUTE,  // ���������ͷ�ʱ��
        atk_cd_time = -FRAME_MINUTE,  // �����ͷ�ʱ��
        changemag_cd_time = -FRAME_MINUTE,  // ������ʱ��
        spdup_cd_time = -FRAME_MINUTE,  // �����ͷ�ʱ��
        slowdown_cd_time = -FRAME_MINUTE,  // �ӻ������ͷ�ʱ��
        sloweddown_time = -FRAME_MINUTE;  // ��ʼ������ʱ��
    PIInstruction buffRecord = PIInstruction_NULL;  // ��¼��֡����ʹ��buff

    Map* map = NULL;  // ������ͼ
    Game* game = NULL;  //������Ϸ������
    Point<TCoor> coor = Point<TCoor>(0.0, 0.0);  // ���꣬�ڲ����������double���ͣ�������־�Ϊint����
    SPD_STATUS spd_status = SPD_NORM;  // �ٶ�״̬
    DEF_STATUS def_status = DEF_NORM;  // ����״̬
    BUFF_CD_STATUS slowdown_cd_status = BUFF_NORM, spdup_cd_status = BUFF_NORM, 
        atk_cd_status = BUFF_NORM, def_cd_status = BUFF_NORM, changemag_cd_status = BUFF_NORM;  // ��������ȴ״̬

};