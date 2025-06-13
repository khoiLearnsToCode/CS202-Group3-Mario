#include <iostream>
#include <raylib.h>


using namespace std;

int main() {
	InitWindow(800, 600, "Hello Raylib");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
	
		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		// Call the function from hello.h

		DrawText("Welcome to Raylib!", 190, 200, 20, LIGHTGRAY);
		
		EndDrawing();
	}
	CloseWindow(); // Close window and OpenGL context
	
    return 0;
}

//!WindowShouldClose() loop is in GameWorld::init() in GameWorld.cpp