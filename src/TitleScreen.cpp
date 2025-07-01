#include "TitleScreen.h"
#include "ResourceManager.h"

TitleScreen::TitleScreen() : 
    startButton("StartButton", {GetScreenWidth() - 330.0f, GetScreenHeight() - 110.0f} , 0.3f), 
    credit("Credit", {100.0f, GetScreenHeight() - 60.0f}, 0.3f, BLACK, GetFontDefault(), 40.0f),
    textures(ResourceManager::getInstance().getTextures()) {}

void TitleScreen::draw() {
    DrawTexture(textures["title_screen"], 0, 0, WHITE);
    DrawTexture(textures["guiRayMarioLogo"], 10, 10, WHITE);
    startButton.draw();
    credit.draw();
}

Button& TitleScreen::getStartButton() {
    return startButton;
}
