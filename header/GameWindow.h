#pragma once
#include "GameWorld.h"
#include "ResourceManager.h"
#include "string"

class GameWindow {

    int width;
    int height;
    std::string title;
    int targetFPS;
    bool initAudio;

    GameWorld gw;
    Camera2D camera;
    CareTaker careTaker;
    bool initialized;

public:

    GameWindow() = default;
    GameWindow(int width, int height, std::string title);
    ~GameWindow();

    void init();

    // Essential getters only
    int getWidth() const;
    int getHeight() const;
    GameWorld& getGameWorld();
    Camera2D& getCamera();

    // Essential setters only
    void setTargetFPS(int fps);
};