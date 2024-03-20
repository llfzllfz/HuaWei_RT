#ifndef MAP_H
#define MAP_H
#include <bits/stdc++.h>
using namespace std;


struct Goods{
    int x, y;
    int money;
    int time;
    Goods(){}
    Goods(int startx, int starty, int startmoney, int starttime){
        x = startx;
        y = starty;
        money = startmoney;
        time = starttime;
    }
};

extern const int N;
extern char ch[200][200];
extern char ch_copy[200][200];
extern char ch_goods[200][200];
extern unordered_map<string, int> goods_value_mp;
extern unordered_map<string, int> goods_time_mp;
extern int goods_density[200][200];
extern int goods_vector_index;
extern vector<Goods> goods;

void read_map();



#endif