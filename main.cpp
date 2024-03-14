#include <bits/stdc++.h>
#include "robot.h"
#include "ship.h"
using namespace std;

const int N = 200;
const int robot_num = 10;
const int berth_num = 10;
const int boat_num = 5;

// struct Robot
// {
//     int x, y, goods;
//     int status;
//     int mbx, mby;
//     int berth=-1;
//     int dis;
//     stack<int> robot_find_goods_move;
//     stack<int> robot_find_berth_move;
//     Robot() {}
//     Robot(int startX, int startY) {
//         x = startX;
//         y = startY;
//     }
// }robot[robot_num + 10];


// // extern string int2str(int x, int y);

// struct Point{
//     int x, y, dis, money;
//     Point() {}
//     Point(int startX, int startY) {
//         x = startX;
//         y = startY;
//     }
//     Point(int startX, int startY, int startmoney, int startdis){
//         x = startX;
//         y = startY;
//         money = startmoney;
//         dis = startdis;
//     }
// };



// struct Robot_ans{
//     int x, y, move;
//     int change = 0;
//     Robot_ans() {}
//     Robot_ans(int x1, int y1, int move1){
//         x = x1;
//         y = y1;
//         move = move1;
//     }
// };

// struct Berth
// {
//     int x;
//     int y;
//     int transport_time; //运输时间
//     int loading_speed; //装载速度
//     int goods;
//     int status;
//     int efficiency;
//     int cost;
//     Berth(){}
//     Berth(int x, int y, int transport_time, int loading_speed) {
//         this -> x = x;
//         this -> y = y;
//         this -> transport_time = transport_time;
//         this -> loading_speed = loading_speed;
//     }
// }berth[berth_num + 10];

// struct Goods{
//     int x, y;
//     int money;
//     int time;
//     Goods(){}
//     Goods(int startx, int starty, int startmoney, int starttime){
//         x = startx;
//         y = starty;
//         money = startmoney;
//         time = starttime;
//     }
// };

// vector<Goods> goods;
// int goods_vector_index = 0;
// unordered_map<string, int> goods_value_mp;

// struct Boat
// {
//     int num, pos, status;
//     int load_num;
//     int pre_go;
//     int pre_move = -1;
//     int state;
//     queue<int> q;
// }boat[10];

// int money, boat_capacity, id;
// char ch[N][N];
// char ch_copy[N][N];
// int gds[N][N];

// void Init()
// {
//     for(int i = 0; i < N; i ++){
//         scanf("%s", ch[i]);
//     }
//     for(int i = 0; i < N; i++){
//         for(int j = 0; j < N; j++){
//             ch_copy[i][j] = ch[i][j];
//         }
//     }
//     for(int i = 0; i < berth_num; i ++)
//     {
//         int id;
//         scanf("%d", &id);
//         scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
//     }
//     scanf("%d", &boat_capacity);
//     char okk[100];
//     scanf("%s", okk);
//     printf("OK\n");
//     fflush(stdout);
// }



// int Input()
// {
//     scanf("%d%d", &id, &money);
//     int num;
//     scanf("%d", &num);
//     for(int i = 1; i <= num; i ++)
//     {
//         int x, y, val;
//         scanf("%d%d%d", &x, &y, &val);
//         goods.push_back(Goods(x, y, val, id));
//         ch_copy[x][y] = 'g';
//         goods_value_mp[int2str(x, y)] = val;
//         // cerr << val << endl;
//     }
//     for(int i = 0; i < robot_num; i ++)
//     {
//         scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &robot[i].status);
//     }
//     for(int i = 0; i < 5; i ++)
//         scanf("%d%d\n", &boat[i].status, &boat[i].pos);
//     char okk[100];
//     scanf("%s", okk);
//     return id;
// }

// void Synchronize(int zhen){
//     while(goods_vector_index < goods.size() and goods[goods_vector_index].time - zhen + 1 > 1000){
//         ch_copy[goods[goods_vector_index].x][goods[goods_vector_index].y] = ch[goods[goods_vector_index].x][goods[goods_vector_index].y];
//         goods_value_mp.erase(int2str(goods[goods_vector_index].x, goods[goods_vector_index].y));
//         goods_vector_index++;
//     }
// }


// void Init_(){
//     for(int i = 0; i < robot_num; i++){
//         robot[i].berth = i;
//     }
//     for(int i = 0; i < berth_num; i++){
//         berth[i].efficiency = berth[i].transport_time + boat_capacity / berth[i].loading_speed;
//     }
// }

// struct berth_str{
//     int i;
//     int efficiency;
//     berth_str() {}
//     berth_str(int _, int __) {
//         i = _;
//         efficiency = __;
//     }
// };

// int sort_berth_str(const berth_str &a, const berth_str &b){
//     return a.efficiency < b.efficiency;
// }

// map<int,int> first_load_time;
// void Init__(){
//     vector<berth_str> tmp;
//     for(int i = 0; i < berth_num; i++){
//         tmp.push_back(berth_str(i, berth[i].efficiency));
//     }
//     sort(tmp.begin(), tmp.end(), sort_berth_str);
//     for(int i = 0; i < 5; i++){
//         first_load_time[i] = 15000 % tmp[i].efficiency;
//         robot[i].berth = i;
//         robot[i + 5].berth = i;
//         boat[i].state = i;
//     }

// }



int main()
{
    cout << int2str(1, 2) << endl;
    // Init();
    // Init_();
    // Init__();
    // for(int zhen = 1; zhen <= 15000; zhen ++)
    // {
    //     int id = Input();
    //     Synchronize(id);
    //     // cerr << id << endl;
    //     if(id > 15000) break;
    //     cal_robot();
    //     cal_betch(id, id - zhen);

    //     // for(int i = 0; i < robot_num; i ++)
    //     //     printf("move %d %d\n", i, rand() % 4);
    //     puts("OK");
    //     fflush(stdout);
    // }

    return 0;
}

