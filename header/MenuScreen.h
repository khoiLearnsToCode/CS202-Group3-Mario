#pragma once

#include "Button.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "raylib.h"

#include <vector>

class MenuScreen : public Screen {
    std::vector<Button*> buttons;
    // 0 for new game
    // 1 for load game
    // 2 for settings
    // 3 for return to title screen

    Texture2D backgroundTexture;

    public:
        MenuScreen();
        ~MenuScreen() override;

        void draw() override;

};
