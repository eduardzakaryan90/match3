#include <iostream>
#include "GameController.h"

int32_t main() {
    match3::GameController gc;
	if (gc.initGame()) {
		gc.startGame();
	}
    return 0;
}
