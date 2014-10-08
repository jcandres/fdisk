#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "console.h"

#ifndef OS_Windows
#include <sys/ioctl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#endif

void resetall() {
	setrgb(COLOR_RESET, COLOR_RESET);
	clrscr();
	gotoxy(0, 0);
	showcursor(1);
}

#ifdef OS_Windows
WORD COLOR_DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
HANDLE hConsole;// = GetStdHandle(STD_OUTPUT_HANDLE);

CONSOLE_SCREEN_BUFFER_INFO getsize() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return csbi;
}
#endif
void init(){
        #ifdef OS_Windows
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        #endif
}

int getwidth() {
#ifdef OS_Windows
	CONSOLE_SCREEN_BUFFER_INFO size = getsize();
	return size.srWindow.Right - size.srWindow.Left;
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
#endif
}

int getheight() {
#ifdef OS_Windows
	CONSOLE_SCREEN_BUFFER_INFO size = getsize();
	return size.srWindow.Bottom - size.srWindow.Top;
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_row;
#endif
}

void clrscr() {
#ifdef OS_Windows
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
#else
	for(int x = 0; x <= getwidth(); x++) {
		for(int y = 0; y <= getheight(); y++) {
			gotoxy(x, y);
			printf(" ");
		}
	}
#endif
        gotoxy(0,0);
}

void clearx(int y, int offset) {
	int start = 0, end = getwidth();
	if(offset < 0) {
		// from 0 to WIDTH-ABS(OFFSET)
		start = 0;
		end = getwidth() + offset;
	} else if(offset > 0) {
		// from OFFSET to WIDTH
		start = offset;
		end = getwidth();
	}

	for(int x = start; x <= end; x++) {
		gotoxy(x, y);
		printf(" ");
	}
}

void cleary(int x, int offset) {
	int start = 0, end = getheight();
	if(offset < 0) {
		// from 0 to HEIGHT-ABS(OFFSET)
		start = 0;
		end = getheight() + offset;
	} else if(offset > 0) {
		// from OFFSET to HEIGHT
		start = offset;
		end = getheight();
	}

	for(int y = start; y <= end; y++) {
		gotoxy(x, y);
		printf(" ");
	}
}

void clear(int x, int y){
        gotoxy(x,y);
        printf(" ");
}

void msleep(int time) {
#ifdef OS_Windows
	Sleep(time);
#else
	clock_t goal = time + clock();
	int i = 0;
	while (goal > clock()) { i++; };
#endif
}

void gotoxy(int x, int y) {
#ifdef OS_Windows
	COORD point;
	point.X = x;
	point.Y = y;
	SetConsoleCursorPosition(hConsole, point);
#else
	printf("\033[%d;%dH", y + 1, x + 1);
#endif
}

/*void wherexy(int &x, int &y) {
#ifdef OS_Windows
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	x = csbi.dwCursorPosition.X;
	y = csbi.dwCursorPosition.Y;
#else
	x = 0;
	y = 0;
#endif
}*/

void setbg(int color) {
	int c = color & 7;
	int bright = (color & COLOR_BRIGHT) == COLOR_BRIGHT;

#ifdef OS_Windows
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	if(color == COLOR_RESET)
		c = (COLOR_DEFAULT & 240) | (csbi.wAttributes & 15);
	else {
		if(bright) c += 8;

		c = c << 4;
		c |= (csbi.wAttributes & 15);
	}

	SetConsoleTextAttribute(hConsole, c);
#else
	if(color == COLOR_RESET)
		printf("\033[49m");
	else
		printf("\033[%d;%dm", bright ? 0 : 1, 40 + c);
#endif
}
void setfg(int color) {
	int c = color & 7;
	int bright = (color & COLOR_BRIGHT) == COLOR_BRIGHT;

#ifdef OS_Windows
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	if(color == COLOR_RESET)
		c = (COLOR_DEFAULT & 15)  | (csbi.wAttributes & 240);
	else {
		if(bright) c += 8;

		c |= (csbi.wAttributes & 240);
	}

	SetConsoleTextAttribute(hConsole, c);
#else
	if(color == COLOR_RESET)
		printf("\033[39m");
	else
		printf("\033[%d;%dm", bright ? 0 : 2, 30 + c);
#endif
}

void setrgb(int fg, int bg) {
	setfg(fg);
	if(bg != -2)
		setbg(bg);
}

void showcursor(int show) {
#ifdef OS_Windows
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = show ? TRUE : FALSE;
	SetConsoleCursorInfo(hConsole, &info);
#else
	printf(show ? "\e[?25h" : "\e[?25l");
#endif
}

int waitkey(const char* prompt) {
#ifdef OS_Windows
	DWORD        mode;
	HANDLE       hstdin;
	INPUT_RECORD inrec;
	DWORD        count;

	/* Set the console mode to no-echo, raw input, */
	/* and no window or mouse events.              */
	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hstdin == INVALID_HANDLE_VALUE
		|| !GetConsoleMode(hstdin, &mode)
		|| !SetConsoleMode(hstdin, 0))
		return 0;

	/* Instruct the user */
	WriteConsole(
		hConsole,
		prompt,
		strlen(prompt),
		&count,
		NULL
		);

	FlushConsoleInputBuffer(hstdin);

	/* Get a single key RELEASE */
	do ReadConsoleInput(hstdin, &inrec, 1, &count);
	while ((inrec.EventType != KEY_EVENT) || !inrec.Event.KeyEvent.bKeyDown);

	/* Restore the original console mode */
	SetConsoleMode(hstdin, mode);

	return inrec.Event.KeyEvent.wVirtualKeyCode;
#else
	struct termios savedState, newState;
	int c;

	if (-1 == tcgetattr(STDIN_FILENO, &savedState))
		return EOF;     /* error on tcgetattr */

	newState = savedState;

	/* disable canonical input and disable echo.  set minimal input to 1. */
	newState.c_lflag &= ~(ECHO | ICANON);
	newState.c_cc[VMIN] = 1;

	if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &newState))
		return EOF;     /* error on tcsetattr */

	c = getchar();      /* block (without spinning) until we get a keypress */

	/* restore the saved state */
	if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &savedState))
		return EOF;     /* error on tcsetattr */

	return c;
#endif
}


