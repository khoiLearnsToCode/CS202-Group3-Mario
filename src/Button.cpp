#include "Button.h"

Button::Button(const char* key, Vector2 position, float scale) : pos (position), btnScale(1.0f) {
    Image img = LoadImageFromTexture(ResourceManager::getInstance().getTexture(key)); 
    ImageResize(&img, img.width * scale, img.height * scale); 
    btnTexture = LoadTextureFromImage(img); 
    UnloadImage(img); 

    hitbox = { pos.x, pos.y, (float)btnTexture.width, (float)btnTexture.height };
}

Button::~Button() {
    UnloadTexture(btnTexture);
}

void Button::draw() {
    update();
    DrawTextureEx(btnTexture, pos, 0.0f, btnScale, WHITE);
}

void Button::update() {
    
    Vector2 mousePos = GetMousePosition();

    bool isHovering = CheckCollisionPointRec(mousePos, hitbox);

    btnScale += (isHovering ? (HOVER_SCALE - btnScale) : (1.0f - btnScale)) * HOVER_SPEED * GetFrameTime();
}

bool Button::isPressed() {
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, hitbox)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    }
    return false;
}

ButtonText::ButtonText(const std::string& text, Vector2 position, float scale, Color color, Font font, float fontSize)
    : text(text), pos(position), textColor(color), font(font), fontSize(fontSize), btnScale(1.0f) {
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1);
        hitbox = { pos.x, pos.y, textSize.x, textSize.y};
}

void ButtonText::draw() {
    update();
    DrawTextEx(font, text.c_str(), pos, fontSize * btnScale, 1, textColor);
}

bool ButtonText::isPressed() {
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, hitbox)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    }
    return false;
}

void ButtonText::update() {
    Vector2 mousePos = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mousePos, hitbox);

    btnScale += (isHovering ? (HOVER_SCALE - btnScale) : (1.0f - btnScale)) * HOVER_SPEED * GetFrameTime();
}
