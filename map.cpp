#include "map.h"
#include <bits/stdc++.h>

char ch[200][200];
char ch_copy[200][200];
char ch_goods[200][200];
unordered_map<string, int> goods_value_mp;
unordered_map<string, int> goods_time_mp;
int goods_density[200][200];
int goods_vector_index = 0;
vector<Goods> goods;


void read_map(){
	for(int i = 0; i < N; i ++){
        scanf("%s", ch[i]);
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            ch_copy[i][j] = ch[i][j];
        }
    }
}




