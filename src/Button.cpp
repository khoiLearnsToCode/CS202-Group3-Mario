#include "Button.h"

Button::Button(Vector2 position, float scale) : pos(position), btnScale(1.0f) {
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;
    std::cout << "Position: " << pos.x << ", " << pos.y << std::endl;
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

bool Button::isReleased() {
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, hitbox)) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    }
    return false;
}

ButtonTexture::ButtonTexture(const char* key, Vector2 position, float scale) : Button(position, scale) {
    Image img = LoadImageFromTexture(ResourceManager::getInstance().getTexture(key)); 
    ImageResize(&img, img.width * scale, img.height * scale); 
    btnTexture = LoadTextureFromImage(img); 
    UnloadImage(img); 

    hitbox = { pos.x, pos.y, (float)btnTexture.width, (float)btnTexture.height };
}

ButtonTexture::~ButtonTexture() {
    UnloadTexture(btnTexture);
}

void ButtonTexture::draw() {
    update();
    DrawTextureEx(btnTexture, pos, 0.0f, btnScale, WHITE);
}

void ButtonTexture::update() {
    
    Vector2 mousePos = GetMousePosition();

    bool isHovering = CheckCollisionPointRec(mousePos, hitbox);

    btnScale += (isHovering ? (HOVER_SCALE - btnScale) : (1.0f - btnScale)) * HOVER_SPEED * GetFrameTime();
}

ButtonText::ButtonText(const std::string& text, Vector2 position, float scale, Color color, Font font, float fontSize) : 
    Button(position, scale), text(text), textColor(color), font(font), fontSize(fontSize) {
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1);
        hitbox = { pos.x, pos.y, textSize.x, textSize.y};
}

void ButtonText::draw() {
    update();
    DrawTextEx(font, text.c_str(), pos, fontSize * btnScale, 1, textColor);
}

void ButtonText::update() {
    Vector2 mousePos = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mousePos, hitbox);

    btnScale += (isHovering ? (HOVER_SCALE - btnScale) : (1.0f - btnScale)) * HOVER_SPEED * GetFrameTime();
}

ButtonTextTexture::ButtonTextTexture(const std::string& text, const char* key, Vector2 position, float scale, Color color, Font font, float fontSize): 
    ButtonText(text, position, scale, color, font, fontSize), currentTexture(nullptr), truePos(position) {

    Image img1 = LoadImageFromTexture(ResourceManager::getInstance().getTexture(std::string(key) + "Release")); 
    ImageResize(&img1, img1.width * scale, img1.height * scale); 
    btnTexture1 = LoadTextureFromImage(img1); 
    currentTexture = &btnTexture1; 
    UnloadImage(img1); 
    
    Image img2 = LoadImageFromTexture(ResourceManager::getInstance().getTexture(std::string(key) + "Press")); 
    ImageResize(&img2, img2.width * scale, img2.height * scale); 
    btnTexture2 = LoadTextureFromImage(img2); 
    UnloadImage(img2); 
    
    hitbox = { pos.x, pos.y, (float)btnTexture1.width, (float)btnTexture1.height };
    
}

ButtonTextTexture::ButtonTextTexture(const char* key, Vector2 position, float scale) : 
    ButtonText("", position, scale, WHITE, GetFontDefault(), 40.0f), currentTexture(nullptr), truePos(position) {

    Image img1 = LoadImageFromTexture(ResourceManager::getInstance().getTexture(std::string(key) + "Release")); 
    ImageResize(&img1, img1.width * scale, img1.height * scale); 
    btnTexture1 = LoadTextureFromImage(img1); 
    currentTexture = &btnTexture1; 
    UnloadImage(img1); 

    Image img2 = LoadImageFromTexture(ResourceManager::getInstance().getTexture(std::string(key) + "Press")); 
    ImageResize(&img2, img2.width * scale, img2.height * scale); 
    btnTexture2 = LoadTextureFromImage(img2); 
    UnloadImage(img2); 

    hitbox = { pos.x, pos.y, (float)btnTexture1.width, (float)btnTexture1.height };
}

ButtonTextTexture::~ButtonTextTexture() {
    UnloadTexture(btnTexture1);
    UnloadTexture(btnTexture2);
}

void ButtonTextTexture::draw() {
    update();  
    //std::cout << "Position: " << pos.x << ", " << pos.y << std::endl;
    //std:: cout << "Hitbox: " << hitbox.x << ", " << hitbox.y << ", " << hitbox.width << ", " << hitbox.height << std::endl;
    //std::cout << "text: " << text << std::endl;
    //std::cout << "fontSize: " << fontSize << std::endl;
    DrawTextureEx(*currentTexture, pos, 0.0f, 1.0f, WHITE);
    Vector2 textPos = { pos.x + (currentTexture->width - MeasureTextEx(font, text.c_str(), fontSize, 1).x) / 2.0f, pos.y + (currentTexture->height - fontSize) / 2.0f };
    DrawTextEx(font, text.c_str(), textPos, fontSize, 0, textColor);
}

void ButtonTextTexture::update() {
    Vector2 mousePos = GetMousePosition();
    
    if (CheckCollisionPointRec(mousePos, hitbox)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            currentTexture = &btnTexture2;
            pos.y = truePos.y + 2.0f;
        } else {
            currentTexture = &btnTexture1;
            pos.y = truePos.y;
        }
    } 
    
    else {
        currentTexture = &btnTexture1;
        pos.y = truePos.y;
    }
}