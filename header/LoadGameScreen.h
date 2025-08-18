#pragma once

#include "Screen.h"
#include "Button.h"
#include "ResourceManager.h"
#include "GameState.h"
#include "raylib.h"
#include "Memento.h"

#include <unordered_map>
#include <vector>
#include <string>

class GameWorld;

class LoadGameScreen : public Screen {
public:
    LoadGameScreen(GameWorld* gw = nullptr);
    ~LoadGameScreen() override;

    LoadGameScreen(const LoadGameScreen&) = delete;
    LoadGameScreen& operator=(const LoadGameScreen&) = delete;

    void update();
    void draw() override;

    bool loadBoardShouldClose() const;

    void setLoadBoardIsOpenInMenuScreen(bool isOpen);
private:
    std::unordered_map<std::string, Button*> buttons;
    Texture2D backgroundTexture;
    Texture2D loadLogo;
    Rectangle border;

    GameWorld* gw;
    std::vector<Memento*> savedMementos;
    std::vector<std::string> displayStrings;
    std::vector<const char*> items; 

    int selectedIndex;
    bool loadBoardIsOpenInMenuScreen;
};