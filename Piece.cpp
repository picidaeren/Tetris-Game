//
// Created by Administrator on 2019/9/24 0024.
//

#include "Piece.h"

//设置方块形状
void Piece::set_shape(int &cshape, int box_shape[][4],int &size_w,int &size_h)
{

    /*首先将用来表示的4x4数组初始化为0*/
    int i,j;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            box_shape[i][j]=0;

    /*设置7种初始形状并设置它们的size*/
    switch(cshape)
    {
        case 0:
            size_h=1;
            size_w=4;
            box_shape[0][0]=1;
            box_shape[0][1]=1;
            box_shape[0][2]=1;
            box_shape[0][3]=1;
            break;
        case 1:
            size_h=2;
            size_w=3;
            box_shape[0][0]=1;
            box_shape[1][0]=1;
            box_shape[1][1]=1;
            box_shape[1][2]=1;
            break;
        case 2:
            size_h=2;
            size_w=3;
            box_shape[0][2]=1;
            box_shape[1][0]=1;
            box_shape[1][1]=1;
            box_shape[1][2]=1;
            break;
        case 3:
            size_h=2;
            size_w=3;
            box_shape[0][1]=1;
            box_shape[0][2]=1;
            box_shape[1][0]=1;
            box_shape[1][1]=1;
            break;

        case 4:
            size_h=2;
            size_w=3;
            box_shape[0][0]=1;
            box_shape[0][1]=1;
            box_shape[1][1]=1;
            box_shape[1][2]=1;
            break;

        case 5:
            size_h=2;
            size_w=2;
            box_shape[0][0]=1;
            box_shape[0][1]=1;
            box_shape[1][0]=1;
            box_shape[1][1]=1;
            break;

        case 6:
            size_h=2;
            size_w=3;
            box_shape[0][1]=1;
            box_shape[1][0]=1;
            box_shape[1][1]=1;
            box_shape[1][2]=1;
            break;
    }

    //设置完形状以后初始化方块的起始位置
    head_x=game_win_width/2;
    head_y=1;

    //如果刚初始化就重合了，游戏结束~
    if(isaggin())    /* GAME OVER ! */
        game_over=true;

}


//方块旋转
void Piece::rotate() {
    int temp[4][4]={0};  //临时变量
    int temp_piece[4][4]={0};  //备份用的数组
    int i,j,tmp_size_h,tmp_size_w;

    tmp_size_w=size_w;
    tmp_size_h=size_h;

    for(int i=0; i<4;i++)
        for(int j=0;j<4;j++)
            temp_piece[i][j]=box_shape[i][j];    //备份一下当前的方块，如果旋转失败则返回到当前的形状


    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            temp[j][i]=box_shape[i][j];    //斜对角线对称
    i=size_h;
    size_h=size_w;
    size_w=i;
    for(i=0;i<size_h;i++)
        for(j=0;j<size_w;j++)
            box_shape[i][size_w-1-j]=temp[i][j];    //左右对称


    /*如果旋转以后重合，则返回到备份的数组形状*/
    if(isaggin()){
        for(int i=0; i<4;i++)
            for(int j=0;j<4;j++)
                box_shape[i][j]=temp_piece[i][j];
        size_w=tmp_size_w;    //记得size也要变回原来的size
        size_h=tmp_size_h;
    }

        /*如果旋转成功，那么在屏幕上进行显示*/
    else{
        for(int i=0; i<4;i++)
            for(int j=0;j<4;j++){
                if(temp_piece[i][j]==1){
                    mvwaddch(game_win,head_y+i,head_x+j,' ');    //移动到game_win窗口的某个坐标处打印字符
                    wrefresh(game_win);
                }
            }
        for(int i=0; i<size_h;i++)
            for(int j=0;j<size_w;j++){
                if(this->box_shape[i][j]==1){
                    mvwaddch(game_win,head_y+i,head_x+j,'#');
                    wrefresh(game_win);
                }
            }

    }


}


//判断是否越界和重合，每次移动或旋转之后要进行判断的函数，
// 函数返回真则不能行动，返回假则可以进行下一步。
bool Piece::isaggin() {
    for(int i=0;i<size_h;i++)
        for(int j=0;j<size_w;j++){
            if(box_shape[i][j]==1){
                if(head_y+i > game_win_height-2)    //下面出界
                    return true;
                if(head_x+j > game_win_width-2 || head_x+i-1<0)    //左右出界
                    return true;
                if(box_map[head_y+i][head_x+j]==1)    //与已占用的box重合
                    return true ;
            }
        }
    return false;


}


//判断层满函数
void Piece::judge(){
    int i,j;
    int line=0;    //用来记录层满的行数
    bool full;
    for(i=1;i<game_win_height-1;i++){ //除去边界
        full=true;
        for(j=1;j<game_win_width-1;j++){
            if(box_map[i][j]==0) //存在未被占用的box
                full=false; //说明本层未满
        }
        if(full){ //如果该层满
            line++; //行满+1
            score+=50; //加分~
            for(j=1;j<game_win_width-1;j++)
                box_map[i][j]=0; //把该层清空(标记为未被占用)
        }
    }

    /*上面判断完后 看line的值,如果非 0 说明有层已满需要进行消除*/
    if(line!=0){
        for(i=game_win_height-2;i>=2;i--){
            int s=i;
            if(exsqr(i)==0){
                while(s>1 && exsqr(--s)==0);    //查找存在方块的行，将其下移
                for(j=1;j<game_win_width-1;j++){
                    box_map[i][j]=box_map[s][j]; //上层下移
                    box_map[s][j]=0;    //上层清空
                }
            }
        }

        /*清空和移动标记完成以后就要屏幕刷新了，重新打印game_win*/
        for(int i=1;i<game_win_height-1;i++)
            for(int j=1;j<game_win_width-1;j++){
                if(box_map[i][j]==1){
                    mvwaddch(game_win,i,j,'#');
                    wrefresh(game_win);
                }
                else{
                    mvwaddch(game_win,i,j,' ');
                    wrefresh(game_win);
                }
            }
    }
}

//判断当前行是否是空行
bool Piece::exsqr(int row) {
    for(int j=1;j<game_win_width-1;j++)
        if(box_map[row][j]==1)
            return true;
    return false;
}

//显示下一个方块的形状以及分数
void Piece::score_next(){
    score+=10;
    judge();

    mvwprintw(score_win, hint_win_height/2,hint_win_width/2-2,"%d",score);
    wrefresh(score_win);


    set_shape(next_shape,box_shape,size_w,size_h);

    this->next_shape=getrand(0,6);
    set_shape(next_shape,next_box_shape,next_size_w,next_size_h);


    for(int i =1;i<hint_win_height-1;i++)
        for(int j=1;j<hint_win_width-1;j++){
            mvwaddch(hint_win, i, j,' ');
            wrefresh(hint_win);
        }
    for(int i =0;i<4;i++)
        for(int j=0;j<4;j++)
            if(next_box_shape[i][j]==1){
                mvwaddch(hint_win,(hint_win_height-size_h)/2+i,(hint_win_width-size_w)/2+j,'#');
                wrefresh(hint_win);
            }
}

//移动函数
void Piece::move() {
    fd_set set;  //定义select函数中需要的结构体
    FD_ZERO(&set);
    FD_SET(0, &set);

    //定义超时时间
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec= 500000;


    if (select(1, &set, NULL, NULL, &timeout) == 0){
        head_y++;
        if(isaggin()){
            head_y--;
            for(int i=0;i<size_h;i++)
                for(int j=0;j<size_w;j++)
                    if(box_shape[i][j]==1)
                        box_map[head_y+i][head_x+j]=1;
            score_next();
        }
        else{
            for(int i=size_h-1; i>=0;i--)
                for(int j=0;j<size_w;j++){
                    if(this->box_shape[i][j]==1){
                        mvwaddch(game_win,head_y-1+i,head_x+j,' ');
                        mvwaddch(game_win,head_y+i,head_x+j,'#');

                    }
                }
            wrefresh(game_win);
        }

    }

    if (FD_ISSET(0, &set)) {
        while ((key = getch()) == -1) ;

        if(key==KEY_LEFT){
            head_x--;
            if(isaggin())
                head_x++; //undo
            else{
                for(int i=0; i<size_h;i++)
                    for(int j=0;j<size_w;j++){
                        if(this->box_shape[i][j]==1){
                            mvwaddch(game_win,head_y+i,head_x+j+1,' ');
                            mvwaddch(game_win,head_y+i,head_x+j,'#');

                        }
                    }
                wrefresh(game_win);
            }
        }

        if(key==KEY_RIGHT){
            head_x++;
            if(isaggin())
                head_x--;
            else{
                for(int i=0; i<size_h;i++)
                    for(int j=size_w-1;j>=0;j--){
                        if(this->box_shape[i][j]==1){
                            mvwaddch(game_win,head_y+i,head_x+j-1,' ');
                            mvwaddch(game_win,head_y+i,head_x+j,'#');

                        }
                    }
                wrefresh(game_win);
            }
        }

        if(key==KEY_DOWN){
            head_y++;
            if(isaggin()){
                head_y--;
                for(int i=0;i<size_h;i++)
                    for(int j=0;j<size_w;j++)
                        if(box_shape[i][j]==1)
                            box_map[head_y+i][head_x+j]=1;

                score_next();

            }
            else{
                for(int i=size_h-1; i>=0;i--)
                    for(int j=0;j<size_w;j++){
                        if(this->box_shape[i][j]==1){
                            mvwaddch(game_win,head_y-1+i,head_x+j,' ');
                            mvwaddch(game_win,head_y+i,head_x+j,'#');

                        }
                    }
                wrefresh(game_win);
            }
        }

        if(key==KEY_UP)
            rotate();

        if(head_x+size_w+1>game_win_width)
            head_x=game_win_width-size_w-1;
        if(head_x<1)
            head_x=1;
    }
}


//初始化函数
void Piece::initial() {
    score=0;
    game_over=false;
    for(int i =0;i<game_win_height;i++)
        for(int j=0;j<game_win_width;j++){
            if(i==0 || i==game_win_height-1 || j==0 || j==game_win_width-1){
                box_map[i][j]=1;
            }
            else
                box_map[i][j]=0;
        }

    srand((unsigned)time(0));
    shape=getrand(0,6);
    set_shape(shape,box_shape,size_w,size_h);

    next_shape=getrand(0,6);
    set_shape(next_shape,next_box_shape,next_size_w,next_size_h);

    for(int i =0;i<4;i++)
        for(int j=0;j<4;j++)
            if(next_box_shape[i][j]==1){
                mvwaddch(hint_win,(hint_win_height-size_h)/2+i,(hint_win_width-size_w)/2+j,'#');
                wrefresh(hint_win);
            }


    mvwprintw(score_win, hint_win_height/2,hint_win_width/2-2,"%d",score);
    wrefresh(score_win);
}

//创建一个窗口，返回窗口的结构体 WINDOW*
WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win,0,0);
    wrefresh(local_win);
    return local_win;
}

//销毁一个窗口
void destory_win(WINDOW *local_win)
{
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(local_win);
    delwin(local_win);
}