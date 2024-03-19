#ifndef ROBOT_H
#define ROBOT_H
#include <bits/stdc++.h>
using namespace std;

extern const int N;
extern const int berth_num;
extern const int robot_num;

extern char ch[][200];
extern char ch_copy[][200];


// 地图上的货物点
struct Point{
    int x, y, dis, money;
    int goods2berth_dis = 10000;
    int pre_dis;
    int berth = -1;
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

// 机器人
struct Robot
{
    int x, y, goods; // x, y 坐标，goods 是否携带货物
    int goods_x, goods_y; // 锁定货物的坐标
    int status;  // 机器人状态
    int berth=-1; // 当前机器人负责的港口
    int dis; // 距离
    int goods_values; //携带的货物价值
    stack<int> robot_find_goods_move; // 到货物的移动路径
    stack<int> robot_find_berth_move; // 到港口的移动路径
    // stack<Point> best_goods; //最优的十个货物选择
    vector<Point> best_goods;
    int robot_visit_find_goods[200+1][200+1];
    int fax_find_goods[200][200];
    int fay_find_goods[200][200];
    int famove_find_goods[200][200];
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }
};

// 机器人移动
struct Robot_ans{
    int x, y, move;
    int change = 0;
    Robot_ans() {}
    Robot_ans(int x1, int y1, int move1){
        x = x1;
        y = y1;
        move = move1;
    }
};

extern Robot robot[10 + 10];

// 边界检测
int check(int x, int y);
// 机器人路径清空
void Init_robot(Robot &x);
// int转string
string int_pad2str(int x);
string int2str(int x, int y);
// 计算初始时候的所有点到港口的距离
void cal_dis(int x, int y);
void Init_map_dis();
// 寻路系统-寻找货物路径
void bfs_find_goods(int robot_index, int zhen, float pow_index);
// 寻路系统-寻找港口路径
void bfs_find_berth(int x, int y, int ux, int uy, int robot_index, int zhen);
// 控制机器人
void cal_robot(int zhen);
// 碰撞系统检测
vector<Robot_ans> pz_judge(vector<Robot_ans> v);
// 读取机器人状态
void read_robot();
int check_berth(int x, int y);
int find_nearst_berth(int x, int y, int robot_index);
int select2best_point(Point x1, Point x2, float pow_index);
int sort_best_goods(Point x1, Point x2);
int find_pzset_move(vector<int> pz_set, vector<string> pz_v, int x, int y, int ux, int uy);

#endif