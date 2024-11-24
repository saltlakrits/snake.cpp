#include <iostream>
#include <fmt/core.h>
// #include <chrono>
// usleep: sleep for arg microseconds
#include "board.h"
// #include <tuple>
#define K 1000 // usleep works in microseconds, so multiplying by K gives milliseconds

void hideCursor() {
    std::cout << "\033[?25l" << std::flush; // Hide cursor
}

void showCursor() {
    std::cout << "\033[?25h" << std::flush; // Show cursor
}

int main() {

	hideCursor();
	
	Board board{100, 30, 50, 15};

	// unbuffered input
	std::cin.rdbuf()->pubsetbuf(nullptr, 0);
	std::cout.rdbuf()->pubsetbuf(nullptr, 0); // Disable buffering for std::cout

	while (true) {
		std::system("clear");

		char input;
		// get a char
		std::cin.get(input);
		//char input = 'w';

		board.tick(input);
	}

	return 0;
}
