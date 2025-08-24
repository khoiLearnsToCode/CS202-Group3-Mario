#include "GameWindow.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include <utility>


GameWindow::GameWindow(int width, int height, std::string title) : 
    width(width),
    height(height),
    title(std::move(title)),
    targetFPS(60),
    initAudio(true),
    gw(),
    camera(Camera2D()),
    careTaker(&gw),
    loadGame(&gw),
    initialized(false) 
{}


GameWindow::~GameWindow() {
}


void GameWindow::init() {

    if (!initialized) {

        InitWindow(width, height, title.c_str());
        Image icon = LoadImage("../resource/graphic/gui/marioIcon.jpg");
        SetWindowIcon(icon);
        UnloadImage(icon);

        if (initAudio) {
            InitAudioDevice();
        }
        SetTargetFPS(targetFPS);

        GameWorld::loadResources();

        gw.initScreensAndButtons();
        initialized = true;

        camera.target = Vector2{ 0, 0 };
        camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight()*1.0f - 104 };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        gw.setCamera(&camera);

        while (!WindowShouldClose()) {
            gw.inputAndUpdate();
            gw.draw();
        }

        GameWorld::unloadResources();
        ResourceManager::destroyInstance();
        if (initAudio) {
            CloseAudioDevice();
        }
        CloseWindow();
    }

}

int GameWindow::getWidth() const {
    return width;
}

int GameWindow::getHeight() const {
    return height;
}

GameWorld& GameWindow::getGameWorld() {
    return gw;
}

Camera2D& GameWindow::getCamera() {
    return camera;
}

void GameWindow::setTargetFPS(int targetFPS) {
    if (!initialized) {
        this->targetFPS = targetFPS;
    }
}

