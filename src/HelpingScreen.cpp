#include "HelpingScreen.h"

HelpingScreen::HelpingScreen() : Screen(), 
                                 font(ResourceManager::getInstance().getFont("SuperMario256")),
                                 fontSize(20.0f) {

    Image img = LoadImageFromTexture(textures["helpBackground"]);
    ImageResize(&img, img.width * 0.5f, img.height * 0.5f);
    backgroundTexture = LoadTextureFromImage(img);
    UnloadImage(img);

    border = { (GetScreenWidth() - backgroundTexture.width) / 2.0f, 
               (GetScreenHeight() - backgroundTexture.height) / 2.0f, 
               (float)backgroundTexture.width, 
               (float)backgroundTexture.height };   
}

HelpingScreen::~HelpingScreen() {
    UnloadTexture(backgroundTexture);
}

void HelpingScreen::draw() {
    DrawTexture(backgroundTexture, border.x, border.y, WHITE);
    DrawTextEx(font, "or", { border.x + 180, border.y + 147 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": move left", { border.x + 260, border.y + 147 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, "or", { border.x + 180, border.y + 207 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": move right", { border.x + 260, border.y + 207 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, "or", { border.x + 180, border.y + 267 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": dodge", { border.x + 260, border.y + 267 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": pause", { border.x + 260, border.y + 327 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": help", { border.x + 260, border.y + 389 }, fontSize, 0.0f, BLACK);

}

bool HelpingScreen::helpingBoardShouldClose() const {
    Vector2 mousePos = GetMousePosition();
    return (!CheckCollisionPointRec(mousePos, border)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
