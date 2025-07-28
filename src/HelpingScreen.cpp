#include "HelpingScreen.h"

HelpingScreen::HelpingScreen() : Screen(), 
                                 font(ResourceManager::getInstance().getFont("SuperMario256")),
                                 fontSize(20.0f) {

    Image img = LoadImageFromTexture(textures["helpBackground"]);
    ImageResize(&img, img.width * 0.5f, img.height * 0.5f);
    backgroundTexture = LoadTextureFromImage(img);
    UnloadImage(img);

    Image helpingLogoImage = LoadImageFromTexture(textures["help"]);
    ImageResize(&helpingLogoImage, helpingLogoImage.width * 0.25f, helpingLogoImage.height * 0.25f);
    helpingLogo = LoadTextureFromImage(helpingLogoImage);
    UnloadImage(helpingLogoImage);

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

    // draw helping logo in the top middle of the border
    Vector2 logoPos = { border.x + (border.width - helpingLogo.width) / 2.0f, 
                        border.y - 60.0f };
    DrawTexture(helpingLogo, logoPos.x, logoPos.y, WHITE);

    DrawTextEx(font, "or", { border.x + 180, border.y + 147 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": move left", { border.x + 260, border.y + 147 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, "or", { border.x + 180, border.y + 207 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": move right", { border.x + 260, border.y + 207 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, "or", { border.x + 180, border.y + 267 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": dodge", { border.x + 260, border.y + 267 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": pause", { border.x + 260, border.y + 327 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": help", { border.x + 260, border.y + 390 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": jump", { border.x + 585, border.y + 147 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": shoot", { border.x + 585, border.y + 207 }, fontSize, 0.0f, BLACK);
    DrawTextEx(font, ": sprint", { border.x + 585, border.y + 267 }, fontSize, 0.0f, BLACK);

}

bool HelpingScreen::helpingBoardShouldClose() const {
    Vector2 mousePos = GetMousePosition();
    return (!CheckCollisionPointRec(mousePos, border)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
