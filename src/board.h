#ifndef BOARD_H
#define BOARD_H

#include <tuple>
#include <vector>
#include <optional>
#include <fmt/core.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "snake.h"
// empty tile = 0
// snake head = 1
// snake body = 2
// fruit = 3

#define K 1000 // for usleep

class Board {

private:

	int width {};
	int height {};
	std::vector<std::vector<int>> board {};
	Snake snake;
	std::vector<int> fruit {};

public:
	int sleepTime = 100;
	bool hasLost = false;


	Board(int boardWidth, int boardHeight, int startX, int startY) : snake(startX, startY) {
		width = boardWidth;
		height = boardHeight;
		initializeBoard();

		// pseudo random seed, for placing fruit
		srand(time(NULL));
		placeFruit();

	}

	void initializeBoard() {
		// Generate empty board
		for (int y = 0; y < height; y++) {
			board.resize(height);
			board[y].resize(width, 0);
		}
	}

	void placeSnake() {
		// insert logic to check if snake fits
		// this is for drawing the snake. OBSOLETE RN.

		// check if head is outside map (or collided with itself?) here?

		std::vector<int> head = snake.show()[0];
		// TODO: unless i have width-1 and height-1 i get a segfault -- is it wrong here,
		// or elsewhere?
		if (head[0] < 0 or head[0] > width-1 or head[1] < 0 or head[1] > height-1) {
			hasLost = true;
		}
		else {
			setTile(head[0], head[1], 1);

			for (int i = 1; i < snake.size(); i++) {
				setTile(snake.show()[i][0], snake.show()[i][1], 2);
			}
		}
	}

	void placeFruit() {
		// Randomly place a fruit
		int random_x = rand() % width;
		int random_y = rand() % height;

		std::vector<int> new_fruit = {random_x, random_y};
		std::deque<std::vector<int>> path = snake.show();

		for (int i = 0; i < snake.size(); i++) {
			if (path[i] == new_fruit) {
				placeFruit();
			}
		}
		fruit = new_fruit;
	}

	void setTile(int x, int y, int z) {
		// can do a fancy setter like this, but this should be private anyway
		//if ((x >= 0) & (x < width) & (y >= 0) & (y < height)) {
		//	board[y][x] = z;
		//}
		board[y][x] = z;
	}

	int getOffsetTile(int x, int y) {
		// For use with drawBoard()
		// coordinates are shifted!
		return board[y - 1][x - 1];
	}

	void drawBoard() {
		for (int y = 0; y <= height + 1; y++) {
			for (int x = 0; x <= width + 1; x++) {
				// this is pretty ugly
				if (y == 0) {
					fmt::print("_");
				}
				else if (y == (height + 1)) {
					fmt::print("¯");
				}
				else if (x == 0 or x == (width + 1)) {
					fmt::print("|");
				}
				else if (getOffsetTile(x, y) == 0) {
					fmt::print(" ");
				}
				else if (getOffsetTile(x, y) == 1) {
					fmt::print("#");
				}
				else if (getOffsetTile(x, y) == 2) {
					fmt::print("#");
				}
				else if (getOffsetTile(x, y) == 3) {
					fmt::print("%");
				}
			}
			fmt::print("\n");
		}
	}
	void generateBoard() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				board[y][x] = 0;
			}
		}
	}

	std::vector<std::vector<int>> tick(char input) {
		// a game tick

		snake.setHeading(input);

		// move snake
		snake.move();

		// check if overlapping with fruit and eat it, setting hasEaten on snake or calling method
		// generate a new fruit as well
		std::vector<int> head = snake.show()[0];
		if (head == fruit) {
			snake.eatFruit();
			sleepTime = std::floor(sleepTime * 0.99);
			placeFruit();
		}

		// generate background
		generateBoard();

		// place snake
		placeSnake();

		// place fruit
		setTile(fruit[0], fruit[1], 3);

		// draw output
		//drawBoard();

		//usleep(sleepTime * K);

		return board;
	}

};

#endif
