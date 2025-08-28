#pragma once

#include "Screen.h"
#include "raylib.h"
#include "ResourceManager.h"

class HelpingScreen : public Screen {
public:
    HelpingScreen();
    HelpingScreen(const HelpingScreen&) = delete;
    HelpingScreen& operator=(const HelpingScreen&) = delete;
    ~HelpingScreen() override;

    void draw() override;
    bool helpingBoardShouldClose() const;

    private:
    Texture2D backgroundTexture;
    Texture2D helpingLogo;
    Rectangle border;
    Font& font;
    float fontSize;
};
