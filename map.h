#ifndef MAP_H
#define MAP_H
#include <bits/stdc++.h>
using namespace std;

extern const int N;
extern char ch[200][200];
extern char ch_copy[200][200];
extern unordered_map<string, int> goods_value_mp;
extern unordered_map<string, int> goods_time_mp;

void read_map();



#endif