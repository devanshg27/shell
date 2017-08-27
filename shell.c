#include <prompt.h>

void init() {
	initPrompt();
}

int main() {
	init();
	while(1) {
		showPrompt();
	}
	return 0;
}