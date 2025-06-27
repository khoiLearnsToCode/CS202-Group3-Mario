#include <iostream>
#include <raylib.h>
#include <string>
#include "GameWindow.h"



int main() {
	
	GameWindow gameWindow(1600, 900, "MarioGame");
	gameWindow.init();
    return 0;
}
