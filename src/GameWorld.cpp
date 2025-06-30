#include "GameWorld.h"
#include "Button.h"

GameState GameWorld::state = GAME_STATE_TITLE_SCREEN;
float GameWorld::gravity = 20;

GameWorld::GameWorld() :
    map(1, true, this),
    camera(nullptr),
    remainingTimePointCount(0),
    titleScreen(nullptr) {}

GameWorld::~GameWorld() {
    if (titleScreen != nullptr) {
        delete titleScreen;
        titleScreen = nullptr;
    }
}

void GameWorld::inputAndUpdate() {
    if (state == GAME_STATE_TITLE_SCREEN) {
        if (titleScreen == nullptr) {
            titleScreen = new TitleScreen();
        }
        if (titleScreen->getStartButton().isPressed()) {
            state = GAME_STATE_PLAYING;
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

    // Draw the map
    else {
        map.draw();
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

