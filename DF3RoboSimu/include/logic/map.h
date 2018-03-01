// ��ͼ��������Ұ�������ж�����ײ�ж�


#pragma once

#include"common.h"
#include"car.h"
#include"prop.h"
#include "game.h"

class Map {

private:
    //Prop
    std::vector<Prop> props;
    //�ϰ���λ�� Obstacle 
    std::vector<obs_info> Obstacle; 
    //car ��ʵֻ��Ҫ��������λ����Ϣ������ֱ����Game��Ҫ
    const Game* game; //TODO
    const char* filename = "../../data/map.txt";

public:
    
    // ���캯��������const Game*��const char*�����ļ���
    Map(const Game* _game, const char* _filename = "../../data/map.txt") {
        filename = _filename;
        init(_game, _filename);
    }

    // ��ʼ����ͼ
    bool init(const Game* _game, const char* filename = "../../data/map.txt");
    
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

