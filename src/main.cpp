#include "board.h"
#include <ncurses.h>
// #include <fmt/core.h>
#include <iostream>
#include <unistd.h>

#define K 1000 // usleep works in microseconds, so multiplying by K gives milliseconds

#define WIDTH 40
#define HEIGHT 20
#define SNAKE_COLOR 1
#define FRUIT_COLOR 2
#define BLOCK_SNAKE 3
#define BLOCK_FRUIT 4

/* TODO
 * Control issue? Seems to be hard to fix in ncurses
 * BUT if I figure it out I can maybe try to make tetris too
 */

auto main() -> int {

	// initializing ncurses
	initscr();

	if (LINES < HEIGHT || COLS < WIDTH) {
		std::cout << "Your terminal window is, somehow, too small.\n\
				For your own sake, and mine, make it bigger." << std::endl;
		std::exit(1);
	}

	// dynamic sizing calculation
	int width = COLS % WIDTH;
	int height = (LINES + 1) % HEIGHT;
	int screenSizeMultiple = std::min(width, height);
	screenSizeMultiple = (screenSizeMultiple == 0) ? 1 : screenSizeMultiple;
	screenSizeMultiple = 2; // only for debug

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);
	start_color();

	// colorpairs for the snake and fruit
	init_pair(SNAKE_COLOR, COLOR_GREEN, COLOR_BLACK);
	init_pair(FRUIT_COLOR, COLOR_RED, COLOR_BLACK);

	// block chars
	init_pair(BLOCK_SNAKE, COLOR_BLACK, COLOR_GREEN);
	init_pair(BLOCK_FRUIT, COLOR_BLACK, COLOR_RED);

	// strings to print
	std::string snake_string (screenSizeMultiple, ' ');
	std::string fruit_string (screenSizeMultiple, ' ');
	const char* snake_c = snake_string.c_str();
	const char* fruit_c = fruit_string.c_str();

	// new dynamic window size
	int window_height = HEIGHT * screenSizeMultiple;
	int window_width = WIDTH * screenSizeMultiple;

	// approximate center of screen
	int startx = (COLS - window_width)/2;
	int starty = (LINES - window_height)/2;

	// create ncurses window
	WINDOW *win = newwin(window_height + 2, window_width + 2, starty, startx);

	refresh();
	box(win, 0, 0);
	wrefresh(win);

	// init board, place snake in approximate middle
	Board board{WIDTH, HEIGHT};

	while (true) {

		// get user input
		int ch;
		ch = getch();

		board.tick(ch);

		if (board.getHasLost()) {
			break;
		}

		for (int y = 1; y <= HEIGHT; y++) {
			for (int i = 0; i < screenSizeMultiple; i++) {
				for (int x = 1; x <= WIDTH; x++) {
					if (board.getOffsetTile(x, y) == 0) {
						mvwprintw(win, y + i, x, "  ");
					}
					else if (board.getOffsetTile(x, y) == 1 || board.getOffsetTile(x, y) == 2) {
						wattron(win, COLOR_PAIR(1));
						mvwprintw(win, y + i, x, "##");
						wattroff(win, COLOR_PAIR(1));
					}
					else if (board.getOffsetTile(x, y) == 3) {
						wattron(win, COLOR_PAIR(2));
						mvwprintw(win, y + i, x, "OO");
						wattroff(win, COLOR_PAIR(2));
					}
				}
			}
		}
		wrefresh(win);
		usleep(board.getSleepTime() * K);
	}

	curs_set(1);
	endwin();

	std::cout << "\nYou lost :(\nYou ate " << board.getFruitsEaten() << " fruit!\n\n" << std::endl;
	return 0;
}
