// ��Ϸ���߼�

//��ҪgetTime()������Ϸ��ǰ֡��

//ÿһ֡��Ҫ����
//1.����Mapˢ�µ���
//2.���˳����ò�ͬС����car->frameRoutine()
//      (1)�ж���ȴ״̬
//      (2)�����û�����
//      (3)�ƶ�����һλ��
//      (4)����С���ط��ļ��ṹ��
//3.��ʱ����
//4.�ж��Ƿ����

#include "car.h"

class Game {
public:
    Game();
    int getTime() { return frame; }

private:
    int frame;
    Car *car1, *car2;
};