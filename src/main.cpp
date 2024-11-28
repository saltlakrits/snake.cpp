#include "board.h"
#include <ncurses.h>
#include <fmt/core.h>
#include <unistd.h>

#define K 1000 // usleep works in microseconds, so multiplying by K gives milliseconds

#define WIDTH 40
#define HEIGHT 20
#define SNAKE_COLOR 1
#define FRUIT_COLOR 2

/* TODO
 * Control issue? Seems to be hard to fix in ncurses
 */

auto main() -> int {

	// initializing ncurses
	initscr();

	if (LINES < HEIGHT || COLS < WIDTH) {
		fmt::print("Your terminal window is, somehow, too small.\n\
				For your own sake, and mine, make it bigger.");
		std::exit(1);
	}

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);
	start_color();

	// colorpairs for the snake and fruit
	init_pair(SNAKE_COLOR, COLOR_GREEN, COLOR_BLACK);
	init_pair(FRUIT_COLOR, COLOR_RED, COLOR_BLACK);

	// approximate center of screen
	int startx = (COLS - WIDTH)/2;
	int starty = (LINES - HEIGHT)/2;

	// create ncurses window
	WINDOW *win = newwin(HEIGHT + 2, WIDTH + 2, starty, startx);

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

		if (board.get_hasLost()) {
			break;
		}

		for (int y = 1; y <= HEIGHT; y++) {
			for (int x = 1; x <= WIDTH; x++) {
				if (board.get_offsetTile(x, y) == 0) {
					mvwaddch(win, y, x, ' ');
				}
				else if (board.get_offsetTile(x, y) == 1) {
					wattron(win, COLOR_PAIR(1));
					mvwaddch(win, y, x, '#');
					wattroff(win, COLOR_PAIR(1));
				}
				else if (board.get_offsetTile(x, y) == 2) {
					wattron(win, COLOR_PAIR(1));
					mvwaddch(win, y, x, '#');
					wattroff(win, COLOR_PAIR(1));
				}
				else if (board.get_offsetTile(x, y) == 3) {
					wattron(win, COLOR_PAIR(2));
					mvwaddch(win, y, x, 'O');
					wattroff(win, COLOR_PAIR(2));
				}
			}
		}
		wrefresh(win);
		usleep(board.get_sleepTime() * K);
	}

	curs_set(1);
	endwin();

	fmt::print("\nYou lost :(\nYou ate {} fruit!\n\n", board.get_fruitsEaten());
	return 0;
}