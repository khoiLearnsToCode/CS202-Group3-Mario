#include "GameWorld.h"
#include "Button.h"

GameState GameWorld::state = GAME_STATE_TITLE_SCREEN;
float GameWorld::gravity = 20;

GameWorld::GameWorld() :
    map(1, true, this),
    camera(nullptr),
    settingBoardIsOpen(false),
    remainingTimePointCount(0),
    titleScreen(nullptr),
    menuScreen(nullptr),
    settingScreen(nullptr) {}

GameWorld::~GameWorld() {
    if (titleScreen != nullptr) {
        delete titleScreen;
        titleScreen = nullptr;
    }

    if (menuScreen != nullptr) {
        delete menuScreen;
        menuScreen = nullptr;
    }

    if (settingScreen != nullptr) {
        delete settingScreen;
        settingScreen = nullptr;
    }
}

Memento* GameWorld::dataFromGameWorldToSave() const {
    // Implement later

    // Data data(map.getId(), map.getRemainingTime(), map.getScore(), map.getLives());
    // return new ConcreteMemento(data);
    return nullptr;
}

void GameWorld::restoreDataFromMemento(const Memento* memento) const {
    // Implement later
    // Data data = memento->getData();
    // map.loadFromJsonFile(data.mapID, true);
    // map.setRemainingTime(data.remainingTime);
    // map.setScore(data.score);
    // map.setLives(data.lives);
}

void GameWorld::initScreens() {
    if (titleScreen == nullptr) {
        titleScreen = new TitleScreen();
    }

    if (menuScreen == nullptr) {
        menuScreen = new MenuScreen();
    }

    if (settingScreen == nullptr) {
        settingScreen = new SettingScreen();
    }
}

void GameWorld::inputAndUpdate() {

    if (settingBoardIsOpen){
        settingScreen->update(); 
        return;
    }

    if (state == GAME_STATE_TITLE_SCREEN) {

        if (titleScreen->getStartButton().isReleased()) {
            state = GAME_STATE_MENU_SCREEN;
        }

        if (titleScreen->getCreditButton().isReleased()) {
            state = GAME_STATE_CREDITS_SCREEN;
        }
    }

    else if (state == GAME_STATE_CREDITS_SCREEN) {
        Texture2D& creditTexture = ResourceManager::getInstance().getTexture("credit");
        Rectangle creditRect = { 
            (GetScreenWidth() - creditTexture.width) / 2.0f, 
            (GetScreenHeight() - creditTexture.height) / 2.0f, 
            1.0f * (creditTexture.width), 
            1.0f * (creditTexture.height) 
        };
        if (!CheckCollisionPointRec(GetMousePosition(), creditRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state = GAME_STATE_TITLE_SCREEN;
        }
    }

    else if (state == GAME_STATE_MENU_SCREEN) {

        if (menuScreen->getButton("NEW GAME")->isReleased()) {
            state = GAME_STATE_PLAYING;
        }

        if (menuScreen->getButton("LOAD GAME")->isReleased()) {
            // implement later
            std::cout << "Load Game button pressed. Implement load game functionality later." << std::endl;
        }

        if (menuScreen->getButton("SETTINGS")->isReleased()) {
            settingBoardIsOpen = true;
        }

        if (menuScreen->getButton("EXIT")->isReleased()) {
            state = GAME_STATE_TITLE_SCREEN;
        }
    }

    else
    {
        // Handle input and update the map
        map.loadFromJsonFile(1, true);
    }
      
}

void GameWorld::draw() {
    BeginDrawing();
    ClearBackground(WHITE);
    std::map<std::string, Texture2D>& textures = ResourceManager::getInstance().getTextures();

    if (state == GAME_STATE_TITLE_SCREEN) {
        titleScreen->draw();
    }

    else if (state == GAME_STATE_CREDITS_SCREEN) {
        titleScreen->draw();

        // Draw a faded background
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.3f));

        DrawTexture(textures["credit"], (GetScreenWidth() - textures["credit"].width) / 2, 
                                        (GetScreenHeight() - textures["credit"].height) / 2, WHITE);
    }

    else if (state == GAME_STATE_MENU_SCREEN) {
        menuScreen->draw();
    }

    // Draw the map
    else {
        map.draw();
    }

    if (settingBoardIsOpen) {
        settingScreen->draw();
    }

    

    EndDrawing();
}

void GameWorld::loadResources() {
    ResourceManager::getInstance().loadResources();
}

void GameWorld::unloadResources() {
    ResourceManager::getInstance().unloadResources();
}

void GameWorld::setCamera(Camera2D* camera) {
    this->camera = camera;
    this->map.setCamera(camera);
}

Camera2D* GameWorld::getCamera() const {
    return this->camera;
}

void GameWorld::resetMap() {
    map.reset();
    state = GAME_STATE_PLAYING;
}

void GameWorld::resetGame() {
    //implement later
}

void GameWorld::nextMap() {
    if (map.hasNext()) {
        state = GAME_STATE_PLAYING;
    } else {
        state = GAME_STATE_FINISHED;
    }
}

void GameWorld::pauseGame(bool playPauseSFX, bool pauseMusic, bool showOverlay, bool pauseMarioUpdate) {
    //implement later
}

void GameWorld::unpauseGame() {
    //implement later
}

