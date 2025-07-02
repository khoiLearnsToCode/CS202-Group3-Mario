#include "MenuScreen.h"

MenuScreen::MenuScreen() : Screen() {
    Font font = ResourceManager::getInstance().getFont("fixedsys");

    // (const std::string& text, const char* key, Vector2 position, float scale, Color color, Font font, float fontSize)
    buttons.emplace("NEW GAME", new ButtonTextTexture("NEW GAME", "longButton", {GetScreenWidth() / 2.0f - 128.0f, GetScreenHeight() / 2.0f}, 2.0f, WHITE, font, 40.0f));
    buttons.emplace("LOAD GAME", new ButtonTextTexture("LOAD GAME", "longButton", {GetScreenWidth() / 2.0f - 128.0f, GetScreenHeight() / 2.0f + 75.0f}, 2.0f, WHITE, font, 40.0f));
    buttons.emplace("SETTINGS", new ButtonTextTexture("SETTINGS", "longButton", {GetScreenWidth() / 2.0f - 128.0f, GetScreenHeight() / 2.0f + 150.0f}, 2.0f, WHITE, font, 40.0f));
    buttons.emplace("EXIT", new ButtonTextTexture("","homeButton", { 100.0f, GetScreenHeight() - 100.0f }, 2.0f, WHITE, font, 40.0f));

    backgroundTexture = ResourceManager::getInstance().getTexture("title_screen");
}

MenuScreen::~MenuScreen() {
    for (auto& [key, button] : buttons) {
        delete button;
        button = nullptr;
    }
    buttons.clear();
}

void MenuScreen::draw() {
    // Draw background texture
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    // Draw faded background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLUE, 0.3f));

    // Draw buttons
    for (auto& [key, button] : buttons) {
        button->draw();
    }
}

std::unordered_map<std::string, Button*>& MenuScreen::getButtons() {
    return buttons;
}

Button* MenuScreen::getButton(const std::string& key) const {
    auto it = buttons.find(key);
    if (it != buttons.end()) {
        return it->second;
    }
    return nullptr; // or throw an exception if preferred
}