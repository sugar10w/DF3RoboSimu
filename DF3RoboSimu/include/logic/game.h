// ��Ϸ���߼�

//��ҪgetTime()������Ϸ��ǰ֡��

//ÿһ֡��Ҫ����
//0.�����û�����
//1.����Mapˢ�µ���
//2.��ż�������С����car->frameRoutine()
//      (1)�ж���ȴ״̬
//      (2)�ƶ�����һλ��
//      (3)�ͷż��ܲ��໥�˺�
//      (4)����С���ط��ļ��ṹ��
//3.��ͼ�˺�����ʱ����
//4.�ж��Ƿ����

#pragma once

#include "map.h"
#include "car.h"

class Game {
public:

    // ���캯��������Map��Car
    Game();
    
    
    TFrame getTime() const { return frame; }

private:
    TFrame frame;
    Map *map;
    Car *car[2];
};