//#include <iostream>
#include <fmt/core.h>
// #include <chrono>
// usleep: sleep for arg microseconds
#include "board.h"
// #include <tuple>
#include <ncurses.h>
#include <fmt/core.h>
//#include <math.h>

#define K 1000 // usleep works in microseconds, so multiplying by K gives milliseconds

//void hideCursor() {
//    std::cout << "\033[?25l" << std::flush; // Hide cursor
//}
//
//void showCursor() {
//    std::cout << "\033[?25h" << std::flush; // Show cursor
//}

#define WIDTH 40
#define HEIGHT 20

int main() {

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);

	/* TODO
	 * Game boundaries/window box mismatched, you can
	 * both go under the border and not close enough to
	 * the border.
	 * TODO
	 * Background color? Might help with issue 1 as well.
	 * TODO
	 * Control issue?
	 * TODO
	 * You shouldn't be able to switch direction 180 degrees!
	 */
	// TODO Place window in the middle of the screen
	WINDOW *win = newwin(HEIGHT + 1, WIDTH + 1, 0, 0);
	refresh();

	// TODO Either this box is offset or something else is
	// BORKED, but you can go hang out under the left and
	// upper boundaries
	box(win, 0, 0);
	//mvwprintw(win, 1, 1, "TEST");
	wrefresh(win);

	int halfWidth = std::floor(WIDTH/2);
	int halfHeight = std::floor(HEIGHT/2);
	Board board{WIDTH, HEIGHT, halfWidth, halfHeight};
	std::vector<std::vector<int>> game_board;

	// unbuffered input
//	std::cin.rdbuf()->pubsetbuf(nullptr, 0);
//	std::cout.rdbuf()->pubsetbuf(nullptr, 0); // Disable buffering for std::cout

	while (true) {

		// TODO Pretty sure holding a key somehow blocks further
		// input for a bit
		int ch;
		ch = getch();

		game_board = board.tick(ch);

		if (board.hasLost) {
			break;
		}

		for (int y = 1; y < HEIGHT; y++) {
			for (int x = 1; x < WIDTH; x++) {
				// TODO Colored output? Background color?
				// TODO Instead of redrawing the whole board constantly,
				// maybe just have a tilesToDraw vector that we loop
				// through and draw. Not sure if you need to manually
				// clear the already drawn tiles each frame, though
				if (game_board[y][x] == 0) {
					mvwprintw(win, y, x, " ");
				}
				else if (game_board[y][x] == 1) {
					mvwprintw(win, y, x, "#");
				}
				else if (game_board[y][x] == 2) {
					mvwprintw(win, y, x, "#");
				}
				else if (game_board[y][x] == 3) {
					mvwprintw(win, y, x, "O");
				}
			}
		}
		wrefresh(win);
		usleep(board.sleepTime * K);
	}

	curs_set(1);
	endwin();

	fmt::print("You lost :(\n\n");
	return 0;
}
