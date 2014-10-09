#include <curses.h>
#include "dbg.h" 

int main() {
        initscr();
        raw();
        noecho();

        keypad(stdscr, TRUE);
        move(20,20);
        printw("KASKAS");
        box(stdscr, ACS_VLINE, ACS_HLINE);

        while (1) {
                
                int k = getch();

                wclear(stdscr);
                printw("%c", k);

                refresh();

                if (k == 27) //esc
                        break;
        }


        endwin();
}
