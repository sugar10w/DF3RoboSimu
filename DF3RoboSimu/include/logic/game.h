// ��Ϸ���߼�

//��ҪgetTime()������Ϸ��ǰ֡��

//ÿһ֡��Ҫ����
//0.��ż��������û�����
//1.��ͼ�˺�����ʱ����
//2.����Mapˢ�µ���
//3.����С����car->frameRoutine()
//      (1)�ж���ȴ״̬
//      (2)�ƶ�����һλ��
//      (3)����С���ط��ļ��ṹ��
//4.д�ط��ļ�
//5.�ж��Ƿ����

#pragma once

#include <iostream>
#include <fstream>
#include <ctime>

class Game;

#include "logic/map.h"
#include "logic/car.h"


class Game {
public:
    friend int main(int argc, char** argv);

    // ���캯��������Map��Car
    Game(char* recordFile, char* mapFile);
    // ��ȡ��ǰ��֡��
    TFrame getTime() const { return frame; }

    /* TODO 
    bool �������� {
        if (map->�ж�����) {
            �Է�������; 
            return true;
        } 
        return false;
    } 
    */

    // Car�Ľ�������������
    bool attack(PLAYER_ID id);
    // Car�Ľ�������������
    bool slowdown(PLAYER_ID id);
    

protected:
    // ÿ֡����״̬
    //     @return    P0/P1 Ϊ��ʤ����DRAWΪƽ�֣�UNKNOWN_PLAYERΪ��δ�ֳ�ʤ��
    PLAYER_ID frameRoutine();

private:
    TFrame frame = 0;
    Map *map = NULL;
    Car *car[2] = { NULL, NULL };
    std::fstream record_file;
    int randframe = 0; // �����ȷ����һ֡�Ⱥ�˳��
};