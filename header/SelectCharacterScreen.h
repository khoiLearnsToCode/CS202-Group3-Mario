#pragma once

#include "Screen.h"
#include "Button.h"

class SelectCharacterScreen : public Screen {
private:
    ButtonTexture marioButton;
    ButtonTexture luigiButton;
    ButtonTextTexture homeButton;

public:
    SelectCharacterScreen();
    ~SelectCharacterScreen() override = default;

    SelectCharacterScreen(const SelectCharacterScreen&) = delete;
    SelectCharacterScreen& operator=(const SelectCharacterScreen&) = delete;

    void update();
    void draw() override;

    ButtonTexture& getMarioButton();
    ButtonTexture& getLuigiButton();
    ButtonTextTexture& getHomeButton();
};
