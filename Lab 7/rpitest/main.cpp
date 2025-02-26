#include "pigpio.h" 
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

int main()
{
	initscr();
	noecho();
	cbreak();
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);

	if(gpioInitialise() < 0)
	{
		return 1;
	}
	
	gpioSetMode(23, PI_INPUT);
	while(true)
	{
		int ch = getch();
		if(ch == 'q')
		{
			refresh();
			break;
		}
		int in = gpioRead(23);
		std::cout << std::left <<  in << std::endl;
	}
	endwin();
	gpioTerminate();
}
