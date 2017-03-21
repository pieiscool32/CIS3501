#include <ncurses.h>
#include <string.h>
#include <iostream>

using namespace std;

int main() {
    initscr();
    printw("Hello World !!!");
    refresh();
    getch();
    endwin();
    
    return 0;
}
