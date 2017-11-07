#include"Window.h"

int main(int argc, char **argv) {

	Window window("SDL Tutorial", 800, 600);
	
	while (!window.isClosed()) {
		window.pollEvents();
		window.clear();
	}
	return 0;
}