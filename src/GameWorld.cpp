#include "GameWorld.h"
#include "Button.h"
#include "Item.h"

GameState GameWorld::state = GAME_STATE_TITLE_SCREEN;
float GameWorld::gravity = 20;

GameWorld::GameWorld() :
    mario( 
        {0,0},          // position
        {32, 40},       // dimention
        {0, 0},         // velocity
        {0, 0, 0, 255}, // color
        260,            // speedX
        360,            // maxSpeedX
        -600,           // jumpSpeed
        false           
    ),
    map(mario, 1, true, this),
    camera(nullptr),
    settingBoardIsOpen(false),
    helpingBoardIsOpen(false),
    stateBeforePause(GAME_STATE_TITLE_SCREEN),
    remainingTimePointCount(400),
    pauseMusic(false),
    pauseMario(false),
    titleScreen(nullptr),
    menuScreen(nullptr),
    settingScreen(nullptr)
    {
        mario.setGameWorld(this);
        mario.setMap(&map);
    }

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
    Data data(map.getId(), remainingTimePointCount, mario.getPoints(), mario.getLives());
    return new ConcreteMemento(data);
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

    map.loadFromJsonFile();

    std::map<std::string, Sound>& sounds = ResourceManager::getInstance().getSounds();
    std::map<std::string, Music>& musics = ResourceManager::getInstance().getMusics();

    std::vector<Tile*>& Tiles = map.getTiles();
    std::vector<Block*>& Blocks = map.getBlocks();
    std::vector<Baddie*>& Baddies = map.getBaddies();
    std::vector<Item*>& Items = map.getItems();
    std::vector<Item*>& StaticItems = map.getStaticItems();

    // std::cerr << "Mario state: " << mario.getState() << std::endl;
    // std::cerr << "GameWorld state: " << state << std::endl;
    if ( mario.getState() != SPRITE_STATE_DYING && 
         mario.getState() != SPRITE_STATE_VICTORY &&
         mario.getState() != SPRITE_STATE_WAITING_TO_NEXT_MAP &&
         state != GAME_STATE_TITLE_SCREEN &&
         state != GAME_STATE_MENU_SCREEN &&
         state != GAME_STATE_CREDITS_SCREEN &&
         state != GAME_STATE_FINISHED && 
         !pauseMusic ) {
        map.playMusic();
    }

    if ( state != GAME_STATE_TITLE_SCREEN &&
         state != GAME_STATE_MENU_SCREEN &&
         state != GAME_STATE_CREDITS_SCREEN &&
         state != GAME_STATE_FINISHED &&
         state != GAME_STATE_SETTINGS_SCREEN &&
         state != GAME_STATE_HELPING_SCREEN ) {
        mario.setActivationWidth( GetScreenWidth() * 2 );
        mario.update();
    } else if ( !pauseMario && state != GAME_STATE_TITLE_SCREEN && 
        state != GAME_STATE_MENU_SCREEN && 
        state != GAME_STATE_CREDITS_SCREEN) {

        mario.update();
    }

    if ( mario.getState() != SPRITE_STATE_DYING && 
         mario.getState() != SPRITE_STATE_VICTORY &&
         mario.getState() != SPRITE_STATE_WAITING_TO_NEXT_MAP &&
         state != GAME_STATE_TITLE_SCREEN &&
         state != GAME_STATE_MENU_SCREEN &&
         state != GAME_STATE_CREDITS_SCREEN &&
         state != GAME_STATE_FINISHED &&
         state != GAME_STATE_SETTINGS_SCREEN &&
         state != GAME_STATE_HELPING_SCREEN ) {}

    else if (mario.getState() == SPRITE_STATE_DYING) {
            if ( !mario.isPlayerDownMusicStreamPlaying() && !mario.isGameOverMusicStreamPlaying() ) {
            
            if ( mario.getLives() > 0 ) {
                resetMap();
            } else if ( mario.getLives() < 0 ) {
                resetGame();
            } else {
                mario.playGameOverMusicStream();
                state = GAME_STATE_GAME_OVER;
                mario.setLives( -1 );
            }
        }
    }

    else if ( mario.getState() == SPRITE_STATE_VICTORY ) {}

    else if (state == GAME_STATE_COUNTING_POINTS) {}

    else if (state == GAME_STATE_IRIS_OUT) {}

    else if (state == GAME_STATE_GO_TO_NEXT_MAP) {}

    else if ( state == GAME_STATE_SETTINGS_SCREEN ) {
        if (IsKeyPressed(KEY_P)) {
            unpauseGame();
        }
    }

    else if ( state == GAME_STATE_HELPING_SCREEN ) {
        if (IsKeyPressed(KEY_H)) {
            unpauseGame();
        }
    }


    if (settingBoardIsOpen){
        settingScreen->update();
        settingScreen->updateVolume(); 

        if (settingScreen->settingBoardShouldClose()) {
        settingBoardIsOpen = false;
        }
    }

    if ( mario.getState() != SPRITE_STATE_DYING && 
         mario.getY() > map.getMaxHeight() ) {

        mario.setState( SPRITE_STATE_DYING );
        mario.playPlayerDownMusicStream();
        mario.removeLives( 1 );
    }

    const float xc = GetScreenWidth() / 2.0;
    const float yc = GetScreenHeight() / 2.0;
    const float pxc = mario.getX() + mario.getWidth() / 2.0;
    const float pyc = mario.getY() + mario.getHeight() / 2.0;
    
    camera->offset.x = xc;

    if ( pxc < xc ) {
        camera->target.x = xc + Map::TILE_WIDTH;
        map.setMarioOffset( 0 );         // x parallax
    } else if ( pxc >= map.getMaxWidth() - xc - Map::TILE_WIDTH ) {
        camera->target.x = map.getMaxWidth() - GetScreenWidth();
        camera->offset.x = 0;
    } else {
        camera->target.x = pxc + Map::TILE_WIDTH;
        map.setMarioOffset( pxc - xc );  // x parallax
    }

    camera->offset.y = yc;

    if ( pyc < yc ) {
        camera->target.y = yc + Map::TILE_WIDTH;
    } else if ( pyc >= map.getMaxHeight() - yc - Map::TILE_WIDTH ) {
        camera->target.y = map.getMaxHeight() - GetScreenHeight();
        camera->offset.y = 0;
    } else {
        camera->target.y = pyc + Map::TILE_WIDTH;
    }

    if (state == GAME_STATE_TITLE_SCREEN || 
        state == GAME_STATE_MENU_SCREEN || 
        state == GAME_STATE_CREDITS_SCREEN) {

        if (!IsMusicStreamPlaying(musics["title"])) {
            PlayMusicStream(musics["title"]);
        } else {
            UpdateMusicStream(musics["title"]);
        }

        if (state == GAME_STATE_TITLE_SCREEN) {

            if (titleScreen->getStartButton().isReleased()) {
                state = GAME_STATE_MENU_SCREEN;
            }

            else if (titleScreen->getCreditButton().isReleased()) {
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

            if (settingBoardIsOpen){
                return;
            }

            if (menuScreen->getButton("NEW GAME")->isReleased()) {
                mario.resetAll();
                map.first();
                map.reset();
                map.loadFromJsonFile(); // Load the map immediately
                // Set Mario to a proper starting position (e.g., 100 pixels from left, near ground level)
                mario.setPos(100.0f, 400.0f);
                // Set the time limit (400 seconds = typical Mario game time)
                mario.setMaxTime(400.0f);

                if (IsMusicStreamPlaying(musics["title"])) {
                    StopMusicStream(musics["title"]);
                }            
                state = GAME_STATE_PLAYING;
            }

            else if (menuScreen->getButton("LOAD GAME")->isReleased()) {
                // implement later
                std::cout << "Load Game button pressed. Implement load game functionality later." << std::endl;
            }

            else if (menuScreen->getButton("SETTINGS")->isReleased()) {
                settingBoardIsOpen = true;
            }

            else if (menuScreen->getButton("EXIT")->isReleased()) {
                state = GAME_STATE_TITLE_SCREEN;
            }
        }
    }

    else if ( state == GAME_STATE_GAME_OVER ) {
        mario.playGameOverMusicStream();
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
        BeginMode2D(*camera);
        map.draw();
        EndMode2D();
        mario.drawHud();
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
    mario.reset(true);
    map.reset();
    state = GAME_STATE_PLAYING;
}

void GameWorld::resetGame() {
    mario.resetAll();
    map.first();
    map.reset();
    state = GAME_STATE_TITLE_SCREEN;
}

void GameWorld::nextMap() {
    if (map.hasNext()) {
        state = GAME_STATE_PLAYING;
        // reset state but keep the power-up
        mario.reset(false);
    } else {
        state = GAME_STATE_FINISHED;
    }
}

void GameWorld::pauseGame(bool playPauseSFX, bool pauseMusic, bool pauseMario, bool showSettingBoard, bool showHelpingBoard) {
    if (playPauseSFX) {
        PlaySound(ResourceManager::getInstance().getSounds()["pause"]);
    }
    this->pauseMusic = pauseMusic;
    this->pauseMario = pauseMario;
    settingBoardIsOpen = showSettingBoard;
    helpingBoardIsOpen = showHelpingBoard;
    this->stateBeforePause = state;

    state = settingBoardIsOpen ? GAME_STATE_SETTINGS_SCREEN : GAME_STATE_HELPING_SCREEN; 
}

void GameWorld::unpauseGame() {
    state = stateBeforePause;
    pauseMusic = false;
    pauseMario = false;
    settingBoardIsOpen = false;
    helpingBoardIsOpen = false;
}

