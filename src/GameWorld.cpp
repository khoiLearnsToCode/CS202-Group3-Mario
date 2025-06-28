#include "GameWorld.h"

GameState GameWorld::state = GAME_STATE_PLAYING;
float GameWorld::gravity = 20;

GameWorld::GameWorld() :
    map(1, true, this),
    camera(nullptr),
    remainingTimePointCount(0) {}

void GameWorld::inputAndUpdate() {

    // Load map data from JSON file
    map.loadFromJsonFile(1, true);
     
}

void GameWorld::draw() {
    BeginDrawing();
    ClearBackground(BLUE);

    // Draw the map
    map.draw();

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

