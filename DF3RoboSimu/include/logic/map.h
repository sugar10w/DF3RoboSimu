// ��ͼ��������Ұ�������ж�����ײ�ж�

//��Ҫvoid slowdown(Point<double> coor, double car_angle��)
//1.�жϱ�����С��
//2.���ø�car->slowedDown()


//��Ҫvoid attack(Point<double> coor, double attack_angle, ATK_NUM_MAG mag_num)
//1.�жϱ�����С��
//2.���ø�car->setHP()

//��Ҫvoid setNextPos(Car* car)
//1.������һ�������뷽�򣨿������ϰ��������С����ײ��������ͷ�
//2.�жϵ���ʰȡ

//��Ҫvoid getView(Car* car, std::vector<car_info>& cars, 
//    std::vector<obs_info>& obs, std::vector<prop_info>& props)
//��ȡС����Ұ������С�����ϰ�����ߵ���Ϣ

//���캯���д���char* mapFile
//Map::Map(char* mapFile)

//��ͼ�ļ�����Ҫ���������С������ʼ��ͳ�ͷ���򣬲��ṩ������ȡ
//void Map::getInitPos(PLAYER_ID player_id, Point<TCoor>& coor, double& car_angle)

//��Ҫˢ�µ��ߵĺ���
//void Map::refreshProp(TFrame frame)

//��Ҫ�ط��ļ���Ϣ
//MapInfo Map::getMapInfo()

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

public:
    
    // ���캯��������const Game*��const char*�����ļ���
    Map(const Game* _game, const char* filename = "../../data/map.txt") {
        init(_game, filename);
    }

    // ��ʼ����ͼ
    bool init(const Game* _game, const char* filename = "../../data/map.txt");
    
    // ���µ���
    void refreshProp(TFrame frame);
    
    // ������ʼ���꣬�����������
    bool getInitPos(PLAYER_ID id, Point<TCoor>& birth_point, TAngle& car_angle) const;

    // �ж�����
    int aim_check(Point<TCoor> P_attack, TAngle car_angle, TAngle attack_angle, Point<TCoor> P_target, TAngle target_angle);
    // ˢ��С����Ұ TODO �ĳɻ�÷���ֵ������const�趨
    void getView(Car* car, std::vector<car_info>& cars) const;
    
    // ��ȡ��һ˲������
    Point<TCoor> getNextPos(const Car* car) const;

    // ��ͼ��ǰ��Ϣ�����ڵ���
    MapInfo getMapInfo() const;

};

