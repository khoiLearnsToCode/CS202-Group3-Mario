#include "TitleScreen.h"
#include "ResourceManager.h"


TitleScreen::TitleScreen() : Screen(),
    startButton("StartButton", {GetScreenWidth() - 330.0f, GetScreenHeight() - 110.0f} , 0.3f), 
    credit("Credit", {100.0f, GetScreenHeight() - 60.0f}, 0.3f, BLACK, GetFontDefault(), 40.0f),
    temp("TEST BUTTON", "longButton", {50.0f, 100.0f}, 2.0f, WHITE, ResourceManager::getInstance().getFont("fixedsys"), 40.0f) {}

void TitleScreen::draw() {
    DrawTexture(textures["title_screen"], 0, 0, WHITE);
    DrawTexture(textures["guiRayMarioLogo"], 10, 10, WHITE);
    startButton.draw();
    credit.draw();
    temp.draw();

}

ButtonTexture& TitleScreen::getStartButton() {
    return startButton;
}

ButtonText& TitleScreen::getCreditButton() {
    return credit;
}
