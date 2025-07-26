#include "TitleScreen.h"
#include "ResourceManager.h"


TitleScreen::TitleScreen() : Screen(),
    startButton("StartButton", {GetScreenWidth() - 330.0f, GetScreenHeight() - 110.0f} , 0.3f), 
    credit("creditButton", {100.0f, GetScreenHeight() - 110.0f}, 0.3f) {}

void TitleScreen::draw() {
    DrawTexture(textures["title_screen"], 0, 0, WHITE);
    DrawTexture(textures["guiRayMarioLogo"], 10, 10, WHITE);
    startButton.draw();
    credit.draw();

}

ButtonTexture& TitleScreen::getStartButton() {
    return startButton;
}

ButtonTexture& TitleScreen::getCreditButton() {
    return credit;
}
