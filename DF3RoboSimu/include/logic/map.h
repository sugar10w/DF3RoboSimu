// ��ͼ��������Ұ�������ж�����ײ�ж�


#pragma once

class Map;

#include "logic/common.h"
#include "logic/car.h"
#include "logic/prop.h"
#include "logic/game.h"
#include "logic/log_format.h"

static const char* DEFAULT_MAP_FILENAME = "../data/map.txt";

class Map {

private:
    //Prop
    std::vector<Prop> props;
    //�ϰ���λ�� Obstacle 
    std::vector<obs_info> Obstacle; 
    // �Ƿ���Ч
    bool valid;
    //car ��ʵֻ��Ҫ��������λ����Ϣ������ֱ����Game��Ҫ
    const Game* game; //TODO
    const char* filename = DEFAULT_MAP_FILENAME;

public:
    
    // �Ƿ���Ч
    inline bool isValid() const { return valid; };

    // ���캯��������const Game*��const char*�����ļ���
    Map(const Game* _game, const char* _filename = DEFAULT_MAP_FILENAME)
        : game(_game)
    {
        filename = _filename;
        valid = init(_game, _filename);
    }

    // ��ʼ����ͼ
    bool init(const Game* _game, const char* filename = DEFAULT_MAP_FILENAME);
    
    // ���µ���
    void refreshProp(TFrame frame);
    
    // ������ʼ���꣬�����������
    bool getInitPos(PLAYER_ID id, Point<TCoor>& birth_point, TAngle& car_angle) const;

    // �ж�����
    hit_status aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target, TAngle target_angle);
    
    // ˢ��С����Ұ TODO �ĳɻ�÷���ֵ������const�趨
    void getView(Car* car, std::vector<car_info>& cars) const;
    
    // ��ȡ��һ˲������
    Point<TCoor> getNextPos(const Car* car) const; 
    TAngle getNextAngle(const Car* car) const;

    // ��ͼ��ǰ��Ϣ�����ڵ���
    MapInfo getMapInfo() const;



};

