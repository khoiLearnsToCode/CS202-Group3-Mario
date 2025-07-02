#include "MenuScreen.h"

MenuScreen::MenuScreen() : Screen() {
    buttons.push_back(new ButtonTextTexture("NEW GAME", "longButton", {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}, 1.0f, WHITE, GetFontDefault(), 20));
    buttons.push_back(new ButtonTextTexture("LOAD GAME", "longButton", {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f + 50}, 1.0f, WHITE, GetFontDefault(), 20));
    buttons.push_back(new ButtonTextTexture("SETTINGS", "longButton", {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f + 100}, 1.0f, WHITE, GetFontDefault(), 20));
    buttons.push_back(new ButtonTexture("exitButton", { 100, 250 }, 1.0f));

    backgroundTexture = ResourceManager::getInstance().getTexture("menuBackground");
}

MenuScreen::~MenuScreen() {
    for (Button* button : buttons) {
        delete button;
        button = nullptr;
    }
    buttons.clear();
}

void MenuScreen::draw() {
    // Draw background texture
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    // Draw buttons
    for (Button* button : buttons) {
        button->draw();
    }

}