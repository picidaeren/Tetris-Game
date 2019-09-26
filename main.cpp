#include <iostream>
#include <Piece.h>


int main() {
	
	//配置屏幕属性
    initscr();
    //raw();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr,TRUE);
	//当开启 keypad 後, 可以使用键盘上的一些特殊字元, 如上下左右>等方向键,
	//curses 会将这些特殊字元转换成 curses.h 内定义的一些特殊键. 
	//这些定义的特殊键通常以 KEY_ 开头.

    refresh();

    game_win = create_newwin(game_win_height, game_win_width, 0,0);
    wborder(game_win, '*', '*', '*', '*', '*', '*', '*', '*');
    wrefresh(game_win);

    hint_win = create_newwin(hint_win_height, hint_win_width, 0, game_win_width+10);
    mvprintw(0, game_win_width+10+2,"%s","Next");
    refresh();

    score_win = create_newwin(hint_win_height, hint_win_width, 20, game_win_width+10);
    mvprintw(20, game_win_width+10+2,"%s","Score");
    refresh();

    Piece* pp = new Piece;
    pp->initial();


    while(1)
    {
        pp->move();
        if(pp->game_over)
            break;
    }

    destory_win(game_win);
    destory_win(hint_win);
    destory_win(score_win);
    delete pp;
    system("clear");

    int row,col;
    getmaxyx(stdscr,row,col);
    mvprintw(row/2,col/2 ,"%s","GAMER OVER ! \n ");
    mvprintw(row/2+2,col/2-2 ,"%s","Wait 5s to return tthe erminal ! \n ");
    refresh();

    sleep(5);
    endwin();
    return 0;
}