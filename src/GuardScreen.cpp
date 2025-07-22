#include "GuardScreen.h"
#include <string>

GuardScreen::GuardScreen() : Screen(), 
                             font(ResourceManager::getInstance().getFont("SuperMario256")),
                             fontSize(19.0f), 
                            cancelButton(nullptr),
                            acceptButton(nullptr),
                             messages{
                                "Are you sure you want to", 
                                " return to home?",
                                " restart this level?",
                                " save your progress and exit?",
                                "All of your progress", 
                                " in this level",
                                " will be lost!",
                                },
                            currentAction(GUARD_ACTION_HOME)

{
    Image img = LoadImageFromTexture(textures["settingBackground"]);
    ImageResize(&img, img.width * 0.5f, img.height * 0.5f);
    backgroundTexture = LoadTextureFromImage(img);
    UnloadImage(img);

    border = { (GetScreenWidth() - backgroundTexture.width) / 2.0f, 
               (GetScreenHeight() - backgroundTexture.height) / 2.0f, 
               (float)backgroundTexture.width, 
               (float)backgroundTexture.height }; 
               
    cancelButton = new ButtonTextTexture("cancelButton", {GetScreenWidth() / 2.0f - 50.0f - 64.0f, border.y + 300}, 2.0f);
    acceptButton = new ButtonTextTexture("acceptButton", {GetScreenWidth() / 2.0f + 50.0f, border.y + 300}, 2.0f);
}

GuardScreen::~GuardScreen() {
    UnloadTexture(backgroundTexture);
    delete cancelButton;
    delete acceptButton;
}

void GuardScreen::draw() {
    DrawTexture(backgroundTexture, border.x, border.y, WHITE);
    
    // Draw different messages based on current action
    float messageY = border.y + 200.0f; // Starting Y position for messages
    
    if (currentAction == GUARD_ACTION_HOME) {
        // "Are you sure you want to return to home?"
        Vector2 textDimensions1 = MeasureTextEx(font, (messages[0] + messages[1]).c_str(), fontSize, 0.0f);
        DrawTextEx(font, (messages[0] + messages[1]).c_str(), 
                   {(GetScreenWidth() - textDimensions1.x) / 2.0f, messageY}, fontSize, 0.0f, BLACK);
        
        // "All of your progress will be lost!"
        Vector2 textDimensions2 = MeasureTextEx(font, (messages[4] + messages[6]).c_str(), fontSize, 0.0f);
        DrawTextEx(font, (messages[4] + messages[6]).c_str(), 
                   {(GetScreenWidth() - textDimensions2.x) / 2.0f, messageY + 30.0f}, fontSize, 0.0f, BLACK);
    }
    else if (currentAction == GUARD_ACTION_RESET) {
        // "Are you sure you want to restart this level?"
        Vector2 textDimensions1 = MeasureTextEx(font, (messages[0] + messages[2]).c_str(), fontSize, 0.0f);
        DrawTextEx(font, (messages[0] + messages[2]).c_str(), 
                   {(GetScreenWidth() - textDimensions1.x) / 2.0f, messageY}, fontSize, 0.0f, BLACK);

        // "All of your progress in this level will be lost!"
        Vector2 textDimensions2 = MeasureTextEx(font, (messages[4] + messages[5] + messages[6]).c_str(), fontSize, 0.0f);
        DrawTextEx(font, (messages[4] + messages[5] + messages[6]).c_str(), 
                   {(GetScreenWidth() - textDimensions2.x) / 2.0f, messageY + 30.0f}, fontSize, 0.0f, BLACK);
    }

    if (cancelButton) {
        cancelButton->draw();
    }
    if (acceptButton) {
        acceptButton->draw();
    }
}

bool GuardScreen::guardBoardShouldClose() const {
    Vector2 mousePos = GetMousePosition();
    return (!CheckCollisionPointRec(mousePos, border)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void GuardScreen::setAction(GuardAction action) {
    currentAction = action;
}

GuardAction GuardScreen::getCurrentAction() const {
    return currentAction;
}

Button* GuardScreen::getCancelButton() const {
    return cancelButton;
}

Button* GuardScreen::getAcceptButton() const {
    return acceptButton;
}
