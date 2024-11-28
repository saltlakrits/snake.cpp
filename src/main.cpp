#include "board.h"
#include <iostream>
#include <ncurses.h>
#include <unistd.h>

// usleep works in microseconds, so multiplying by K gives milliseconds
#define K 1000

#define WIDTH 20
#define HEIGHT 20

// how many times wider will we draw the
// horizontal chars than the vertical
#define GRAPHICAL_X_MULTIPLIER 2
// result will in reality depend on your font

#define BLOCK_BORDER 1
#define BLOCK_SNAKE 2
#define BLOCK_FRUIT 3
#define SCORE_COLOR 4

auto main() -> int {

  // initializing ncurses
  initscr();
  // note that from here on COLS, LINES are defined as the
  // line- and columncount of the term

  // TODO Implement setting integer scale manually via CLI arg?

  // check if LINES/COLS are big enough for minimum dimensions
  if (LINES < HEIGHT + 3 || COLS < (WIDTH + 2) * GRAPHICAL_X_MULTIPLIER) {
    endwin();
    std::cout << "Your terminal window is, somehow, too small.\n\
				For your own sake, and mine, make it bigger."
              << std::endl;
    std::exit(1);
  }

  // dynamic sizing (integer scaling)
  int width = COLS / WIDTH;
  // + 1 to have space for score, probably a better way to figure that
  int height = LINES / (HEIGHT + 1);
  int screenSizeMultiple = std::min(width, height);

  // ensure screenSizeMultiple is at least 1
  screenSizeMultiple = (screenSizeMultiple == 0) ? 1 : screenSizeMultiple;
  int y_multiple = screenSizeMultiple;
  // we want the horizontal characters to be multiplied by a factor,
  // in an attempt to even out the x and y axes
  int x_multiple = screenSizeMultiple * GRAPHICAL_X_MULTIPLIER;

  // more ncurses init
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  nodelay(stdscr, TRUE);
  start_color();

  // block char pairs, snake and fruit should be spaces
  init_pair(BLOCK_SNAKE, COLOR_BLACK, COLOR_GREEN);
  init_pair(BLOCK_FRUIT, COLOR_BLACK, COLOR_RED);

  // border color
  init_pair(BLOCK_BORDER, COLOR_BLUE, COLOR_BLUE);

  // score color
  init_pair(SCORE_COLOR, COLOR_BLACK, COLOR_BLUE);

  // new dynamic window size
  // will be HEIGHT, or HEIGHT*2, or HEIGHT*3...
  int window_height = HEIGHT * y_multiple;
  // will be WIDTH*2, WIDTH*4, WIDTH*6...
  int window_width = WIDTH * x_multiple;

  // approximate center of screen
  int startx = (COLS - window_width) / 2;
  int starty = (LINES - window_height) / 2;

  // create ncurses window
  // +2 in both directions to make space for border
  WINDOW *win = newwin(window_height + 2, window_width + 2, starty, startx);

  refresh();

  // draw border
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

    // build score string
    std::string scoreLine = "Fruits eaten: ";
    scoreLine += std::to_string(board.getFruitsEaten());
    int scoreLen = scoreLine.length();
    // convert to C string
    const char *score_c;
    score_c = scoreLine.c_str();

    // print score string in border
    attron(COLOR_PAIR(SCORE_COLOR));
    attron(A_BOLD);
    mvprintw(starty,
             startx + window_width + GRAPHICAL_X_MULTIPLIER - scoreLen - 1,
             "%s", score_c);
    attroff(COLOR_PAIR(SCORE_COLOR));
    attroff(A_BOLD);
    // treat string as argument to make it safe, according to lsp

    if (board.getHasLost()) {
      break;
    }

    // this nested for loop doesn't feel great
    // also maybe hard to read
    // draw the internal coordinates to the screen,
    // scaled accordingly
    for (int y = 1; y <= getmaxy(win) - 2; y += y_multiple) {
      // repeat y draw y_multiple times
      for (int i = 0; i < y_multiple; i++) {
        for (int x = 1; x <= getmaxx(win) - 2; x += x_multiple) {
          // repeat x draw x_multiple times
          for (int z = 0; z < x_multiple; z++) {
            // drawing coords converted to internal game coords
            int game_x = (x - 1) / x_multiple;
            int game_y = (y - 1) / y_multiple;

            if (board.getTile(game_x, game_y) == 0) {
              mvwprintw(win, y + i, x + z, " ");
            } else if (board.getTile(game_x, game_y) <= 2) {
              wattron(win, COLOR_PAIR(BLOCK_SNAKE));
              mvwprintw(win, y + i, x + z, " ");
              wattroff(win, COLOR_PAIR(BLOCK_SNAKE));
            } else if (board.getTile(game_x, game_y) == 3) {
              wattron(win, COLOR_PAIR(BLOCK_FRUIT));
              mvwprintw(win, y + i, x + z, " ");
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

  std::cout << "\nYou lost :(\nYou ate " << board.getFruitsEaten()
            << " fruit!\n\n"
            << std::endl;
  return 0;
}
