//#include "BanzaiBill.h"
#include "BlueKoopaTroopa.h"
#include "BobOmb.h"
#include "BulletBill.h"
#include "BuzzyBeetle.h"
//#include "CloudBlock.h"
//#include "Coin.h"
//#include "CourseClearToken.h"
//#include "ExclamationBlock.h"
//#include "EyesClosedBlock.h"
//#include "EyesOpenedBlock.h"
#include "FlyingGoomba.h"
#include "GameWorld.h"
//#include "GlassBlock.h"
#include "Goomba.h"
#include "GreenKoopaTroopa.h"
//#include "InvisibleBlock.h"
//#include "Item.h"
#include "JumpingPiranhaPlant.h"
#include "Map.h"
//#include "MessageBlock.h"
#include "MontyMole.h"
#include "MummyBeetle.h"
#include "Muncher.h"
#include "PiranhaPlant.h"
//#include "QuestionBlock.h"
//#include "QuestionFireFlowerBlock.h"
//#include "QuestionMushroomBlock.h"
//#include "QuestionOneUpMushroomBlock.h"
//#include "QuestionStarBlock.h"
//#include "QuestionThreeUpMoonBlock.h"
#include "raylib.h"
#include "RedKoopaTroopa.h"
#include "ResourceManager.h"
#include "Rex.h"
#include "Sprite.h"
//#include "StoneBlock.h"
#include "Swooper.h"
//#include "utils.h"
//#include "WoodBlock.h"
#include "YellowKoopaTroopa.h"
//#include "YoshiCoin.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


Map::Map(Mario& mario, int id, bool loadTestMap, GameWorld* gw) :

    id(id),
    maxId(3),

    maxWidth(0),
    maxHeight(0),

    mario(mario),
    marioOffset(0),
    backgroundId(1),
    maxBackgroundId(10),
    backgroundColor(WHITE),
    backgroundTexture(Texture()),
    drawBlackScreen(false),
    drawBlackScreenFadeAcum(0),
    drawBlackScreenFadeTime(1.5),

    //tileSetId(1),
    //maxTileSetId(4),

    musicId(0),
    maxMusicId(9),

    //parseBlocks(parseBlocks),
    //parseItems(parseItems),
    //parseBaddies(parseBaddies),

    loadTestMap(loadTestMap),
    parsed(false),

    camera(nullptr),
    gw(gw) {
}

Map::~Map() {

    for (const auto& tile : untouchableTiles) {
        delete tile;
    }

    for (const auto& tile : touchableTiles) {
        delete tile;
    }

    // for (const auto& backScenarioTile : backScenarioTiles) {
    //     delete backScenarioTile;
    // }

    // for (const auto& frontScenarioTile : frontScenarioTiles) {
    //     delete frontScenarioTile;
    // }

    //for (const auto& item : items) {
    //    delete item;
    //}

    //for (const auto& staticItem : staticItems) {
    //    delete staticItem;
    //}

    for (const auto& baddie : baddies) {
       delete baddie;
    }

    //for (const auto& block : blocks) {
    //    delete block;
    //}

}

void Map::loadFromJsonFile(bool shouldLoadTestMap) {
    //clear current map
    //reset();

    if (parsed) {
        return;
    }

    loadTestMap = shouldLoadTestMap;

    std::string jsonFilePath;
    if (loadTestMap) {
        jsonFilePath = "../resource/maps/test.json";
    } 
    
    else {
        jsonFilePath = "../resource/maps/map" + std::to_string(id) + ".json";
    }

    std::ifstream fin(jsonFilePath);
    if (!fin){
        std::cout << "Cannot open " << jsonFilePath << std::endl;
        return;
    }

    nlohmann::json mapJson;
    try {
        fin >> mapJson;
    } catch (const std::exception& e) {
        std::cout << "Error parsing JSON file " << jsonFilePath << ": " << e.what() << std::endl;
        return;
    }

    // Check if required fields exist
    if (!mapJson.contains("width") || !mapJson.contains("height") || 
        !mapJson.contains("layers") || mapJson["layers"].empty() ||
        !mapJson["layers"][0].contains("data")) {
        std::cout << "JSON file " << jsonFilePath << " is missing required fields" << std::endl;
        return;
    }

    int width = mapJson["width"];
	int height = mapJson["height"];
    maxWidth = width * TILE_WIDTH;
    maxHeight = height * TILE_WIDTH;

    // Set background ID
    if (mapJson.contains("backgroundId")) {
        backgroundId = mapJson["backgroundId"];
        if (backgroundId < 0) {
            backgroundId = 0;
        } else if (backgroundId > maxBackgroundId) {
            backgroundId = maxBackgroundId;
        }
    } else {
        backgroundId = 1; // Default value
    }

    // Set music ID
    if (mapJson.contains("musicId")) {
        musicId = mapJson["musicId"];
        if (musicId < 0) {
            musicId = 0;
        } else if (musicId > maxMusicId) {
            musicId = maxMusicId;
        }
    }
    // musicId is already set to map id by default in constructor

    // Load tile IDs and dimensions
	int tilewidth = mapJson["tilewidth"];
    std::vector<int> tileIDsUntouchable = mapJson["layers"][0]["data"];

    // Load IDs as data
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int tileID = tileIDsUntouchable[y * width + x];
            if (tileID != 0){
                untouchableTiles.push_back(new Tile(Vector2{1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, Vector2{TILE_WIDTH * 1.0f, TILE_WIDTH * 1.0f},
                BLACK, "tile_" + std::to_string(tileID), true, TILE_TYPE_NON_SOLID));
            }
        }
    } 
    tileIDsUntouchable.clear();

    // Load touchable tiles
    std::vector<int> tileIDsTouchable = mapJson["layers"][2]["data"];

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int tileID = tileIDsTouchable[y * width + x];
            if (tileID != 0){
                touchableTiles.push_back(new Tile(Vector2{1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, Vector2{TILE_WIDTH * 1.0f, TILE_WIDTH * 1.0f},
                BLACK, "tile_" + std::to_string(tileID), true));
            }
        }
    }
    tileIDsTouchable.clear();

    // Load back baddies
    std::vector<int> backBaddieIDs = mapJson["layers"][1]["data"];

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int baddieID = backBaddieIDs[y * width + x];
            if (baddieID != 146 && baddieID != 136) continue;

            Baddie* newBaddie;

            if (baddieID == 146) { // Piranha Plant
                newBaddie = new PiranhaPlant({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,66}, RED);
            } 

            else if (baddieID == 136) { // Jumping Piranha Plant
                newBaddie = new JumpingPiranhaPlant({1.0f * x * TILE_WIDTH + 16, 1.0f * y * TILE_WIDTH + 34}, {32,42}, RED);
            } 

            backBaddies.push_back(newBaddie);
            baddies.push_back(newBaddie);
        }
    }

    backBaddieIDs.clear();

    // Load front baddies
    std::vector<int> frontBaddieIDs = mapJson["layers"][5]["data"];

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int baddieID = frontBaddieIDs[y * width + x];
            if (baddieID == 0 || baddieID < 121 || baddieID > 158) continue;

            Baddie* newBaddie;
            if (baddieID == 121) {
                newBaddie = new BlueKoopaTroopa({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,54}, {-100,0}, BLUE);
            } 

            else if (baddieID == 123) {
                newBaddie = new BobOmb({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {24,30}, {-100,0}, BLACK);
            } 

            else if (baddieID == 125) { //bullet bill
                newBaddie = new BulletBill({1.0f * x * TILE_WIDTH, 1.0f * (y+2) * TILE_WIDTH }, {32,28}, {-200,0}, BLACK);
            } 

            else if (baddieID == 126) { // buzzy beetle
                newBaddie = new BuzzyBeetle({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,32}, {-80,0}, BLUE);
            }

            else if (baddieID == 128) { //flying gomba
                newBaddie = new FlyingGoomba({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {66,50}, {-100,0}, MAROON);
            } 

            else if (baddieID == 132) { //goomba
                newBaddie = new Goomba({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,30}, {-100,0}, MAROON);
            } 

            else if (baddieID == 134) { //green koopa troopa
                newBaddie = new GreenKoopaTroopa({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,54}, {-100,0}, GREEN);
            } 

            else if (baddieID == 140) { //monty mole
                newBaddie = new MontyMole({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,30}, {-200,0}, BROWN);
            } 

            else if (baddieID == 142) { // mummy beetle
                newBaddie = new MummyBeetle({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,32}, {-80,0}, GRAY);
            } 

            else if (baddieID == 144) { //muncher
                newBaddie = new Muncher({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,30}, BROWN);
            }

            else if (baddieID == 148) { // red koopa troopa
                newBaddie = new RedKoopaTroopa({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,54}, {-100,0}, RED);
            } 

            else if (baddieID == 152) { //rex
                newBaddie = new Rex({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {40,64}, {-100,0}, VIOLET);
            }

            else if (baddieID == 154) { //swooper
                newBaddie = new Swooper({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,34}, {-100,0}, BLACK);
            } 

            else if (baddieID == 157) { // yellow koopa troopa
                newBaddie = new YellowKoopaTroopa({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {32,54}, {-100,0}, YELLOW);
            } 
            
            else {
                std::cerr << "Unsupported baddie ID: " << baddieID << " at position (" << x << ", " << y << ")" << std::endl;
                continue; // Skip unsupported baddies
            }
            frontBaddies.push_back(newBaddie);
            baddies.push_back(newBaddie);
        }
    }

    parsed = true;

    fin.close();
}

void Map::draw() {

    DrawRectangle(0, 0, maxWidth, maxHeight, backgroundColor);

    std::string backgroundTextureKey = "background" + std::to_string(backgroundId);
    if (ResourceManager::getInstance().getTextures().find(backgroundTextureKey) != ResourceManager::getInstance().getTextures().end()) {
        backgroundTexture = ResourceManager::getInstance().getTexture(backgroundTextureKey);
    } else {
        backgroundTexture = Texture2D(); // Fallback to an empty texture if not found
        std::cerr << "Background texture not found: " << backgroundTextureKey << std::endl;
    }

    if (backgroundId > 0) {
        const int repeats = maxWidth / backgroundTexture.width + 2;
        for (int i = 0; i <= repeats; i++) {
            DrawTexture(
                backgroundTexture,
                -backgroundTexture.width + i * backgroundTexture.width - marioOffset * 0.06,
                maxHeight - backgroundTexture.height,
                WHITE);
        }
    }

    //for (const auto& backScenarioTile : backScenarioTiles) {
    //    backScenarioTile->draw();
    //}

    for (const auto& baddie : backBaddies) {
       baddie->draw();
    }

    for (const auto& tile : untouchableTiles) {
        tile->draw();
    }

    

    for (const auto& block : blocks) {
       block->draw();
    }

    for (const auto& item : items) {
       item->draw();
    }

    for (const auto& staticItem : staticItems) {
       staticItem->draw();
    }

    for (const auto& baddie : frontBaddies) {
       baddie->draw();
    }

    mario.draw();

    for (const auto& tile : touchableTiles) {
        tile->draw();
    }

    if (drawBlackScreen) {
       if (drawBlackScreenFadeAcum < drawBlackScreenFadeTime) {
           drawBlackScreenFadeAcum += GetFrameTime();
       }
       DrawRectangle(0, 0, maxWidth, maxHeight, Fade(BLACK, 0.5 * drawBlackScreenFadeAcum / drawBlackScreenFadeTime));
    }
}

std::vector<Tile*>& Map::getTiles() {
    return touchableTiles;
}

std::vector<Block*>& Map::getBlocks() {
    return blocks;
}

std::vector<Item*>& Map::getItems() {
   return items;
}

std::vector<Item*>& Map::getStaticItems() {
   return staticItems;
}

std::vector<Baddie*>& Map::getBaddies() {
   return baddies;
}

void Map::playMusic() const {

   if (musicId != 0) {

       std::map<std::string, Music> musics = ResourceManager::getInstance().getMusics();
       const std::string key(TextFormat("music%d", musicId));

       if (mario.isInvincible()) {
           if (IsMusicStreamPlaying(musics[key])) {
               StopMusicStream(musics[key]);
           }
           if (!IsMusicStreamPlaying(musics["invincible"])) {
               PlayMusicStream(musics["invincible"]);
               SeekMusicStream(musics["invincible"], 1);
           }
           else {
               UpdateMusicStream(musics["invincible"]);
           }
       }
       else {
           if (!IsMusicStreamPlaying(musics[key])) {
               StopMusicStream(musics["invincible"]);
               PlayMusicStream(musics[key]);
           }
           else {
               UpdateMusicStream(musics[key]);
           }
       }

   }
}


float Map::getMaxWidth() const {
    return maxWidth;
}

float Map::getMaxHeight() const {
    return maxHeight;
}

void Map::setMarioOffset(float marioOffset) {
   this->marioOffset = marioOffset;
}

void Map::setDrawBlackScreen(bool drawBlackScreen) {
   this->drawBlackScreen = drawBlackScreen;
}

void Map::setCamera(Camera2D* camera) {
    this->camera = camera;
}

void Map::setGameWorld(GameWorld* gw) {
    this->gw = gw;
}

int Map::getId() const {
    return id;
}

void Map::reset() {

    maxWidth = 0;
    maxHeight = 0;
    marioOffset = 0;
    drawBlackScreen = false;
    drawBlackScreenFadeAcum = 0;

    for (const auto& tile : untouchableTiles) {
        delete tile;
    }
    untouchableTiles.clear();

    for (const auto& tile : touchableTiles) {
        delete tile;
    }
    touchableTiles.clear();

    for (const auto& block : blocks) {
       delete block;
    }
    blocks.clear();

    for (const auto& item : items) {
       delete item;
    }
    items.clear();

    for (const auto& staticItem : staticItems) {
       delete staticItem;
    }
    staticItems.clear();

    for (const auto& baddie : baddies) {
       delete baddie;
    }
    baddies.clear();
    frontBaddies.clear();
    backBaddies.clear();

    StopMusicStream(ResourceManager::getInstance().getMusics()[std::string(TextFormat("music%d", musicId))]);
    parsed = false;
    loadFromJsonFile();

}

bool Map::hasNext() {

    id++;

    if (id > maxId) {
        return false;
    }
    else {
        reset();
    }

    return true;

}

void Map::first() {
    id = 1;
}

//void Map::pauseGameToShowMessage() const {
//    gw->pauseGame(false, false, false, true);
//}

void Map::eraseBaddieFromDrawingVectors(Baddie* baddie) {

   for (size_t i = 0; i < frontBaddies.size(); i++) {
       if (frontBaddies[i] == baddie) {
           frontBaddies.erase(frontBaddies.begin() + i);
           return;
       }
   }

   for (size_t i = 0; i < backBaddies.size(); i++) {
       if (backBaddies[i] == baddie) {
           backBaddies.erase(backBaddies.begin() + i);
           return;
       }
   }
}
