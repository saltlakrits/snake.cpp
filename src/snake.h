#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <deque>

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class Snake {

private:
	std::deque<std::vector<int>> snakePath {};
	enum Direction heading = UP;
	bool hasEaten = false; // if this is true, we
	// want to grow when we move

public:

	Snake(int x, int y) {
		snakePath.push_front({x, y});
	}
	// vector insert, pop_back, push_back
	// deque (Double Ended QUEue)
	// push_back and front, pop_back and front

	void setHeading(char input) {
		if (input == 'w') {
			heading = UP;
		}
		else if (input == 'd') {
			heading = RIGHT;
		}
		else if (input == 's') {
			heading = DOWN;
		}
		else if (input == 'a') {
			heading = LEFT;
		}
	}
	
	void move() {
		// Move snake by pushing a new tile to
		// the front of snake path, and, unless we're
		// growing, popping the last

		// maybe check here if lost?
		std::vector<int> head = snakePath[0];
		// grow one tile
		std::vector<int> new_head;
		// x first, y after (pushing)
		switch(heading) {
			// MAYBE COORDINATES SHOULD BE SWAPPED?
			case 0:
				new_head.push_back(head[0]);
				new_head.push_back(head[1] - 1);
				break;
			case 1:
				new_head.push_back(head[0] + 1);
				new_head.push_back(head[1]);
				break;
			case 2:
				new_head.push_back(head[0]);
				new_head.push_back(head[1] + 1);
				break;
			case 3:
				new_head.push_back(head[0] - 1);
				new_head.push_back(head[1]);
		}
		// now we add the new head (tile) to the snakePath
		snakePath.push_front(new_head);
		// unless we do want to grow, we want to pop the last coord
		if (hasEaten | (snakePath.size() <= 3)) {
			hasEaten = false;
		}
		else {
			snakePath.pop_back();
		}
	}

	std::deque<std::vector<int>> show() {
		// show private member

		std::deque<std::vector<int>> path;

		for (int x = 0; x < snakePath.size(); x++) {
			std::vector<int> inner_vec;
			std::copy(snakePath[x].begin(), snakePath[x].end(), back_inserter(inner_vec));
			path.push_back(inner_vec);
		}
		return path;
	}

	size_t size() {
		// show private member size, maybe unnecessary as can do snake.show().size() ?
		return snakePath.size();
	}

	void check_collided() {
		// check if collided with itself?
		// and/or board boundaries?
	}
	
	void eatFruit() {
		hasEaten = true;
	}
};

#endif
