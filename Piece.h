//
// Created by Administrator on 2019/9/24 0024.
//

#ifndef TETRIS_PIECE_H
#define TETRIS_PIECE_H

#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

/* 交换a和b
 * 用于数组交换
 * */
void swap(int &a, int &b){
    int t=a;
    a = b;
    b = t;
}

/* 得到一个(min,max)区间的随机整数*/

int getrand(int min, int max)
{
    return(min+rand()%(max-min+1));
}



class Piece {
public:
    int score;     //得分
    int shape;     //表示当前方块的形状
    int next_shape;     //表示下一个方块的形状

    int head_x;        //当前方块首个box的位置，标记位置
    int head_y;

    int size_h;        //当前方块的size
    int size_w;

    int next_size_h;     //下一个方块的size
    int next_size_w;

    int box_shape[4][4];    //当前方块的shpe数组 4x4
    int next_box_shape[4][4];     //下一个方块的shpe数组 4x4

    int box_map[30][45];     //用来标记游戏框内的每个box

    bool game_over;     //游戏结束的标志

public:
    void initial();     //初始化函数
    void set_shape(int &cshape, int box_shape[][4],int &size_w, int & size_h);     //设置方块形状

    void score_next();     //显示下一个方块的形状以及分数
    void judge();     //判断是否层满
    void move();    //移动函数 通过 ← → ↓ 控制
    void rotate();    //旋转函数
    bool isaggin();    //判断下一次行动是否会越界或者重合
    bool exsqr(int row); //判断当前行是否空

};


#endif //TETRIS_PIECE_H
