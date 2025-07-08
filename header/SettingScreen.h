#pragma once

#include "Screen.h"
#include "Button.h"
#include "ResourceManager.h"
#include "raylib.h"

#include <unordered_map>

class SettingScreen : public Screen {
public:
    SettingScreen();
    ~SettingScreen() override;

    SettingScreen(const SettingScreen&) = delete;
    SettingScreen& operator=(const SettingScreen&) = delete;

    void update();
    void draw() override;

    bool settingBoardShouldClose() const;

private:
    std::unordered_map<std::string, Button*> buttons;
    Texture2D backgroundTexture;
    Rectangle border;

    float musicVolume;
    float sfxVolume;
    bool isMutedMusic;
    bool isMutedSFX;
};
