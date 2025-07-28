#include "SettingScreen.h"
#include "GameWorld.h"
#include "GuardScreen.h"

#ifndef RAYGUI_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION
#endif

#include "raygui.h"

SettingScreen::SettingScreen(GameWorld* gw) : Screen(), musicVolume(1.0f), sfxVolume(1.0f), 
isMutedMusic(false), isMutedSFX(false), settingBoardIsOpenInMenuScreen(true), gw(gw) {

    Image img = LoadImageFromTexture(textures["settingBackground"]);
    ImageResize(&img, img.width * 0.5f, img.height * 0.5f); 
    backgroundTexture = LoadTextureFromImage(img);
    UnloadImage(img);

    Image settingLogoImage = LoadImageFromTexture(textures["setting"]);
    ImageResize(&settingLogoImage, settingLogoImage.width * 0.4f, settingLogoImage.height * 0.4f);
    settingLogo = LoadTextureFromImage(settingLogoImage);
    UnloadImage(settingLogoImage);

    border = { (GetScreenWidth() - backgroundTexture.width) / 2.0f, 
               (GetScreenHeight() - backgroundTexture.height) / 2.0f, 
               (float)backgroundTexture.width, 
               (float)backgroundTexture.height };


    buttons.emplace("MUTEMUSIC", new ButtonTextTexture("muteButton", {border.x + 100, border.y + 100}, 2.0f));
    buttons.emplace("UNMUTEMUSIC", new ButtonTextTexture("unmuteButton", {border.x + 100, border.y + 100}, 2.0f));

    buttons.emplace("MUTESFX", new ButtonTextTexture("muteButton", {border.x + 100, border.y + 175}, 2.0f));
    buttons.emplace("UNMUTESFX", new ButtonTextTexture("unmuteButton", {border.x + 100, border.y + 175}, 2.0f));

    buttons.emplace("HOME", new ButtonTextTexture("homeButton", {border.x + 100, border.y + 250}, 2.0f));
    buttons.emplace("RESET", new ButtonTextTexture("resetButton", {border.x + 100, border.y + 325}, 2.0f));

    GuiLoadStyle("../resource/font/candy.rgs");
}

SettingScreen::~SettingScreen() {
    for (auto& buttonPair : buttons) {
        delete buttonPair.second;
    }
    UnloadTexture(backgroundTexture);
}

void SettingScreen::update() {

    if (!isMutedMusic && buttons["UNMUTEMUSIC"]->isReleased()) {
        isMutedMusic = true;
        musicVolume = 0.0f; // Mute music
    } else if (isMutedMusic && buttons["MUTEMUSIC"]->isReleased()) {
        isMutedMusic = false;
        musicVolume = 0.5f; // Unmute music
    }

    if (!isMutedSFX && buttons["UNMUTESFX"]->isReleased()) {
        isMutedSFX = true;
        sfxVolume = 0.0f; // Mute SFX
    } else if (isMutedSFX && buttons["MUTESFX"]->isReleased()) {
        isMutedSFX = false;
        sfxVolume = 0.5f; // Unmute SFX
    }

    // Don't allow HOME and RESET buttons when guard screen is active
    if (GameWorld::state != GAME_STATE_GUARD_SCREEN) {
        if (buttons["HOME"]->isReleased()) {
            settingBoardIsOpenInMenuScreen = false;
            if (!isMutedSFX) {
                PlaySound(ResourceManager::getInstance().getSound("pause"));
            }
            if (gw) {
                gw->showGuardScreen(GUARD_ACTION_HOME);
            }
            else {
                std::cerr << "GameWorld pointer is null. Cannot show guard screen." << std::endl;
            }
        } 
        
        if (buttons["RESET"]->isReleased()) {
            settingBoardIsOpenInMenuScreen = false;
            if (!isMutedSFX) {
                PlaySound(ResourceManager::getInstance().getSound("pause"));
            }
            if (gw) {
                gw->showGuardScreen(GUARD_ACTION_RESET);
            } else {
                std::cerr << "GameWorld pointer is null. Cannot show guard screen." << std::endl;
            }
        }
    }
}

void SettingScreen::draw() {
    DrawTexture(backgroundTexture, border.x, border.y, WHITE);

    // draw setting logo in the top middle of the border
    Vector2 logoPos = { border.x + (border.width - settingLogo.width) / 2.0f, 
                        border.y - 60.0f };
    DrawTexture(settingLogo, logoPos.x, logoPos.y, WHITE);
    for (const auto& buttonPair : buttons) {
        if ((buttonPair.first == "HOME" || buttonPair.first == "RESET") && settingBoardIsOpenInMenuScreen) {
            continue;
        }
        // Don't draw HOME and RESET buttons when guard screen is active
        // if ((buttonPair.first == "HOME" || buttonPair.first == "RESET") && GameWorld::state == GAME_STATE_GUARD_SCREEN) {
        //     continue;
        // }
        buttonPair.second->draw();
    }

    Font& font = ResourceManager::getInstance().getFont("SuperMario256");
    float fontSize = 25.0f;
    Color textColor1 = {236, 160, 119, 255};
    Color textColor2 = {235, 114, 114, 255};

    Rectangle musicSliderRec = { border.x + 275, border.y + 105 + 16, 200, 20 };
    GuiSlider(musicSliderRec, "", "", &musicVolume, 0.0f, 1.0f);
    bool isMouseHoveringMusicSlider = CheckCollisionPointRec(GetMousePosition(), musicSliderRec);
    DrawTextEx(font, "MUSIC", { musicSliderRec.x - 95, musicSliderRec.y }, fontSize, 0.0f, 
                isMouseHoveringMusicSlider ? textColor2 : textColor1);
    musicVolume = floor(musicVolume * 100.0f) / 100.0f;
    DrawTextEx(font, std::to_string((int)(musicVolume * 100)).c_str(),
              { musicSliderRec.x + 210, musicSliderRec.y }, fontSize, 0.0f,
              isMouseHoveringMusicSlider ? textColor2 : textColor1);

    Rectangle sfxSliderRec = { border.x + 275, border.y + 105 + 16 + 75, 200, 20 };
    GuiSlider(sfxSliderRec, "", "", &sfxVolume, 0.0f, 1.0f);
    bool isMouseHoveringSfxSlider = CheckCollisionPointRec(GetMousePosition(), sfxSliderRec);
    DrawTextEx(font, "SFX", { sfxSliderRec.x - 60, sfxSliderRec.y }, fontSize, 0.0f, 
                isMouseHoveringSfxSlider ? textColor2 : textColor1);
    sfxVolume = floor(sfxVolume * 100.0f) / 100.0f;
    DrawTextEx(font, std::to_string((int)(sfxVolume * 100)).c_str(),
              { sfxSliderRec.x + 210, sfxSliderRec.y }, fontSize, 0.0f,
              isMouseHoveringSfxSlider ? textColor2 : textColor1);

    if (musicVolume != 0.0f) {
        isMutedMusic = false; // Unmute if volume is set
    } 

    if (sfxVolume != 0.0f) {
        isMutedSFX = false; // Unmute if volume is set
    } 

    // Draw mute buttons
    if (isMutedMusic) {
        buttons["MUTEMUSIC"]->draw();
    } else {
        buttons["UNMUTEMUSIC"]->draw();
    }
    
    if (isMutedSFX) {
        buttons["MUTESFX"]->draw();
    } else {
        buttons["UNMUTESFX"]->draw();
    }
}

bool SettingScreen::settingBoardShouldClose() const {
    Vector2 mousePos = GetMousePosition();
    return (!CheckCollisionPointRec(mousePos, border)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

float SettingScreen::getMusicVolume() const {
    return musicVolume;
}

float SettingScreen::getSfxVolume() const {
    return sfxVolume;
} 

void SettingScreen::updateVolume() const {
    if (musicVolume != ResourceManager::getInstance().getMusicVolume()) {
        ResourceManager::getInstance().setMusicVolume(musicVolume);
    }

    if (sfxVolume != ResourceManager::getInstance().getSfxVolume()) {
        ResourceManager::getInstance().setSfxVolume(sfxVolume);
    }
}

void SettingScreen::setSettingBoardIsOpenInMenuScreen(bool isOpen) {
    settingBoardIsOpenInMenuScreen = isOpen;
}