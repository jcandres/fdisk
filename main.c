#include <stdio.h>
#include <stdlib.h>
#include "console.h"

int getkey(){
        return (char)getchar();
}

int main() {
        int x, y;
        x=y=10;
        gotoxy(20,20);
        printf("FFFFFFFFF");
        showcursor(0);
        //clear(21, 20);

        //msleep(1000000);
        int k;
        while (k = waitkey("kaskask? ")) {
                if (k==65) y--;
                if (k==66) y++;
                if (k==67) x++;
                if (k==68) x--;
                if (k=='q') break;

                //int k = 1;//waitkey("");// rand() % 99;
                clrscr();
                gotoxy(x, y);
                setrgb(rand() %16,rand() %16 );
                printf("%i", k);


        }
        setrgb(-1, -1);
        clrscr();
        printf("EXXIT");
        waitkey("");
}
