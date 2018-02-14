// С��
#pragma once
#include "common.h"
#include "game.h"
#include "map.h"
#include "infoFormat.h"
#include <vector>

class Car;
extern void playerFunc(Car* car, Map* map, Game* game);

///    \brief  С��������  
///  
///    ÿ��С����λ��������coor��ǰ������car_angle�������ǰ�����������Ƕ�attack_angleȷ����
///    ����Ѫ��hp������mp��������mag��ͨ���������ٶ�lspd��rspd����С���˶���ͨ����ת����ǶȽ�����׼��
///    useBuff����ÿֻ֡���ͷ�һ����������(�����������С����١��ӻ����ߡ�����)
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
    SPD_STATUS getSpdCdStatus() { return spd_status; }
    DEF_STATUS getDefStatus() { return def_status; }
    BUFF_CD_STATUS getAtkCdStatus() { return atk_cd_status; }
    BUFF_CD_STATUS getSpeedUpCdStatus() { return spdup_cd_status; }
    BUFF_CD_STATUS getDefCdStatus() { return def_cd_status; }
    BUFF_CD_STATUS getSlowDownCdStatus() { return slowdown_cd_status; }
    BUFF_CD_STATUS getChangeMagCdStatus() { return changemag_cd_status; }
    int getSlowedDownTime() { return sloweddown_time; }

    /// TODO: change to real function.
    int getTime() { return game ? game->getTime() : -1; }

    /// �ж��Ƿ��ڽ���
    ///    
    ///     @return    �Ƿ��ڽ���
    bool isOOR();

    /// ���������ٶ�
    ///    
    ///     ������Ϊǰ������Ϊ���ˣ�0Ϊ����
    ///     ��_lspd<-spd_status*SPD_BASE��������Ϊ-spd_status*SPD_BASE����_lspd>spd_status*SPD_BASE��
    ///     ������Ϊspd_status*SPD_BASE����������Ϊ_lspd��
    ///     @param _lspd ���õ������ٶ�
    ///     @return    ���ú�������ٶ�
    double setLeftSpeed(double _lspd);

    /// ���������ٶ�
    ///    
    ///     ������Ϊǰ������Ϊ���ˣ�0Ϊ����
    ///     ��_rspd<-spd_status*SPD_BASE��������Ϊ-spd_status*SPD_BASE����_rspd>spd_status*SPD_BASE��
    ///     ������Ϊspd_status*SPD_BASE����������Ϊ_rspd��
    ///     @param _rspd ���õ������ٶ�
    ///     @return    ���ú�������ٶ�
    double setRightSpeed(double _rspd);

    /// ��ת��׼����
    ///    
    ///     �Ƕ���Ϊ��ʱ�룬��Ϊ˳ʱ�룬0Ϊ�������ýǶ�Ϊ���ǰ���������ʱ��Ƕ�
    ///     ��_angle<-ROTATE_SPD��������Ϊ-ROTATE_SPD����_angle>ROTATE_SPD��
    ///     ������ΪROTATE_SPD����������Ϊ_angle��
    ///     @param _angle ��׼��ʱ����ת�Ƕ�
    ///     @return    ��ת������׼�������ǰ���������ʱ��Ƕ�
    double rotateAttack(double _angle);

    /// ������
    ///    
    ///     �������У�����ATK_MAG״̬���޷����й�������ʱMAG_CHANGETIME��֮�����ATK_NORM״̬
    ///     @return    �Ƿ�ɹ��ͷż���
    bool changeMag();

    /// �����ӻ�����
    ///    
    ///     ��mp>=BUFF_MP[BUFF_SLOWDOWN]�ҷǼ�����ȴ����£�����׼�������ӻ����ߡ�
    ///     @return    �Ƿ�ɹ��ͷż���
    bool emitSlowdown();

    /// ����
    ///    
    ///     ��mp>=BUFF_MP[BUFF_SPEEDUP]�ҷǼ�����ȴ������ͷţ���ԭ��Ϊ������ָ�Ϊ���٣���ԭ��Ϊ���ٱ�Ϊ���١�
    ///     @return    �Ƿ�ɹ��ͷż���
    bool speedUp();

    /// �ͷ���������
    ///    
    ///     ��mp>=BUFF_MP[BUFF_PROTECT]�ҷǼ�����ȴ������ͷţ�����ֵ���롣
    ///     @return    �Ƿ�ɹ��ͷż���
    bool protect();

    /// ����
    ///    
    ///     ��mag>=num�ҷǼ�����ȴ�ǻ��������������׼���򹥻������򲻹�����
    ///     @param num �ӵ���������
    ///     @return    �Ƿ�ɹ��ͷż���
    bool attack(ATK_NUM_MAG num);

    /// �����Ұ��Ŀ�����
    ///    
    ///     �����Ұ��С�����ϰ�����ߵ������Ϣ
    ///     @param cars С����Ϣ���飬���ô���
    ///     @param obs �ϰ�����Ϣ���飬���ô���
    ///     @param props ������Ϣ���飬���ô���
    ///     @note    3��vector���ݻ��ȱ���ա�
    void getView(std::vector<car_info>& cars, std::vector<obs_info>& obs, std::vector<prop_info>& props);

protected:
    /// С�����캯��
    ///    
    ///     �趨��ʼλ�á���ͷ�����������飬��Car��ֻ����Game�ഴ��ʵ����
    ///     @param _map С��������ͼָ��
    ///     @param _coor ��ʼ����
    ///     @param _car_angle ��ʼ��ͷ����
    ///     @param _team ��������
    Car(Game* _game, Map* _map, Point<int> _coor = Point<int>(0, 0), double _car_angle = 0.0, int _team = 0)
        :car_angle(_car_angle), team(_team), map(_map) 
    { coor.x = (double)_coor.x; coor.y = (double)_coor.y; }

    /// �����ط��ļ��ṹ��
    ///    
    ///     ���С���ĵ�ǰ֡��Ϣ
    PlayerInfo getPlayerInfo();

    /// �ܵ��ӻ����߹���
    ///    
    ///     ��ԭ״̬Ϊ�������Ϊ���٣���ԭ״̬Ϊ�������Ϊ���١�Ϊ��ֹС���˶�����ͻ�䣬���������ٶ�������룬����ѡ�ֿ����е�����
    void slowedDown();

    /// ����С��λ��
    ///    
    ///     ��Map�趨����ͳ�ͷ����
    ///     @param _coor С������
    ///     @param _tank_angle ��ͷ����
    void setPos(Point<double> _coor, double _car_angle);

    /// ����С������ֵ
    ///    
    ///     ����С������ֵ
    ///     @param _mp �¼���ֵ
    void setMP(int _mp);

    /// ����С������ֵ
    ///    
    ///     ����С������ֵ
    ///     @param _hp ������ֵ
    void setHP(int _hp);

    /// ÿ֡ˢ��״̬
    ///    
    ///     ����С��״̬������ѡ�ֺ������ж���һ֡λ�ã����ػط��ļ��ṹ��
    PlayerInfo frameRoutine();

private:
    /// �ڲ�״̬����
    ///    
    ///     ����С��״̬
    void statusUpdate();

    double hp = HP_MAX,  /// Ѫ������Χ[0, HP_MAX]
        lspd = 0.0,   /// �����ٶȣ���Χ[-spd_status*SPD_BASE, spd_status*SPD_BASE]
        rspd = 0.0,   /// �����ٶȣ���Χ[-spd_status*SPD_BASE, spd_status*SPD_BASE]
        car_angle = 0.0,  /// ��ͷ���򣬷�Χ[0, 360)����ˮƽ����Ϊ��ʼ����ʱ�����
        attack_angle = 0.0;  /// �����ǰ�����������Ƕȣ���Χ[0, 360)���Գ�ͷ����Ϊ��ʼ����ʱ�����
    int mp = MP_MAX,  /// ���ܣ���Χ[0, MP_MAX]
        mag = MAG_MAX,  /// ���У���Χ[0, MAG_MAX]
        team = 0,  /// ����id
        def_cd_time = 0,  /// ���������ͷ�ʱ��
        atk_cd_time = 0,  /// �����ͷ�ʱ��
        changemag_cd_time = 0,  /// ������ʱ��
        spdup_cd_time = 0,  /// �����ͷ�ʱ��
        slowdown_cd_time = 0,  /// �ӻ������ͷ�ʱ��
        sloweddown_time = 0;  /// ��ʼ������ʱ��
    bool useBuff = false, useRotate = false;  /// ��֤ÿ֡�������ܺ͹���ת��ֻ�ܱ�����һ��
    PIInstruction buffRecord = PIInstruction_NULL;  /// ��¼��֡����ʹ��buff
    Map* map = NULL;  /// ������ͼ
    Game* game = NULL;  ///������Ϸ������
    Point<double> coor = Point<double>(0.0, 0.0);  /// ���꣬�ڲ����������double���ͣ�������־�Ϊint����
    SPD_STATUS spd_status = SPD_NORM;  /// �ٶ�״̬
    DEF_STATUS def_status = DEF_NORM;  /// ����״̬
    BUFF_CD_STATUS slowdown_cd_status = BUFF_NORM, spdup_cd_status = BUFF_NORM, 
        atk_cd_status = BUFF_NORM, def_cd_status = BUFF_NORM, changemag_cd_status = BUFF_NORM;  /// ��������ȴ״̬
};