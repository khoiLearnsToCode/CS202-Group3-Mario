#pragma once

#include <raylib.h>
#include <iostream>

#include "ResourceManager.h"
#include "Drawable.h"

#define HOVER_SPEED 10.0f
#define HOVER_SCALE 1.075f

class Button : public virtual Drawable {
    protected:
    Vector2 pos;
    Rectangle hitbox;
    float btnScale;

    virtual void update() = 0;

    public:
        Button() = default;
        Button(Vector2 position, float scale);
        virtual ~Button() = default;
        virtual void draw() = 0;
        virtual bool isPressed();
        virtual bool isReleased();
};


class ButtonTexture : public Button {
    Texture2D btnTexture;
    void update() override;

    public:
        ButtonTexture() = default;
        ButtonTexture(const char* key, Vector2 position, float scale);
        ~ButtonTexture() override;
        void draw() override;

};

class ButtonText : public Button {
       
    virtual void update() override;

    protected:
        std::string text;
        Color textColor;
        float fontSize;
        Font font;

    public:
        ButtonText() = default;
        ButtonText(const std::string& text, Vector2 position, float scale, Color color, Font font, float fontSize);
        virtual ~ButtonText() = default;
        virtual void draw() override;
};

class ButtonTextTexture : public ButtonText {
    Texture2D btnTexture1;
    Texture2D btnTexture2;
    Texture2D* currentTexture;
    const Vector2 truePos;

    void update() override;

    public:
        ButtonTextTexture() = default;
        ButtonTextTexture(const std::string& text, const char* key, Vector2 position, float scale, Color color, Font font, float fontSize);
        ButtonTextTexture(const char* key, Vector2 position, float scale);
        ~ButtonTextTexture() override;
        void draw() override;
};