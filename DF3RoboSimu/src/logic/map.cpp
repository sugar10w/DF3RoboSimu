#include "..\..\include\logic\map.h"

// 地图，包括障碍物判定等

bool Map::init()
{
    std::ifstream file("../../data/map.txt");
    if (!file.is_open()) {
        std::cout << "未能成功打开地图文件" << std::endl;
        return false;
    }
    else {
        std::string temp;
        std::vector<std::string> paras;

        std::getline(file, temp);
        paras = split(temp, ',');
        HpPackage_1 = Prop(HP_PAK, atof(paras[1].c_str()), atof(paras[2].c_str()), 1);

        std::getline(file, temp);
        paras = split(temp, ',');
        HpPackage_2 = Prop(HP_PAK, atof(paras[1].c_str()), atof(paras[2].c_str()), 2);

        std::getline(file, temp);
        paras = split(temp, ',');
        MpPackage_1 = Prop(MP_PAK, atof(paras[1].c_str()), atof(paras[2].c_str()), 2);

        std::getline(file, temp);
        paras = split(temp, ',');
        MpPackage_2 = Prop(MP_PAK, atof(paras[1].c_str()), atof(paras[2].c_str()), 2);

        std::getline(file, temp);
        paras = split(temp, ',');
        SpeedBuff = Prop(SPD_BUFF, atof(paras[1].c_str()), atof(paras[2].c_str()), 0);

        std::getline(file, temp);
        paras = split(temp, ',');
        DefendBuff = Prop(DEF_BUFF, atof(paras[1].c_str()), atof(paras[2].c_str()), 0);

        std::getline(file, temp);
        paras = split(temp, ',');
        car1 = new Car(Point<TCoor>(atof(paras[1].c_str()), atof(paras[2].c_str())), atof(paras[3].c_str()), 1);
        //toDo: 为什么没有权限（已经是友元类了）
        std::getline(file, temp);
        paras = split(temp, ',');
        car2 = new Car(Point<TCoor>(atof(paras[1].c_str()), atof(paras[2].c_str())), atof(paras[3].c_str()), 2);

        std::getline(file, temp);
        paras = split(temp, ',');
        ObstacleRadius = atof(paras[1].c_str());

        while (std::getline(file, temp))
        {
            paras = split(temp, ',');
            Obstacle.push_back(Point<TCoor>(atof(paras[2].c_str()), atof(paras[1].c_str())));
        }
    }
}
std::vector<std::string> split(std::string s, char c) {
    int len = s.length;
    int index_start = 0;
    int index_end = 0;
    std::vector<std::string> result;
    while (index_end < len) {
        if (s.c_str()[index_end] == c) {
            if (index_end != index_start)
                result.push_back(s.substr(index_start, index_end - 1));
            index_start = index_end + 1;
            index_end = index_start;
        }
        else {
            index_end++;
        }
    }
    result.push_back(s.substr(index_start, index_end - 1));
    return result;
}