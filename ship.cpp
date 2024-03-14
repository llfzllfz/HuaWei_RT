#include "ship.h"
#include "berth.h"
#include <bits/stdc++.h>
using namespace std;

Boat boat[5 + 5];


void read_boat(){
	for(int i = 0; i < 5; i ++)
        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
}