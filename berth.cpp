#include "berth.h"
#include <bits/stdc++.h>
using namespace std;


Berth berth[10 + 10];


// void cal_betch(int zhen, int tz){
//     for(int i = 0; i < 5; i++){
//         if(boat[i].status == 0) continue;
//         cerr << zhen << ' ' << i << ' ' << boat[i].status << ' ' << boat[i].pos << ' ' << boat_capacity << ' ' << boat[i].load_num << ' ' << berth[boat[i].state].goods << endl;
//         if(boat[i].pos == -1){
//             printf("ship %d %d\n", i, boat[i].state);
//             boat[i].load_num = 0;
//             cerr << "ship" << endl;
//         }
//         else{
//             int load_num = min(min(berth[boat[i].state].goods, berth[boat[i].state].loading_speed), max(0, boat_capacity - boat[i].load_num));;
//             boat[i].load_num = boat[i].load_num + load_num;
//             berth[boat[i].state].goods -= load_num;
//             if(boat[i].load_num == boat_capacity || (15000 - tz - zhen - 5) < berth[boat[i].state].transport_time){
//                 printf("go %d\n", i);
//                 cerr << "go" << endl;
//             }
//         }
//     }
// }
