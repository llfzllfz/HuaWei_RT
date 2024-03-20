#include "robot.h"
#include "berth.h"
#include "map.h"
#include <bits/stdc++.h>
using namespace std;

//机器人
Robot robot[10 + 10];
// 机器人移动方向，右，左，上，下
int robot_move_x[] = {0, 0, -1, 1};
int robot_move_y[] = {1, -1, 0, 0};
//寻找货物系统参数
// int robot_visit_find_goods[200+1][200+1];
// int fax_find_goods[200][200];
// int fay_find_goods[200][200];
// int famove_find_goods[200][200];
//寻找港口系统参数
int robot_visit_find_berth[200+1][200+1];
int fax_find_berth[200][200];
int fay_find_berth[200][200];
int famove_find_berth[200][200];

unordered_map<int, Point> cp;
// 初始化到港口距离参数
int Init_bfs_vist[200][200];
int map_dis[200][200][10];
double pow_index_base = 1.0;
int extra_dis = 0;
double g2b_alpha = 0;
double b2g_alpha = 0;
vector<double> containers;
mt19937 g_rand(2024);
vector<int> move_v;

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
    x.best_goods.clear();
}

void bfs_find_goods(int robot_index, int zhen, double pow_index){
    int x = robot[robot_index].x;
    int y = robot[robot_index].y;
    memset(robot[robot_index].robot_visit_find_goods, 0, sizeof(robot[robot_index].robot_visit_find_goods));
    memset(robot[robot_index].fax_find_goods, -1, sizeof(robot[robot_index].fax_find_goods));
    memset(robot[robot_index].fay_find_goods, -1, sizeof(robot[robot_index].fay_find_goods));
    memset(robot[robot_index].famove_find_goods, -1, sizeof(robot[robot_index].famove_find_goods));
    Point bestPoint = Point(-1, -1, 0, 0);
    queue<Point> q;
    q.push(Point(x, y, 0, 0));
    robot[robot_index].robot_visit_find_goods[x][y] = 1;
    while(!q.empty()){
        Point now = q.front();
        q.pop();
        for(int i = 0; i < 4; i++){
        // for(const auto& i: move_v){
        // for(int idx = 0; idx < 4; idx++){
            // int i = move_v[idx];
            int next_x = now.x + robot_move_x[i];
            int next_y = now.y + robot_move_y[i];
            if(check(next_x, next_y) == -1) continue;
            if(robot[robot_index].robot_visit_find_goods[next_x][next_y] == 1) continue;
            Point next = Point(next_x, next_y);
            next.dis = now.dis + 1;
            q.push(next);
            robot[robot_index].robot_visit_find_goods[next.x][next.y] = 1;
            robot[robot_index].fax_find_goods[next.x][next.y] = now.x;
            robot[robot_index].fay_find_goods[next.x][next.y] = now.y;
            robot[robot_index].famove_find_goods[next.x][next.y] = i;
            if(ch_copy[next_x][next_y] == 'g'){
                if(next.dis > (1000 - (zhen - goods_time_mp[int2str(next_x, next_y)] + 1))) continue;
                int value = goods_value_mp[int2str(next_x, next_y)];
                if(bestPoint.x == -1){
                    // 锁定最近距离的货物到货物最近距离的港口
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
                            bestPoint.pre_dis = robot[robot_index].dis;
                        }
                    }
                    if(double(bestPoint.dis + bestPoint.pre_dis) < containers[robot_index] * double(bestPoint.goods2berth_dis))
                        robot[robot_index].best_goods.push_back(bestPoint);
                }
                if(bestPoint.x == -1) continue;
                bestPoint.x = -1;
                bestPoint.goods2berth_dis = 10000;
            }
        }
    }
    sort(robot[robot_index].best_goods.begin(), robot[robot_index].best_goods.end(), sort_best_goods);
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
    return best_berth;
}

int select2best_point(Point x1, Point x2, double pow_index){
    if(x1.x == x2.x && x1.y == x2.y){
        int x1_dis = x1.dis + x1.goods2berth_dis + x1.pre_dis + extra_dis;
        int x2_dis = x2.dis + x2.goods2berth_dis + x2.pre_dis + extra_dis;
        int x1_value = goods_value_mp[int2str(x1.x, x1.y)];
        int x2_value = goods_value_mp[int2str(x2.x, x2.y)];
        // if(pow(double(x1_value), pow_index) / double(x1_dis) + g2b_alpha * x1.goods2berth_dis - b2g_alpha * (x1.dis + x1.pre_dis) < pow(double(x2_value), pow_index) / double(x2_dis) + g2b_alpha * x2.goods2berth_dis - b2g_alpha * (x2.dis + x2.pre_dis)){
        //     return 1;
        // }
        if(xjb_func(x1) < xjb_func(x2)) return 1;
        else return 2;
    }
    return 0;
}

int sort_best_goods(Point x1, Point x2){
    return xjb_func(x1) > xjb_func(x2);
    // int x1_dis = x1.dis + x1.goods2berth_dis + x1.pre_dis + extra_dis;
    // int x2_dis = x2.dis + x2.goods2berth_dis + x2.pre_dis + extra_dis;
    // int x1_value = goods_value_mp[int2str(x1.x, x1.y)];
    // int x2_value = goods_value_mp[int2str(x2.x, x2.y)];
    // double pow_index = pow_index_base;
    // return pow(double(x1_value), pow_index) / double(x1_dis) + g2b_alpha * x1.goods2berth_dis - b2g_alpha * (x1.dis + x1.pre_dis) > pow(double(x2_value), pow_index) / double(x2_dis) + g2b_alpha * x2.goods2berth_dis - b2g_alpha * (x2.dis + x2.pre_dis);
}

double xjb_func(Point x1){
    int g2b = x1.goods2berth_dis;
    int r2g = x1.dis + x1.pre_dis;
    int value = goods_value_mp[int2str(x1.x, x1.y)];
    double pow_index = pow_index_base;
    // double alpha = max(1.0, (r2g - g2b) / double(r2g));
    // return pow(double(value), pow_index) / double(r2g + g2b) * (1 - alpha);
    // return pow(double(value), pow_index) / double(r2g + g2b) + g2b_alpha * g2b;
    // cerr << goods_density[x1.x][x1.y] <<  ' ' << goods.size() - goods_vector_index << endl;
    return pow(double(value), pow_index) / double(r2g + g2b) + (goods_density[x1.x][x1.y] * g2b_alpha / (goods.size() - goods_vector_index));
    // return pow(double(value), pow_index) * goods_density[x1.x][x1.y] * g2b_alpha / (double(r2g + g2b) * (goods.size() - goods_vector_index));
}

void cal_best_path(int robot_index){
    int x = robot[robot_index].best_goods[0].x;
    int y = robot[robot_index].best_goods[0].y;
    while(x != robot[robot_index].x || y != robot[robot_index].y){
        int move = robot[robot_index].famove_find_goods[x][y];
        robot[robot_index].robot_find_goods_move.push(move);
        int now_xx = robot[robot_index].fax_find_goods[x][y];
        int now_yy = robot[robot_index].fay_find_goods[x][y];
        x = now_xx;
        y = now_yy;
    }
    robot[robot_index].berth = robot[robot_index].best_goods[0].berth;
}

// 机器人管理
void cal_robot(int zhen){
    // for(int i = 0; i < robot_num; i++){
    //     if(containers[i] > 2) continue;
    //     if(zhen < 3000) containers[i] = 2;
    //     if(zhen >= 3000 && zhen < 6000) containers[i] = 1.6;
    //     if(zhen >= 6000) containers[i] = 1.3;
    // }
    for(int i = 0; i < 4; i++) move_v.push_back(i);
    // shuffle(move_v.begin(), move_v.end(), g_rand);
    // if(zhen != 0 && zhen % 1000 == 0 && extra_dis == 100) {extra_dis -= 50;}
    // if(zhen != 0 && zhen % 500 == 0 && extra_dis == 0) {extra_dis += 50;}
    if(zhen != 0 && zhen % 1000 < 450) {g2b_alpha = 0;}
    if(zhen != 0 && zhen % 1000 > 450) {g2b_alpha = 0;}
    if(zhen != 0 && zhen % 1000 < 450) {b2g_alpha = 0;}
    if(zhen != 0 && zhen % 1000 > 450) {b2g_alpha = 0.003;}
    vector<thread> threads;
    for(int i = 0; i < robot_num; i++){
        Init_robot(robot[i]);
        if(robot[i].status == 0) continue;
        if(!robot[i].robot_find_goods_move.empty()) continue;
        if(robot[i].goods != 0) continue;
        Init_robot(robot[i]);
        threads.emplace_back(thread(bfs_find_goods, i, zhen, pow_index_base));
    }
    for(auto& t : threads){
        t.join();
    }
    // for(int i = 0; i < robot_num; i++){
    //     containers[i] = -1;
    // }
    int flg = 1;
    while(flg){
        flg = 0;
        for(int i = 0; i < robot_num; i++){
            if(robot[i].status == 0) continue;
            if(!robot[i].robot_find_goods_move.empty()) continue;
            if(robot[i].best_goods.empty()) continue;
            if(robot[i].goods != 0) continue;
            for(int j = 0; j < robot_num; j++){
                if(i == j) continue;
                if(robot[j].status == 0) continue;
                if(!robot[j].robot_find_goods_move.empty()) continue;
                if(robot[j].best_goods.empty()) continue;
                if(robot[j].goods != 0) continue;
                int select = select2best_point(robot[i].best_goods[0], robot[j].best_goods[0], pow_index_base);
                if(select == 0) continue;
                else if(select == 1) {
                    if(robot[i].best_goods.size() == 1 && robot[j].best_goods.size() != 1)
                        robot[j].best_goods.erase(robot[j].best_goods.begin());
                    else
                        robot[i].best_goods.erase(robot[i].best_goods.begin());
                    flg = 1;
                    break;
                }
                else if(select == 2) {
                    if(robot[j].best_goods.size() == 1 && robot[i].best_goods.size() != 1)
                        robot[i].best_goods.erase(robot[i].best_goods.begin());
                    else
                        robot[j].best_goods.erase(robot[j].best_goods.begin());
                    flg = 1;
                    break;
                }
            }
        }
    }
    for(int i = 0; i < robot_num; i++){
        if(robot[i].status == 0) continue;
        if(!robot[i].robot_find_goods_move.empty()) continue;
        if(robot[i].best_goods.empty()) continue;
        if(robot[i].goods != 0) continue;
        cal_best_path(i);
    }

    for(int i = 0; i < robot_num; i++){
        if(robot[i].best_goods.empty() && (goods.size() - goods_vector_index) > 10) containers[i] *= 2;
        // cerr << containers[i] << endl;
    }

    vector<Robot_ans> v;
    for(int i = 0; i < robot_num; i++){
        if(robot[i].status == 0) {
            v.push_back(Robot_ans(robot[i].x, robot[i].y, -1));
            Init_robot(robot[i]);
            continue;
        }
        if(robot[i].goods == 0){
            int move = -1;
            if(!robot[i].robot_find_goods_move.empty()){
                move = robot[i].robot_find_goods_move.top();
                // cerr << i << ' ' << move << robot[i].robot_find_goods_move.top() << endl;
            }
            v.push_back(Robot_ans(robot[i].x, robot[i].y, move));
        }
        else{
            if(!robot[i].robot_find_berth_move.empty()){
                int move = robot[i].robot_find_berth_move.top();
                v.push_back(Robot_ans(robot[i].x, robot[i].y, move));
            }
            else{
                Init_robot(robot[i]);
                int best_berth = find_nearst_berth(robot[i].x, robot[i].y, i);
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
        int next_x = ans[i].x + robot_move_x[move];
        int next_y = ans[i].y + robot_move_y[move];
        if(ch[next_x][next_y] == '*' || ch[next_x][next_y] == '#'){
            cerr << next_x << ' ' << next_y << ' ' << ch_copy[next_x][next_y] << ' ' << robot[i].goods << endl;
        }
        printf("move %d %d\n", i, move);
        robot[i].dis = robot[i].dis + 1;
        if(robot[i].goods == 0){
            robot[i].robot_find_goods_move.pop();
            int next_x = robot[i].x + robot_move_x[move];
            int next_y = robot[i].y + robot_move_y[move];
            if(ch_copy[next_x][next_y] == 'g' || ch_copy[next_x][next_y] == char(i + '0')){
                printf("get %d\n", i);
                ch_copy[next_x][next_y] = ch[next_x][next_y];
                robot[i].dis = 0;
                robot[i].goods_values = goods_value_mp[int2str(next_x, next_y)];
                // for(int i = 0; i < robot_num; i++){
                if(containers[i] <= 0) continue;
                if(zhen < 3000) containers[i] = 2;
                if(zhen >= 3000 && zhen < 6000) containers[i] = 1.6;
                if(zhen >= 6000) containers[i] = 1.3;
                // }
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
                berth[robot[i].berth].goods_values = berth[robot[i].berth].goods_values + robot[i].goods_values;
                if(containers[i] <= 0) continue;
                if(zhen < 3000) containers[i] = 2;
                if(zhen >= 3000 && zhen < 6000) containers[i] = 1.6;
                if(zhen >= 6000) containers[i] = 1.3;
            }
        }
    }
}

int find_pzset_move(vector<int> pz_set, vector<string> pz_v, int x, int y, int ux, int uy){
    int dx = (ux - x) == 0 ? 0 : ((ux - x) > 0 ? 1 : -1);
    int dy = (uy - y) == 0 ? 0 : ((uy - y) > 0 ? 1 : -1);
    int move = -1;
    for(int i = 0; i < 4; i++){
        if(robot_move_x[i] == dx || robot_move_y[i] == dy){
            auto it = find(pz_set.begin(), pz_set.end(), i);
            if(it != pz_set.end()) continue;
            int next_x = x + robot_move_x[i];
            int next_y = y + robot_move_y[i];
            if(check(next_x, next_y) == -1) {pz_set.push_back(i); continue;}
            auto itt = find(pz_v.begin(), pz_v.end(), int2str(next_x, next_y));
            if(itt != pz_v.end()) {pz_set.push_back(i); continue;}
            move = i;
        }
    }
    if(move != -1) return move;
    for(int i = 0; i < 4; i++){
        auto it = find(pz_set.begin(), pz_set.end(), i);
        if(it != pz_set.end()) continue;
        int next_x = x + robot_move_x[i];
        int next_y = y + robot_move_y[i];
        if(check(next_x, next_y) == -1) {pz_set.push_back(i); continue;}
        auto itt = find(pz_v.begin(), pz_v.end(), int2str(next_x, next_y));
        if(itt != pz_v.end()) {pz_set.push_back(i); continue;}
        move = i;
    }
    return move;
}

vector<Robot_ans> pz_judge(vector<Robot_ans> v){
    vector<string> pz_v;
    for(int i = 0; i < robot_num; i++){
        pz_v.push_back(int2str(robot[i].x, robot[i].y));
    }
    for(int i = 0; i < v.size(); i++){
        if(v[i].move == -1) continue;
        int x1 = v[i].x + robot_move_x[v[i].move];
        int y1 = v[i].y + robot_move_y[v[i].move];
        vector<int> pz_set;
        pz_set.clear();
        for(int j = 0; j < pz_v.size(); j++){
            if(i == j) continue;
            if(pz_v[j] == int2str(x1, y1)){
                pz_set.push_back(v[i].move);
                if(robot[i].goods == 0){
                    int ux = robot[i].best_goods[0].x;
                    int uy = robot[i].best_goods[0].y;
                    int move = find_pzset_move(pz_set, pz_v, v[i].x, v[i].y, ux, uy);
                    v[i].move = move;
                }
                else{
                    int tmp_berth = robot[i].berth;
                    int ux = berth[tmp_berth].x;
                    int uy = berth[tmp_berth].y;
                    int move = find_pzset_move(pz_set, pz_v, v[i].x, v[i].y, ux, uy);
                    v[i].move = move;
                }
                break;
            }
        }
        if(v[i].move == -1) continue;
        x1 = v[i].x + robot_move_x[v[i].move];
        y1 = v[i].y + robot_move_y[v[i].move];
        pz_v[i] = int2str(x1, y1);
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
    for(int i = 0; i < robot_num; i++) containers.push_back(2);
    // for(int i = 0; i < robot_num; i++){
    //     int tmp_dis = 10000;
    //     for(int j = 0; j < berth_num; j++){
    //         tmp_dis = min(tmp_dis, map_dis[robot[i].x][robot[i].y][j]);
    //     }
    //     robot[i].dis = tmp_dis;
    // }
}


void read_robot(){
	for(int i = 0; i < robot_num; i ++)
    {
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &robot[i].status);
    }
}