#include "SelectCharacterScreen.h"
#include "ResourceManager.h"

SelectCharacterScreen::SelectCharacterScreen() 
    : Screen(),
    marioButton("Mario", {900, 285}, 0.5f),
    luigiButton("Luigi", {300, 180}, 0.7f),
    homeButton("homeButton", { 100.0f, GetScreenHeight() - 100.0f }, 2.0f)
{
    Image img = LoadImageFromTexture(textures["selectYourCharacter"]);
    ImageResize(&img, img.width * 1.2f, img.height * 1.2f);
    selectYourCharacterLogo = LoadTextureFromImage(img);
    UnloadImage(img);
}


void SelectCharacterScreen::update() {}

void SelectCharacterScreen::draw() {
    // Draw background
    DrawTexture(textures["select_character_screen"], 0, 0, WHITE);

    // Draw select your character logo
    Vector2 logoPos = { (GetScreenWidth() - selectYourCharacterLogo.width) / 2.0f, 0.0f };
    DrawTexture(selectYourCharacterLogo, logoPos.x, logoPos.y, WHITE);
    // Draw buttons
    marioButton.draw();
    luigiButton.draw();
    homeButton.draw();
}

ButtonTexture& SelectCharacterScreen::getMarioButton() {
    return marioButton;
}

ButtonTexture& SelectCharacterScreen::getLuigiButton() {
    return luigiButton;
}

ButtonTextTexture& SelectCharacterScreen::getHomeButton() {
    return homeButton;
}
