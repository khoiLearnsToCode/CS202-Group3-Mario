#pragma once

#include <raylib.h>
#include <iostream>

#include "ResourceManager.h"
#include "Drawable.h"

#define HOVER_SPEED 10.0f
#define HOVER_SCALE 1.075f


class Button : public virtual Drawable {
    Vector2 pos;
    Texture2D btnTexture;
    float btnScale;
    void update();
    Rectangle hitbox;

    public:
        Button() = default;
        Button(const char* key, Vector2 position, float scale);
        ~Button();
        void draw() override;
        bool isPressed();

};

class ButtonText : public virtual Drawable {
    std::string text;
    Vector2 pos;
    Color textColor;
    float fontSize;
    float btnScale;
    Font font;
    Rectangle hitbox;
    void update();


    public:
        ButtonText() = default;
        ButtonText(const std::string& text, Vector2 position, float scale, Color color, Font font, float fontSize);
        ~ButtonText() = default;
        void draw() override;
        bool isPressed() ;
};