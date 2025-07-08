#pragma once

#include "Button.h"
#include "Drawable.h"
#include "raylib.h"
#include "Screen.h"
#include <map>


class TitleScreen : public Screen {
    ButtonTexture startButton;
    ButtonText credit;
    ButtonTextTexture temp;

    public:
    TitleScreen();
    ~TitleScreen() override = default;
    TitleScreen(const TitleScreen&) = delete;
    TitleScreen& operator=(const TitleScreen&) = delete;
    
    void draw() override;
    ButtonTexture& getStartButton();
    ButtonText& getCreditButton();
};