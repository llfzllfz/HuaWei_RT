#include <bits/stdc++.h>
#include "robot.h"
#include "ship.h"
#include "map.h"
#include "berth.h"
using namespace std;

const int N = 200;
const int robot_num = 10;
const int berth_num = 10;
const int boat_num = 5;


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

vector<Goods> goods;
int goods_vector_index = 0;


int money, boat_capacity, id;

void Init()
{
    read_map();
    read_berth();
    for(int i = 0; i < berth_num; i++) robot_berth.push_back(berth[i]);
    
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    cerr << "Init map dis" << endl;
    Init_map_dis();
    cerr << "Finish map dis" << endl;

    printf("OK\n");
    fflush(stdout);
}

int Input()
{
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d", &num);
    for(int i = 1; i <= num; i ++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        // cerr << x << ' ' << y << ' ' << val << endl;
        goods.push_back(Goods(x, y, val, id));
        ch_copy[x][y] = 'g';
        ch_goods[x][y] = 'g';
        goods_value_mp[int2str(x, y)] = val;
        goods_time_mp[int2str(x, y)] = id;
    }
    read_robot();
    read_boat();
    
    char okk[100];
    scanf("%s", okk);
    return id;
}

void Synchronize(int zhen){
    while(goods_vector_index < goods.size() and goods[goods_vector_index].time - zhen + 1 > 1000){
        ch_copy[goods[goods_vector_index].x][goods[goods_vector_index].y] = ch[goods[goods_vector_index].x][goods[goods_vector_index].y];
        ch_goods[goods[goods_vector_index].x][goods[goods_vector_index].y] = ch[goods[goods_vector_index].x][goods[goods_vector_index].y];
        goods_value_mp.erase(int2str(goods[goods_vector_index].x, goods[goods_vector_index].y));
        goods_vector_index++;
    }
}


void Init_(){
    for(int i = 0; i < robot_num; i++){
        robot[i].berth = i;
    }
    for(int i = 0; i < berth_num; i++){
        berth[i].efficiency = berth[i].transport_time * 2 + boat_capacity / berth[i].loading_speed;
    }
}

struct berth_str{
    int i;
    int efficiency;
    berth_str() {}
    berth_str(int _, int __) {
        i = _;
        efficiency = __;
    }
};

int sort_berth_str(const berth_str &a, const berth_str &b){
    return a.efficiency < b.efficiency;
}

map<int,int> first_load_time;
void Init__(){
    vector<berth_str> tmp;
    for(int i = 0; i < berth_num; i++){
        tmp.push_back(berth_str(i, berth[i].efficiency));
    }
    sort(tmp.begin(), tmp.end(), sort_berth_str);
    for(int i = 0; i < 5; i++){
        first_load_time[i] = 15000 % tmp[i].efficiency;
        robot[i].berth = i;
        robot[i + 5].berth = i;
        boat[i].state = i;
    }
}

void cal_betch(int zhen, int tz, int boat_capacity){
    for(int i = 0; i < 5; i++){
        if(boat[i].status == 0) continue;
        // cerr << zhen << ' ' << i << ' ' << boat[i].status << ' ' << boat[i].pos << ' ' << boat_capacity << ' ' << boat[i].load_num << ' ' << berth[boat[i].state].goods << endl;
        if(boat[i].pos == -1){
            printf("ship %d %d\n", i, boat[i].state);
            boat[i].num = 0;
            cerr << "ship" << endl;
        }
        else{
            int load_num = min(min(berth[boat[i].state].goods_num, berth[boat[i].state].loading_speed), max(0, boat_capacity - boat[i].num));;
            boat[i].num = boat[i].num + load_num;
            berth[boat[i].state].goods_num -= load_num;
            if(boat[i].num == boat_capacity || (15000 - tz - zhen - 5) < berth[boat[i].state].transport_time){
                printf("go %d\n", i);
                cerr << "go" << endl;
            }
        }
    }
}



int main()
{
    // cout << int2str(1, 2) << endl;
    // cerr << "Start" << endl;
    Init();
    Init_();
    Init__();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = Input();
        Synchronize(id);
        // cerr << id << endl;
        if(id > 15000) break;
        cal_robot(id);
        cal_betch(id, id - zhen, boat_capacity);
        int berth_all_values = 0;
        for(int i = 0; i < berth_num; i++){
            berth_all_values += berth[i].goods_values;
        }
        cerr << "berth goods value: " << berth_all_values << endl;

        // for(int i = 0; i < robot_num; i ++)
        //     printf("move %d %d\n", i, rand() % 4);
        puts("OK");
        fflush(stdout);
    }

    return 0;
}

