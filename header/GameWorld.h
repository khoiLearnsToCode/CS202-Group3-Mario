#pragma once

class Mario;

#include "Drawable.h"
#include "GameState.h"
#include "Map.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "TitleScreen.h"
#include "MenuScreen.h"
#include "SettingScreen.h"
#include "CareTaker.h"
#include "Memento.h"
#include "Mario.h"
#include "raylib.h"
#include <iostream>

class GameWorld : public virtual Drawable {

    TitleScreen* titleScreen;
    MenuScreen* menuScreen;
    SettingScreen* settingScreen;

    friend class CareTaker;
    

    Mario mario;
    Map map;
    Camera2D* camera;
    bool settingBoardIsOpen;
    //GameState stateBeforePause;
    int remainingTimePointCount;

    // bool pauseMusic;
    // bool pauseMarioUpdate;
    // bool showOverlayOnPause;

    // bool irisOutFinished;
    // float irisOutTime;
    // float irisOutAcum;

    Memento* dataFromGameWorldToSave() const;
    void restoreDataFromMemento(const Memento* memento) const;

public:

    // static bool immortalMario;
    static GameState state;
    static float gravity;

    GameWorld();
    ~GameWorld() override;

    void initScreens();

    /**
     * @brief Reads user input and updates the state of the game.
     */
    void inputAndUpdate();

    /**
     * @brief Draws the state of the game.
     */
    void draw() override;

    /**
     * @brief Load game resources like images, textures, sounds, fonts, shaders,
     * etc.
     * Should be called inside the constructor.
     */
    static void loadResources();

    /**
     * @brief Unload the once loaded game resources.
     * Should be called inside the destructor.
     */
    static void unloadResources();

    void setCamera(Camera2D* camera);
    Camera2D* getCamera() const;

    void resetMap();
    void resetGame();
    void nextMap();
    void pauseGame(bool playPauseSFX, bool pauseMusic, bool showOverlay, bool pauseMarioUpdate);
    void unpauseGame();

    // bool isPauseMusicOnPause() const;
    // bool isShowOverlayOnPause() const;

};