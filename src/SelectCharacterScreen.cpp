#include "SelectCharacterScreen.h"
#include "ResourceManager.h"

SelectCharacterScreen::SelectCharacterScreen() 
    : Screen(),
    marioButton("Mario", {100, 100}, 0.7f),
    luigiButton("Luigi", {600, 100}, 0.7f),
    homeButton("homeButton", { 100.0f, GetScreenHeight() - 100.0f }, 2.0f)
{}


void SelectCharacterScreen::update() {}

void SelectCharacterScreen::draw() {
    // Draw background
    DrawTexture(textures["select_character_screen"], 0, 0, WHITE);
    // Draw buttons
    marioButton.draw();
    luigiButton.draw();
    homeButton.draw();
}

ButtonTexture& SelectCharacterScreen::getMarioButton() {
    return marioButton;
}

ButtonTexture& SelectCharacterScreen::getLuigiButton() {
    return luigiButton;
}

ButtonTextTexture& SelectCharacterScreen::getHomeButton() {
    return homeButton;
}
