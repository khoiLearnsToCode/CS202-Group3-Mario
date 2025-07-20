#pragma once

#include "Screen.h"
#include "raylib.h"
#include "ResourceManager.h"

class HelpingScreen : public Screen {
public:
    HelpingScreen();
    virtual ~HelpingScreen() override;

    virtual void draw() override;
    bool helpingBoardShouldClose() const;

    private:
    Texture2D backgroundTexture;
    Rectangle border;
    Font& font;
    float fontSize;
};
