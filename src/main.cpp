#include "board.h"
#include <ncurses.h>
// #include <fmt/core.h>
#include <iostream>
#include <unistd.h>

#define K 1000 // usleep works in microseconds, so multiplying by K gives milliseconds

#define WIDTH 40
#define HEIGHT 20
//#define SNAKE_COLOR 1
//#define FRUIT_COLOR 2
#define BLOCK_BORDER 1
#define BLOCK_SNAKE 2
#define BLOCK_FRUIT 3

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
	//int screenSizeMultiple = std::min(width, height);
	// this should reasonably never be needed
	// screenSizeMultiple = (screenSizeMultiple == 0) ? 1 : screenSizeMultiple;
	//screenSizeMultiple = 2; // only for debug, ensure that we can scale it to double size before doing anything fancier
	int y_multiple = 2;
	int x_multiple = 2 * 2;

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);
	start_color();

	// colorpairs for the snake and fruit
	// init_pair(SNAKE_COLOR, COLOR_GREEN, COLOR_BLACK);
	// init_pair(FRUIT_COLOR, COLOR_RED, COLOR_BLACK);

	// block char pairs, snake and fruit should be spaces
	init_pair(BLOCK_SNAKE, COLOR_BLACK, COLOR_GREEN);
	init_pair(BLOCK_FRUIT, COLOR_BLACK, COLOR_RED);

	// border color
	init_pair(BLOCK_BORDER, COLOR_BLACK, COLOR_YELLOW);

	// new dynamic window size
	int window_height = HEIGHT * y_multiple;
	int window_width = WIDTH * x_multiple;

	// approximate center of screen
	int startx = (COLS - window_width)/2;
	int starty = (LINES - window_height)/2;

	// create ncurses window
	WINDOW *win = newwin(window_height + 2, window_width + 2, starty, startx);

	refresh();

	wattron(win, COLOR_PAIR(BLOCK_BORDER));
	box(win, 0, 0);
	wattroff(win, COLOR_PAIR(BLOCK_BORDER));
	wrefresh(win);

	// init board
	Board board{WIDTH, HEIGHT};

	while (true) {

		// get user input
		int ch;
		ch = getch();

		board.tick(ch);

		if (board.getHasLost()) {
			break;
		}

		// my dynamic sizing doesn't work. maybe it's easier to scale the internal
		// representation.

		// TODO: x should be "graphically scaled" more than y, to compensate for the fact
		// that most fonts are taller than they are wide. it's probably never going to be
		// perfect, though.

		// this nested for loop doesn't feel great
		// also maybe hard to read
		for (int y = 0; y < window_height; y += y_multiple) {
			// repeat y draw y_multiple times
			for (int i = 0; i < y_multiple; i++) {
				for (int x = 0; x < window_width; x += x_multiple) {
					// repeat x draw x_multiple times
					for (int z = 0; z < x_multiple; z++) {
						// drawing coords converted to internal game coords
						int game_x = (x - 0)/x_multiple;
						int game_y = (y - 0)/y_multiple;

						if (board.getOffsetTile(game_x, game_y) == 0) {
							mvwprintw(win, y + i,  x + z, " ");
						}
						else if (board.getOffsetTile(game_x, game_y) <= 2) {
							wattron(win, COLOR_PAIR(BLOCK_SNAKE));
							mvwprintw(win, y + i,  x + z, " ");
							wattroff(win, COLOR_PAIR(BLOCK_SNAKE));
						}
						else if (board.getOffsetTile(game_x, game_y) == 3) {
							wattron(win, COLOR_PAIR(BLOCK_FRUIT));
							mvwprintw(win, y + i,  x + z, " ");
							wattroff(win, COLOR_PAIR(BLOCK_FRUIT));
						}
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
