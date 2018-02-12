// 地图，包括障碍物判定等

//需要void slowdown(Point<double> coor, double car_angle)
//1.判断被攻击小车
//2.调用该car->slowedDown()


//需要void attack(Point<double> coor, double attack_angle)
//1.判断被攻击小车
//2.调用该car->setHP()

//需要void setNextPos(Car* car)
//1.计算下一处坐标与方向（考虑与障碍物或其他小车碰撞），出界惩罚
//2.判断道具拾取

//需要void getView(Car* car, std::vector<car_info>& cars, 
//    std::vector<obs_info>& obs, std::vector<prop_info>& props)
//获取小车视野内其余小车、障碍物、道具的信息