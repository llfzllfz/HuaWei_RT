#include "robot.h"
#include "berth.h"
#include "map.h"
#include <bits/stdc++.h>
using namespace std;

//机器人
Robot robot[10 + 10];
// 机器人移动方向
int robot_move_x[] = {0, 0, -1, 1};
int robot_move_y[] = {1, -1, 0, 0};
//寻找货物系统参数
int robot_visit_find_goods[200+1][200+1];
int fax_find_goods[200][200];
int fay_find_goods[200][200];
int famove_find_goods[200][200];
//寻找港口系统参数
int robot_visit_find_berth[200+1][200+1];
int fax_find_berth[200][200];
int fay_find_berth[200][200];
int famove_find_berth[200][200];

unordered_map<int, Point> cp;
// 初始化到港口距离参数
int Init_bfs_vist[200][200];
int map_dis[200][200][10];

int check(int x, int y){
    if(x < 0 || x >= N) return -1;
    if(y < 0 || y >= N) return -1;
    if(ch_copy[x][y] == '#' || ch_copy[x][y] =='*') return -1;
    return 1;
}

string int_pad2str(int x){
    string sx = "";
    while(x){
        int mod = x % 10;
        x /= 10;
        sx = sx + char(mod + '0');
    }
    while(sx.size() < 3){
        sx = '0' + sx;
    }
    return sx;
}

string int2str(int x, int y){
    string sx = int_pad2str(x);
    string sy = int_pad2str(y);
    return sx + sy;
}

void Init_robot(Robot &x){
    while(!x.robot_find_goods_move.empty()) x.robot_find_goods_move.pop();
    while(!x.robot_find_berth_move.empty()) x.robot_find_berth_move.pop();
}

// int compare_bestPoint(Point x1, Point x2){

// }


// 性价比最优，寻找货物系统
Point bfs_find_goods(int x, int y, int robot_index, int zhen){
    memset(robot_visit_find_goods, 0, sizeof(robot_visit_find_goods));
    memset(fax_find_goods, -1, sizeof(fax_find_goods));
    memset(fay_find_goods, -1, sizeof(fay_find_goods));
    memset(famove_find_goods, -1, sizeof(famove_find_goods));
    Point bestPoint = Point(-1, -1, 0, 0);
    queue<Point> q;
    q.push(Point(x, y, 0, 0));
    robot_visit_find_goods[x][y] = 1;

    // cerr << 6 << endl;
    while(!q.empty()){
        Point now = q.front();
        // cerr << now.x << ' ' << now.y << endl;
        q.pop();
        for(int i=0; i<4;i++){
            int next_x = now.x + robot_move_x[i];
            int next_y = now.y + robot_move_y[i];
            if(check(next_x, next_y) == -1) continue;
            if(robot_visit_find_goods[next_x][next_y] == 1) continue;
            Point next = Point(next_x, next_y);
            next.dis = now.dis + 1;
            if(ch_copy[next_x][next_y] == 'g'){
            	if(next.dis > (1000 - (zhen - goods_time_mp[int2str(next_x, next_y)] + 1))) continue;
                int value = goods_value_mp[int2str(next_x, next_y)];
                if(bestPoint.x == -1){
                	for(int j = 0; j < robot_berth.size(); j++){
                		int tmp_berth_index = robot_berth[j].id;
                		if(map_dis[next.x][next.y][tmp_berth_index] == -1) continue;
                		if(bestPoint.goods2berth_dis > map_dis[next.x][next.y][tmp_berth_index]){
                			bestPoint.x = next.x;
		                    bestPoint.y = next.y;
		                    bestPoint.money = value;
		                    bestPoint.dis = next.dis;
                			bestPoint.goods2berth_dis = map_dis[next.x][next.y][tmp_berth_index];
                			bestPoint.berth = tmp_berth_index;
                		}
                	}
                }
                if(bestPoint.x == -1)
                    	continue;
                for(int j = 0; j < robot_berth.size(); j++){
                	int tmp_berth_index = robot_berth[j].id;
                	if(map_dis[next.x][next.y][tmp_berth_index] == -1) continue;
                	int tmp_dis = next.dis + robot[robot_index].dis + map_dis[next.x][next.y][tmp_berth_index];
                	int best_dis = bestPoint.dis + robot[robot_index].dis + bestPoint.goods2berth_dis;
                	if(float(value) / float(tmp_dis) > float(bestPoint.money) / float(best_dis)){
                		bestPoint.x = next.x;
                		bestPoint.y = next.y;
                		bestPoint.money = value;
                		bestPoint.dis = next.dis;
                		bestPoint.goods2berth_dis = map_dis[next.x][next.y][tmp_berth_index];
                		bestPoint.berth = tmp_berth_index;
                	}
                }
            }
            q.push(next);
            robot_visit_find_goods[next.x][next.y] = 1;
            fax_find_goods[next.x][next.y] = now.x;
            fay_find_goods[next.x][next.y] = now.y;
            famove_find_goods[next.x][next.y] = i;
        }
    }
    if(bestPoint.x != -1){
        int now_x = fax_find_goods[bestPoint.x][bestPoint.y];
        int now_y = fay_find_goods[bestPoint.x][bestPoint.y];
        robot[robot_index].robot_find_goods_move.push(famove_find_goods[bestPoint.x][bestPoint.y]);
        while(now_x != x || now_y != y){
            int move = famove_find_goods[now_x][now_y];
            robot[robot_index].robot_find_goods_move.push(move);
            int now_xx = fax_find_goods[now_x][now_y];
            int now_yy = fay_find_goods[now_x][now_y];
            now_x = now_xx;
            now_y = now_yy;
        }
        if(bestPoint.dis != robot[robot_index].robot_find_goods_move.size()){
            cerr << "Not equal" << endl;
            cerr << bestPoint.dis << ' ' << robot[robot_index].robot_find_goods_move.size() << endl;
        }
        robot[robot_index].berth = bestPoint.berth;
    }
    return bestPoint;
}


// 寻找机器人到锁定港口的最优路径
int bfs_find_berth(int x, int y, int ux, int uy, int robot_index){
    memset(robot_visit_find_berth, 0, sizeof(robot_visit_find_berth));
    memset(fax_find_berth, -1, sizeof(fax_find_berth));
    memset(fay_find_berth, -1, sizeof(fay_find_berth));
    memset(famove_find_berth, -1, sizeof(famove_find_berth));
    queue<Point>q;
    q.push(Point(x, y, 0, 0));
    robot_visit_find_berth[x][y] = 1;
    int best_dis = -1;
    int path_connect = -1;
    while(!q.empty()){
        Point now = q.front();
        if(now.x == ux && now.y == uy) {
            best_dis = now.dis;
            path_connect = 1;
            break;
        }
        q.pop();
        for(int i = 0; i < 4; i++){
            int next_x = now.x + robot_move_x[i];
            int next_y = now.y + robot_move_y[i];
            if(check(next_x, next_y) == -1) continue;
            if(robot_visit_find_berth[next_x][next_y] == 1) continue;
            Point next = Point(next_x, next_y);
            next.dis = now.dis + 1;
            q.push(next);
            robot_visit_find_berth[next.x][next.y] = 1;
            fax_find_berth[next.x][next.y] = now.x;
            fay_find_berth[next.x][next.y] = now.y;
            famove_find_berth[next.x][next.y] = i;
        }
    }
    int now_x = ux;
    int now_y = uy;
    while(best_dis != -1 && (now_x != -1  && now_y != -1) && (now_x != x || now_y != y)){
        int move = famove_find_berth[now_x][now_y];
        robot[robot_index].robot_find_berth_move.push(move);
        int now_xx = fax_find_berth[now_x][now_y];
        int now_yy = fay_find_berth[now_x][now_y];
        now_x = now_xx;
        now_y = now_yy;
    }
    if(best_dis != robot[robot_index].robot_find_berth_move.size() && best_dis != -1){
        cerr << "Not equal" << endl;
        cerr << best_dis << ' ' << robot[robot_index].robot_find_berth_move.size() << ' ' << ' ' << now_x << ' ' << now_y << endl;
    }
    return path_connect;
}


int check_berth(int x, int y){
	for(int i = 0; i < robot_berth.size(); i++){
		int tmp_berth_idx = robot_berth[i].id;
		int ux = berth[tmp_berth_idx].x;
		int uy = berth[tmp_berth_idx].y;
		if(x >= ux && y >= uy && x <= ux + 3 && y <= uy + 3) return tmp_berth_idx;
	}
	return -1;
}

// 寻找当前距离最近的开放港口
int find_nearst_berth(int x, int y, int robot_index){
	memset(robot_visit_find_berth, 0, sizeof(robot_visit_find_berth));
    memset(fax_find_berth, -1, sizeof(fax_find_berth));
    memset(fay_find_berth, -1, sizeof(fay_find_berth));
    memset(famove_find_berth, -1, sizeof(famove_find_berth));
    queue<Point>q;
    q.push(Point(x, y, 0, 0));
    robot_visit_find_berth[x][y] = 1;
    int best_berth = -1;
    int ux = -1;
    int uy = -1;
    while(!q.empty()){
    	Point now = q.front();
    	if(ch_copy[now.x][now.y] == 'B'){
    		best_berth = check_berth(now.x, now.y);
    		if(best_berth != -1) {
    			ux = now.x;
    			uy = now.y;
    			break;
    		}
    	}
    	q.pop();
    	for(int i = 0; i < 4; i++){
            int next_x = now.x + robot_move_x[i];
            int next_y = now.y + robot_move_y[i];
            if(check(next_x, next_y) == -1) continue;
            if(robot_visit_find_berth[next_x][next_y] == 1) continue;
            Point next = Point(next_x, next_y);
            next.dis = now.dis + 1;
            q.push(next);
            robot_visit_find_berth[next.x][next.y] = 1;
            fax_find_berth[next.x][next.y] = now.x;
            fay_find_berth[next.x][next.y] = now.y;
            famove_find_berth[next.x][next.y] = i;
        }
    }
    int now_x = ux;
    int now_y = uy;
    while(best_berth != -1 && (now_x != -1  && now_y != -1) && (now_x != x || now_y != y)){
        int move = famove_find_berth[now_x][now_y];
        robot[robot_index].robot_find_berth_move.push(move);
        int now_xx = fax_find_berth[now_x][now_y];
        int now_yy = fay_find_berth[now_x][now_y];
        now_x = now_xx;
        now_y = now_yy;
    }
    // if(best_dis != robot[robot_index].robot_find_berth_move.size() && best_dis != -1){
    //     cerr << "Not equal" << endl;
    //     cerr << best_dis << ' ' << robot[robot_index].robot_find_berth_move.size() << ' ' << ' ' << now_x << ' ' << now_y << endl;
    // }
    return best_berth;
}

// 机器人管理
void cal_robot(int zhen){
    vector<Robot_ans> v;
    for(int i = 0; i < robot_num; i++){
        if(robot[i].status == 0) {
            v.push_back(Robot_ans(robot[i].x, robot[i].y, -1));
            Init_robot(robot[i]);
            continue;
        }
        if(robot[i].goods == 0){
            if(!robot[i].robot_find_goods_move.empty()){
                int move = robot[i].robot_find_goods_move.top();
                v.push_back(Robot_ans(robot[i].x, robot[i].y, move));
            }
            else{
                Init_robot(robot[i]);
                auto it = cp.find(i);
                if(it != cp.end()) ch_copy[cp[i].x][cp[i].y] = ch[cp[i].x][cp[i].y];
                Point bestPoint = bfs_find_goods(robot[i].x, robot[i].y, i, zhen);
                if(bestPoint.x != -1){
	                ch_copy[bestPoint.x][bestPoint.y] = char(i + '0');
	                cp[i] = bestPoint;
                }
                if(!robot[i].robot_find_goods_move.empty())
                	v.push_back(Robot_ans(robot[i].x, robot[i].y, robot[i].robot_find_goods_move.top()));
                else
                	v.push_back(Robot_ans(robot[i].x, robot[i].y, -1));
            }
        }
        else{
            if(!robot[i].robot_find_berth_move.empty()){
                int move = robot[i].robot_find_berth_move.top();
                v.push_back(Robot_ans(robot[i].x, robot[i].y, move));
            }
            else{
                Init_robot(robot[i]);
                // int path_connected = bfs_find_berth(robot[i].x, robot[i].y, berth[robot[i].berth].x, berth[robot[i].berth].y, i);
                int best_berth = find_nearst_berth(robot[i].x, robot[i].y, i);
                // if // 如果当前港口堵塞，转移到另一个港口
                if(!robot[i].robot_find_berth_move.empty())
                	v.push_back(Robot_ans(robot[i].x, robot[i].y, robot[i].robot_find_berth_move.top()));
                else
                	v.push_back(Robot_ans(robot[i].x, robot[i].y, -1));
            }
        }
    }
    vector<Robot_ans> ans = pz_judge(v);
    for(int i = 0; i < robot_num; i++){
        int move = ans[i].move;
        if(move == -1) continue;
        if(ans[i].change != 0){
            if(ans[i].change == 1){
                Init_robot(robot[i]);
                int next_x = ans[i].x + robot_move_x[move];
                int next_y = ans[i].y + robot_move_y[move];
                ch_copy[next_x][next_y] = '*';
                ch_copy[ans[i].x][ans[i].y] = '*';
                if(robot[i].goods == 0){
                    auto it = cp.find(i);
                    if(it != cp.end()) ch_copy[cp[i].x][cp[i].y] = ch[cp[i].x][cp[i].y];
                    Point bestPoint = bfs_find_goods(robot[i].x, robot[i].y, i, zhen);
                    if(bestPoint.x != -1){
	                    ch_copy[bestPoint.x][bestPoint.y] = char(i + '0');
	                    cp[i] = bestPoint;
                    }
                }
                else{
                    // bfs_find_berth(robot[i].x, robot[i].y, berth[robot[i].berth].x, berth[robot[i].berth].y, i);
                    int best_berth = find_nearst_berth(robot[i].x, robot[i].y, i);
                }
                ch_copy[next_x][next_y] = ch[next_x][next_y];
                ch_copy[ans[i].x][ans[i].y] = ch[ans[i].x][ans[i].y];
            }
            continue;
        }
        int next_x = ans[i].x + robot_move_x[move];
        int next_y = ans[i].y + robot_move_y[move];
        if(ch[next_x][next_y] == '*' || ch[next_x][next_y] == '#'){
            cerr << next_x << ' ' << next_y << ' ' << ch_copy[next_x][next_y] << ' ' << robot[i].goods << endl;
        }
        printf("move %d %d\n", i, move);
        // cerr << next_x << ' ' << next_y << ' ' << ch_copy[next_x][next_y] << ' ' << robot[i].goods << ch[next_x][next_y] << endl;
        robot[i].dis = robot[i].dis + 1;
        if(robot[i].goods == 0){
        	// cerr << "move " << move << endl;
            robot[i].robot_find_goods_move.pop();
            int next_x = robot[i].x + robot_move_x[move];
            int next_y = robot[i].y + robot_move_y[move];
            if(ch_copy[next_x][next_y] == 'g' || ch_copy[next_x][next_y] == char(i + '0')){
                printf("get %d\n", i);
                // cerr << "get" << endl;
                ch_copy[next_x][next_y] = ch[next_x][next_y];
                ch_copy[cp[i].x][cp[i].y] = ch[cp[i].x][cp[i].y];
                cp.erase(i);
                robot[i].dis = 0;
            }
        }
        else{
            robot[i].robot_find_berth_move.pop();
            int next_x = robot[i].x + robot_move_x[move];
            int next_y = robot[i].y + robot_move_y[move];
            if(ch[next_x][next_y] == 'B'){
                printf("pull %d\n", i);
                berth[robot[i].berth].goods_num = berth[robot[i].berth].goods_num + 1;
                robot[i].dis = 0;
            }
        }
    }
}

vector<Robot_ans> pz_judge(vector<Robot_ans> v){
    vector<Robot_ans> ans;
    for(int i = 0; i < v.size(); i++){
        if(v[i].move == -1){
            ans.push_back(Robot_ans(v[i].x, v[i].y, -1));
            continue;
        }
        int x1 = v[i].x + robot_move_x[v[i].move];
        int y1 = v[i].y + robot_move_y[v[i].move];
        for(int j = 0; j < v.size(); j++){
            if(v[j].move == -1) continue;
            if(i == j) continue;
            // if(v[j].change != 0) continue;
            int x2 = v[j].x + robot_move_x[v[j].move];
            int y2 = v[j].y + robot_move_y[v[j].move];
            // 机器人抢占同一个点, 机器人互换位置
            if((x1 == x2 && y1 == y2) || ((x1 == v[j].x && y1 == v[j].y) && (x2 == v[i].x && y2 == v[i].y))){
                if(v[i].change == 0) v[i].change = 1;
                if(v[j].change == 0) v[j].change = 2;
                // break;
            }
        }
    }
    for(int i = 0; i < v.size(); i++){
    	if(v[i].move == -1) continue;
    	int x1 = v[i].x + robot_move_x[v[i].move];
        int y1 = v[i].y + robot_move_y[v[i].move];
    	for(int j = 0; j < v.size(); j++){
    		if(i == j) continue;
	    	if(x1 == v[j].x && y1 == v[j].y && (v[j].move == -1 || v[j].change != 0)){
	    		if(v[i].change == 0) v[i].change = 1;
	            // break;
	    	}
	    }
    }
    return v;
}

int tmp_map_dis[200][200];
// 在初始化的时候计算地图上任意两个点之间的距离
void cal_dis(int x, int y){
	memset(Init_bfs_vist, 0, sizeof(Init_bfs_vist));
	memset(tmp_map_dis, -1, sizeof(tmp_map_dis));
	queue<Point> q;
	q.push(Point(x, y, 0, 0));
	Init_bfs_vist[x][y] = 1;
	while(!q.empty()){
		Point now = q.front();
		tmp_map_dis[now.x][now.y] = now.dis;
		// if(now.x == ux && now.y == uy) return now.dis;
		q.pop();
		for(int i = 0; i < 4; i++){
			int next_x = now.x + robot_move_x[i];
            int next_y = now.y + robot_move_y[i];
            if(check(next_x, next_y) == -1) continue;
            if(Init_bfs_vist[next_x][next_y] == 1) continue;
            Point next = Point(next_x, next_y);
            next.dis = now.dis + 1;
            q.push(next);
            Init_bfs_vist[next.x][next.y] = 1;
		}
	}
}

void Init_map_dis(){
	for(int i = 0; i < berth_num; i++){
		cal_dis(berth[i].x, berth[i].y);
		for(int j = 0; j < N; j++){
			for(int k = 0; k < N; k++){
				map_dis[j][k][i] = tmp_map_dis[j][k];
			}
		}
	}
}


void read_robot(){
	for(int i = 0; i < robot_num; i ++)
    {
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &robot[i].status);
    }
}