#pragma once

#include "Button.h"
#include "Drawable.h"
#include "raylib.h"
#include <map>


class TitleScreen : public virtual Drawable {
    Button startButton;
    ButtonText credit;
    std::map<std::string, Texture2D>& textures;

    public:
    TitleScreen();
    ~TitleScreen() override = default;
    void draw() override;
    Button& getStartButton();
};