#pragma once

#include <vector>
#include <fmt/core.h>
#include <time.h>

#include "snake.h"

/* On the board, 0 is an empty tile,
 * 1 is the snake's head,
 * 2 is the snake's body,
 * 3 is the fruit.
 */

class Board {

public:

	Board(int boardWidth, int boardHeight) : snake(boardWidth/2, boardHeight/2) {
		width = boardWidth;
		height = boardHeight;
		initializeBoard();

		// pseudo random seed, for placing fruit
		srand(time(NULL));
		placeFruit();
	}

	void tick(char input) {
		/* Call this method to make the game
		 * progress one tick forward; which means
		 * the snake will move one space forward,
		 * and the GameBoard object will handle
		 * everything else.
		 *
		 * For anything with static graphics
		 * (pixel, simple tile based, etc) this
		 * works wonders, and you simply sleep
		 * before ticking again.
		 *
		 * For anything with smooth animations or
		 * maybe sound playing, it would not work.
		 */

		// move snake
		snake.move(input);
		if (snake.collided_with_self()) {
			hasLost = true;
		}

		// check if eating fruit
		// generate a new fruit if needed
		std::vector<int> head = snake.show()[0];

		if (head == fruit) {
			snake.eatFruit();
			// sleeptime becomes 1% shorter.
			// this is maybe too convervative...
			sleepTime = std::floor(sleepTime * 0.99);
			placeFruit();
			fruitsEaten += 1;
		}

		// generate (clear) background
		generateBoard();

		// place snake
		placeSnake();

		// place fruit
		setTile(fruit[0], fruit[1], 3);

	}
	auto get_hasLost() -> bool {
		return hasLost;
	}

	auto get_sleepTime() -> int {
		return sleepTime;
	}

	auto get_fruitsEaten() -> int {
		return fruitsEaten;
	}

	auto get_tile(int x, int y) -> int {
		return board[y][x];
	}

	auto get_offsetTile(int x, int y) -> int {
		// offset for drawing, works in ncurses
		return board[y - 1][x - 1];
	}


private:

	int width {};
	int height {};
	std::vector<std::vector<int>> board {};
	Snake snake;
	std::vector<int> fruit {};
	int fruitsEaten = 0;
	bool hasLost = false;
	int sleepTime = 100;

	void generateBoard() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				board[y][x] = 0;
			}
		}
	}

	void initializeBoard() {
		// Generate empty board
		for (int y = 0; y < height; y++) {
			board.resize(height);
			board[y].resize(width, 0);
		}
	}

	void placeSnake() {

		std::vector<int> head = snake.show()[0];

		if (head[0] < 0 || head[0] == width || head[1] < 0 || head[1] == height) {
			hasLost = true;
		}
		else {
			setTile(head[0], head[1], 1);

			for (int i = 1; i < snake.length(); i++) {
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

		for (int i = 0; i < snake.length(); i++) {
			if (path[i] == new_fruit) {
				placeFruit();
			}
		}
		fruit = new_fruit;
	}

	void setTile(int x, int y, int z) {
		if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
			board[y][x] = z;
		}
	}
};

