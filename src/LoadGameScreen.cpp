#include "LoadGameScreen.h"
#include "GameWorld.h"
#include "GuardScreen.h"
#include "CareTaker.h"
#include "raygui.h"

LoadGameScreen::LoadGameScreen(GameWorld* gw) : Screen(), gw(gw), selectedIndex(-1), loadBoardIsOpenInMenuScreen(true) {

    Image img = LoadImageFromTexture(textures["settingBackground"]); 
    ImageResize(&img, img.width * 0.5f, img.height * 0.5f);
    backgroundTexture = LoadTextureFromImage(img);
    UnloadImage(img);

    
    Image loadLogoImage = LoadImageFromTexture(textures["loadGame"]);  
    ImageResize(&loadLogoImage, loadLogoImage.width * 0.4f, loadLogoImage.height * 0.4f);
    loadLogo = LoadTextureFromImage(loadLogoImage);
    UnloadImage(loadLogoImage);

    border = { (GetScreenWidth() - backgroundTexture.width) / 2.0f,
               (GetScreenHeight() - backgroundTexture.height) / 2.0f,
               (float)backgroundTexture.width,
               (float)backgroundTexture.height };

    
    GuiLoadStyle("../resource/font/candy.rgs");    
}

LoadGameScreen::~LoadGameScreen() {
    for (auto& buttonPair : buttons) {
        delete buttonPair.second;
    }
    UnloadTexture(backgroundTexture);
    UnloadTexture(loadLogo);
    // Clean up copied mementos
    for (auto memento : savedMementos) {
        delete memento;
    }
    savedMementos.clear();
}

void LoadGameScreen::update() {
    // Handle HOME and RESET buttons
    if (GameWorld::state != GAME_STATE_GUARD_SCREEN) {
        if (buttons["HOME"]->isReleased()) {
            loadBoardIsOpenInMenuScreen = false;
            if (gw) {
                gw->showGuardScreen(GUARD_ACTION_HOME);
            }
            else {
                std::cerr << "GameWorld pointer is null. Cannot show guard screen." << std::endl;
            }
        }

        if (buttons["RESET"]->isReleased()) {
            loadBoardIsOpenInMenuScreen = false;
            if (gw) {
                gw->showGuardScreen(GUARD_ACTION_RESET);
            }
            else {
                std::cerr << "GameWorld pointer is null. Cannot show guard screen." << std::endl;
            }
        }
    }

    // Handle selection confirmation (e.g., via Enter key or button press)
    if (selectedIndex >= 0 && IsKeyPressed(KEY_ENTER)) {
        if (gw && gw->getCareTaker()) {
            gw->getCareTaker()->restore(selectedIndex);
            loadBoardIsOpenInMenuScreen = false;
            gw->loadBoardIsOpen = false; 
            gw->unpauseGame();
        }
    }
}

void LoadGameScreen::draw() {
    DrawTexture(backgroundTexture, border.x, border.y, WHITE);

    // Draw load game logo in the top middle of the border
    Vector2 logoPos = { border.x + (border.width - loadLogo.width) / 2.0f,
                        border.y - 60.0f };
    DrawTexture(loadLogo, logoPos.x, logoPos.y, WHITE);

    for (const auto& buttonPair : buttons) {
        if ((buttonPair.first == "HOME" || buttonPair.first == "RESET") && loadBoardIsOpenInMenuScreen) {
            continue;
        }
        buttonPair.second->draw();
    }

    // Draw the list of saved games using GuiListView
    Rectangle listRec = { border.x + 100, border.y + 100, border.width - 200, border.height - 200 };
    if (savedMementos.empty()) {
        Font& font = ResourceManager::getInstance().getFont("SuperMario256");
        DrawTextEx(font, "No saved games available.", { listRec.x + 20, listRec.y + 20 }, 25.0f, 0.0f, BLACK);
    }
    else {
        if (gw && gw->getCareTaker()) {
            savedMementos = gw->getCareTaker()->getSavedData();
            for (const auto& memento : savedMementos) {
                displayStrings.push_back(memento->display());
                items.push_back(displayStrings.back().c_str());
            }
        }
        int scrollIndex = 0;  
        selectedIndex = GuiListViewEx(listRec, items.data(), items.size(), nullptr, &scrollIndex, &selectedIndex);
    }
}

bool LoadGameScreen::loadBoardShouldClose() const {
    Vector2 mousePos = GetMousePosition();
    return (!CheckCollisionPointRec(mousePos, border)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void LoadGameScreen::setLoadBoardIsOpenInMenuScreen(bool isOpen) {
    loadBoardIsOpenInMenuScreen = isOpen;
}