#include "SettingScreen.h"

#ifndef RAYGUI_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION
#endif

#include "raygui.h"

SettingScreen::SettingScreen() : Screen(), musicVolume(0.5f), sfxVolume(0.5f), isMutedMusic(false), isMutedSFX(false) {
    Image img = LoadImageFromTexture(textures["settingBackground"]);
    ImageResize(&img, img.width * 10.0f, img.height * 10.0f); 
    backgroundTexture = LoadTextureFromImage(img);
    UnloadImage(img);

    border = { (GetScreenWidth() - backgroundTexture.width) / 2.0f, 
               (GetScreenHeight() - backgroundTexture.height) / 2.0f, 
               (float)backgroundTexture.width, 
               (float)backgroundTexture.height };

    std::cout << "Border: " << border.x << ", " << border.y << ", " << border.width << ", " << border.height << std::endl;

    buttons.emplace("MUTEMUSIC", new ButtonTextTexture("muteButton", {border.x + 50, border.y + 50}, 2.0f));
    buttons.emplace("UNMUTEMUSIC", new ButtonTextTexture("unmuteButton", {border.x + 50, border.y + 50}, 2.0f));

    buttons.emplace("MUTESFX", new ButtonTextTexture("muteButton", {border.x + 50, border.y + 120}, 2.0f));
    buttons.emplace("UNMUTESFX", new ButtonTextTexture("unmuteButton", {border.x + 50, border.y + 120}, 2.0f));

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

    //std::cout << "Music Volume: " << musicVolume << ", SFX Volume: " << sfxVolume << std::endl;
   
}

void SettingScreen::draw() {
    DrawTexture(backgroundTexture, border.x, border.y, WHITE);
    for (const auto& buttonPair : buttons) {
        buttonPair.second->draw();
    }

    Vector2 musicSliderPos = { border.x + 120, border.y + 50 + 16 };
    GuiSlider({ musicSliderPos.x, musicSliderPos.y, 200, 20 }, "", "", &musicVolume, 0.0f, 1.0f);
    musicVolume = floor(musicVolume * 10.0f) / 10.0f;

    Vector2 sfxSliderPos = { border.x + 120, border.y + 50 + 16 + 70};
    GuiSlider({ sfxSliderPos.x, sfxSliderPos.y, 200, 20 }, "", "", &sfxVolume, 0.0f, 1.0f);
    sfxVolume = floor(sfxVolume * 10.0f) / 10.0f;

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