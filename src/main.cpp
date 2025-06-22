#include <iostream>
#include <raylib.h>
#include <string>


using namespace std;

int main() {
	//InitWindow(800, 600, "Hello Raylib");
	//SetTargetFPS(60);

	//while (!WindowShouldClose()) {
	//
	//	BeginDrawing();
	//	ClearBackground(RAYWHITE);
	//	

	//	DrawText("Welcome to Raylib!", 190, 200, 20, LIGHTGRAY);
	//	
	//	EndDrawing();
	//}
	//CloseWindow(); 
	cout << string(GetWorkingDirectory()) << endl;
	
    return 0;
}

//!WindowShouldClose() loop is in GameWorld::init() in GameWorld.cpp