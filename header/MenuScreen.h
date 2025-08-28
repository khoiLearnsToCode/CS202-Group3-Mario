#pragma once

#include "Button.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "raylib.h"

#include <vector>
#include <unordered_map>

class MenuScreen : public Screen {
    std::unordered_map<std::string, Button*> buttons;

    Texture2D backgroundTexture;

    public:
        MenuScreen();
        ~MenuScreen() override;

        void draw() override;
        std::unordered_map<std::string, Button*>& getButtons();
        Button* getButton(const std::string& key) const;

};
