#pragma once

class Mario;

#include "Drawable.h"
#include "GameState.h"
#include "Map.h"
#include "Button.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "TitleScreen.h"
#include "MenuScreen.h"
#include "SelectCharacterScreen.h"
#include "SettingScreen.h"
#include "HelpingScreen.h"
#include "GuardScreen.h"
#include "CareTaker.h"
#include "Memento.h"
#include "Mario.h"
#include "raylib.h"
#include <iostream>

class GameWorld : public virtual Drawable {

    TitleScreen* titleScreen;
    MenuScreen* menuScreen;
    SelectCharacterScreen* selectCharacterScreen;
    SettingScreen* settingScreen;
    HelpingScreen* helpingScreen;
    GuardScreen* guardScreen;

    friend class CareTaker;
    friend class SettingScreen;

    Mario mario;
    Map map;
    Camera2D* camera;
    bool settingBoardIsOpen;
    bool helpingBoardIsOpen;
    GameState stateBeforePause;
    int remainingTimePointCount;

    bool pauseMusic;
    bool pauseMario;
    // bool showOverlayOnPause;

    bool outroFinished;
    float outroTime;
    float outroAcum;

    ButtonTextTexture* settingButton;
    ButtonTextTexture* helpButton;

    // Pause-game buttons cooldown
    float pauseButtonsCooldownAcum;
    float pauseButtonsCooldownTime;

    Memento* dataFromGameWorldToSave();
    void restoreDataFromMemento(const Memento* memento) const;

public:

    // static bool immortalMario;
    static GameState state;
    static float gravity;

    GameWorld();
    ~GameWorld() override;

    void initScreensAndButtons();

    // get user input, update game state
    void inputAndUpdate();
    void draw() override;

    static void loadResources();
    static void unloadResources();

    void setCamera(Camera2D* camera);
    Camera2D* getCamera() const;

    void resetMap();
    void resetGame();
    void nextMap();
    void pauseGame(bool playPauseSFX, bool pauseMusic, bool pauseMario, bool showSettingBoard, bool showHelpingBoard);
    void unpauseGame();
    void showGuardScreen(GuardAction action);

    // bool isPauseMusicOnPause() const;
    // bool isShowOverlayOnPause() const;

};