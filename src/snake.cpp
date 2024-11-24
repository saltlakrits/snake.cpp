#include <clocale>
#include <notcurses/notcurses.h>


int main() {
	setlocale(LC_ALL, "");
	notcurses_init();
	notcurses_stop();
	return 0;
}
