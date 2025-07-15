#pragma once

#include "Screen.h"
#include "Button.h"
#include "ResourceManager.h"
#include "GameState.h"
#include "raylib.h"

#include <unordered_map>

class GameWorld;

class SettingScreen : public Screen {
public:
    SettingScreen(GameWorld* gw = nullptr);
    ~SettingScreen() override;

    SettingScreen(const SettingScreen&) = delete;
    SettingScreen& operator=(const SettingScreen&) = delete;

    void update();
    void draw() override;

    bool settingBoardShouldClose() const;

    float getMusicVolume() const;
    float getSfxVolume() const;
    void updateVolume() const;

    void setSettingBoardIsOpenInMenuScreen(bool isOpen);
private:
    std::unordered_map<std::string, Button*> buttons;
    Texture2D backgroundTexture;
    Rectangle border;

    GameWorld* gw;

    float musicVolume;
    float sfxVolume;
    bool isMutedMusic;
    bool isMutedSFX;
    bool settingBoardIsOpenInMenuScreen;

    
};
