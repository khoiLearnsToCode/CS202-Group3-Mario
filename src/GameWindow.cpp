#include "GameWindow.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include <utility>

 /**
  * @brief Construct a new GameWindow object
  */

GameWindow::GameWindow(int width, int height, std::string title) : 
    width(width),
    height(height),
    title(std::move(title)),
    targetFPS(60),
    initAudio(true),
    gw(),
    camera(Camera2D()),
    careTaker(&gw),
    initialized(false) 
{}

/** 
 * @brief Destroy the GameWindow object
 */
GameWindow::~GameWindow() {
}

/**
 * @brief Initializes the Window, starts the game loop and, when it
 * finishes, the window will be finished too.
 */
void GameWindow::init() {

    if (!initialized) {

        InitWindow(width, height, title.c_str());

        if (initAudio) {
            InitAudioDevice();
        }
        SetTargetFPS(targetFPS);

        GameWorld::loadResources();

        gw.initScreens();
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

//std::string GameWindow::getTitle() const {
//    return title;
//}
//
//int GameWindow::getTargetFPS() const {
//    return targetFPS;
//}
//
//bool GameWindow::isAntialiasing() const {
//    return antialiasing;
//}
//
//bool GameWindow::isResizable() const {
//    return resizable;
//}
//
//bool GameWindow::isFullScreen() const {
//    return fullScreen;
//}
//
//bool GameWindow::isUndecorated() const {
//    return undecorated;
//}
//
//bool GameWindow::isAlwaysOnTop() const {
//    return alwaysOnTop;
//}
//
//bool GameWindow::isAlwaysRun() const {
//    return alwaysRun;
//}
//
//bool GameWindow::isInitAudio() const {
//    return initAudio;
//}
//
//bool GameWindow::isInitialized() const {
//    return initialized;
//}
//
//void GameWindow::setWidth(int width) {
//    if (!initialized) {
//        this->width = width;
//    }
//}
//
//void GameWindow::setHeight(int height) {
//    if (!initialized) {
//        this->height = height;
//    }
//}
//
//void GameWindow::setTitle(std::string title) {
//    if (!initialized) {
//        this->title = std::move(title);
//    }
//}



//void GameWindow::setAntialiasing(bool antialiasing) {
//    if (!initialized) {
//        this->antialiasing = antialiasing;
//    }
//}
//
//void GameWindow::setResizable(bool resizable) {
//    if (!initialized) {
//        this->resizable = resizable;
//    }
//}
//
//void GameWindow::setFullScreen(bool fullScreen) {
//    if (!initialized) {
//        this->fullScreen = fullScreen;
//    }
//}
//
//void GameWindow::setUndecorated(bool undecorated) {
//    if (!initialized) {
//        this->undecorated = undecorated;
//    }
//}
//
//void GameWindow::setAlwaysOnTop(bool alwaysOnTop) {
//    if (!initialized) {
//        this->alwaysOnTop = alwaysOnTop;
//    }
//}
//
//void GameWindow::setAlwaysRun(bool alwaysRun) {
//    if (!initialized) {
//        this->alwaysRun = alwaysRun;
//    }
//}
//
//void GameWindow::setInitAudio(bool initAudio) {
//    if (!initialized) {
//        this->initAudio = initAudio;
//    }
//}
