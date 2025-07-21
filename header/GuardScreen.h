#pragma once

#include "Screen.h"
#include "raylib.h"
#include "ResourceManager.h"
#include "Button.h"

enum GuardAction {
    GUARD_ACTION_HOME,
    GUARD_ACTION_RESET
};

class GuardScreen : public Screen {
    private:
    Texture2D backgroundTexture;
    Rectangle border;
    Font& font;
    float fontSize;
    Button* cancelButton;
    Button* acceptButton;
    const std::vector<std::string> messages;
    GuardAction currentAction;

    public:
    GuardScreen();
    GuardScreen(const GuardScreen&) = delete;
    GuardScreen& operator=(const GuardScreen&) = delete;
    ~GuardScreen() override;


    void draw() override;
    bool guardBoardShouldClose() const;
    void setAction(GuardAction action);
    GuardAction getCurrentAction() const;

    Button* getCancelButton() const;
    Button* getAcceptButton() const;
};