#include <iostream>
#include "GameController.h"

int main() {
    match3::GameController gc;
	if (gc.initGame()) {
		gc.startGame();
	}
    return 0;
}
