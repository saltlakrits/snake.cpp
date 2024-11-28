#pragma once

#include <vector>
#include <deque>

// For controls
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class Snake {

private:
	std::deque<std::vector<int>> path {};
	enum Direction heading = UP;
	bool hasEaten = false; // if this is true, we
	// want to grow when we move

	void setHeading(char input) {
		if (input == 'w' && heading != DOWN) {
			heading = UP;
		}
		else if (input == 'd' && heading != LEFT) {
			heading = RIGHT;
		}
		else if (input == 's' && heading != UP) {
			heading = DOWN;
		}
		else if (input == 'a' && heading != RIGHT) {
			heading = LEFT;
		}
	}

public:

	Snake(int x, int y) {
		path.push_front({x, y});
	}
	// vector insert, pop_back, push_back
	// deque (Double Ended QUEue)
	// push_back and front, pop_back and front

	
	void move(char input) {
		// Move snake by pushing a new tile to
		// the front of snake path, and, unless we're
		// growing, popping the last

		setHeading(input);

		std::vector<int> head = path[0];
		// grow one tile
		std::vector<int> new_head;
		// x first, y after
		switch(heading) {
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
		path.push_front(new_head);
		// unless we do want to grow, we want to pop the last coord
		// if we are less than 3 tiles long, we want to grow regardless
		if (hasEaten || (path.size() <= 3)) {
			hasEaten = false;
		}
		else {
			path.pop_back();
		}
	}

	 std::deque<std::vector<int>> getPath() {
		// show private member HA HA

		std::deque<std::vector<int>> path_copy;

		for (int x = 0; x < path.size(); x++) {
			std::vector<int> inner_vec;
			std::copy(path[x].begin(), path[x].end(), back_inserter(inner_vec));
			path_copy.push_back(inner_vec);
		}
		return path_copy;
	}

	auto length() -> size_t {
		// show private member size, maybe unnecessary as can do snake.show().size() ?
		// just convenient
		return path.size();
	}

	auto collidedWithSelf() -> bool {
		for (int i = 1; i < path.size(); i++) {
			if (path[0] == path[i]) {
				return true;
			}
		}
		return false;
	}
	
	void eatFruit() {
		hasEaten = true;
	}
};

