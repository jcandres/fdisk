#include <stdio.h>
#include <stdlib.h>
#include "console.h"

int getkey(){
        return (char)getch();
}

int main() {
        init();
        int x, y;
        x=y=10;
        gotoxy(20,20);
        printf("FFFFFFFFF");
        showcursor(0);

        for (int i=0; i<getwidth(); i++)
                for (int j=0; j<getheight(); j++){
                        setbg(i+j);
                        clear(i,j);
                        }

        int k;
        while ((k = waitkey(""))) {
                if (k==65) y--;
                if (k==66) y++;
                if (k==67) x++;
                if (k==68) x--;
                if (k=='q') break;

                //int k = 1;//waitkey("");// rand() % 99;
                clrscr();
                gotoxy(x, y);
                setrgb(rand() %16,rand() %16 );
                printf("%i %c", k,k);


        }
        setrgb(-1, -1);
        clrscr();
        printf("EXXIT");
        waitkey("");
}
