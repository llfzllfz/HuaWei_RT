#ifndef ROBOT_H
#define ROBOT_H
#include <bits/stdc++.h>
using namespace std;


extern const int N;
extern const int berth_num;
extern const int robot_num;



extern char ch[][200];
extern char ch_copy[][200];

// 机器人
struct Robot
{
    int x, y, goods; // x, y 坐标，goods 是否携带货物
    int status;  // 机器人状态
    int berth=-1; // 当前机器人负责的港口
    int dis; // 距离
    stack<int> robot_find_goods_move; // 到货物的移动路径
    stack<int> robot_find_berth_move; // 到港口的移动路径
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }
};

// 地图上的点
struct Point{
    int x, y, dis, money;
    Point() {}
    Point(int startX, int startY) {
        x = startX;
        y = startY;
    }
    Point(int startX, int startY, int startmoney, int startdis){
        x = startX;
        y = startY;
        money = startmoney;
        dis = startdis;
    }
};


// 边界检测
int check(int x, int y);
// 机器人路径清空
void Init_robot(Robot &x);
// int转string
string int_pad2str(int x);
string int2str(int x, int y);
// 计算初始时候的所有点到港口的距离
int cal_dis(int x, int y, int ux, int uy);
void Init_map_dis();

#endif