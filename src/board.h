#pragma once

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "snake.h"

/* On the board, 0 is an empty tile,
 * 1 is the snake's head,
 * 2 is the snake's body,
 * 3 is the fruit.
 */

class Board {

public:
  Board(int boardWidth, int boardHeight)
      : snake(boardWidth / 2, boardHeight / 2) {
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
    if (snake.collidedWithSelf()) {
      hasLost = true;
    }

    // check if eating fruit
    // generate a new fruit if needed
    const std::vector<int>& head = snake.getPath()[0];

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
  auto getHasLost() -> bool { return hasLost; }

  auto getSleepTime() -> int { return sleepTime; }

  auto getFruitsEaten() -> int { return fruitsEaten; }

  auto getTile(int x, int y) -> int { return board[y][x]; }

private:
  int width{};
  int height{};
  std::vector<std::vector<int>> board{};
  Snake snake;
  std::vector<int> fruit{};
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

    const std::vector<int>& head = snake.getPath()[0];

    if (head[0] < 0 || head[0] == width || head[1] < 0 || head[1] == height) {
      hasLost = true;
    } else {
      setTile(head[0], head[1], 1);

      for (int i = 1; i < snake.length(); i++) {
        setTile(snake.getPath()[i][0], snake.getPath()[i][1], 2);
      }
    }
  }

  void placeFruit() {
    while (true) {

      // Randomly place a fruit
      int random_x = rand() % width;
      int random_y = rand() % height;

      std::vector<int> new_fruit = {random_x, random_y};
      const std::deque<std::vector<int>>& path = snake.getPath();

      bool collides = false;

      // check if it overlaps with snake
      for (int i = 0; i < snake.length(); i++) {
        if (path[i] == new_fruit) {
          // if it does, we generate new coords and try again
          collides = true;
        }
      }
      if (!collides) {
        fruit = new_fruit;
        break;
      }
    }
  }

  void setTile(int x, int y, int z) {
    // if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
    // 	board[y][x] = z;
    // }
    if ((0 <= x < width) && (0 <= y < height)) {
      board[y][x] = z;
    }
  }
};
