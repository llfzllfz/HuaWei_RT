#ifndef BERTH_H
#define BERTH_H

extern const int berth_num;

struct Berth
{
    int x, y; // 当前坐标
    int transport_time; //运输时间
    int loading_speed; //装载速度
    int goods_num; //港口货物量
    int efficiency; //单词满载运输的效率
    Berth(){}
    Berth(int x, int y, int transport_time, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> transport_time = transport_time;
        this -> loading_speed = loading_speed;
    }
};

extern Berth berth[10 + 10];

#endif