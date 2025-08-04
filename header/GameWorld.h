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
#include "MapEditorScreen1.h"
#include "MapEditorScreen2.h"
#include "SelectCharacterScreen.h"
#include "SettingScreen.h"
#include "HelpingScreen.h"
#include "GuardScreen.h"
#include "LeaderBoardScreen.h"
#include "CareTaker.h"
#include "Memento.h"
#include "Mario.h"
#include "raylib.h"
#include <iostream>

class GameWorld : public virtual Drawable {

    TitleScreen* titleScreen;
    MenuScreen* menuScreen;
    MapEditorScreen1* mapEditorScreen1;
    MapEditorScreen2* mapEditorScreen2;
    SelectCharacterScreen* selectCharacterScreen;
    SettingScreen* settingScreen;
    HelpingScreen* helpingScreen;
    GuardScreen* guardScreen;
    LeaderBoardScreen* leaderBoardScreen;

    friend class CareTaker;
    friend class SettingScreen;

    Mario mario;
    Map map;
    Camera2D* camera;
    bool settingBoardIsOpen;
    bool helpingBoardIsOpen;
    bool leaderBoardIsOpen;
    GameState stateBeforePause;
    int remainingTimePointCount;
    int totalPlayedTime;

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

    // Distance-based collision detection threshold
    const float maxDistForCollisionCheck;

    Memento* dataFromGameWorldToSave();
    void restoreDataFromMemento(const Memento* memento) const;

    Memento* dataFromGameWorldToLeaderboard();

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

    int getTotalPlayedTime() const;

    void addToTotalPlayedTime(float timeToAdd);

    void resetMap();
    void resetGame();
    void nextMap();
    void pauseGame(bool playPauseSFX, bool pauseMusic, bool pauseMario, bool showSettingBoard, bool showHelpingBoard);
    void unpauseGame();
    void showGuardScreen(GuardAction action);

    // Distance threshold getter for collision optimization
    float getMaxDistForCollisionCheck() const;

    // bool isPauseMusicOnPause() const;
    // bool isShowOverlayOnPause() const;

};