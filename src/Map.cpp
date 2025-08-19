//#include "BanzaiBill.h"
#include "BlueKoopaTroopa.h"
#include "BobOmb.h"
#include "BulletBill.h"
#include "BuzzyBeetle.h"
#include "Block.h"
#include "Item.h"
#include "FlyingGoomba.h"
#include "GameWorld.h"
#include "Goomba.h"
#include "GreenKoopaTroopa.h"
#include "Item.h"
#include "JumpingPiranhaPlant.h"
#include "Map.h"
#include "MontyMole.h"
#include "MummyBeetle.h"
#include "Muncher.h"
#include "PiranhaPlant.h"
#include "raylib.h"
#include "RedKoopaTroopa.h"
#include "ResourceManager.h"
#include "Rex.h"
#include "Sprite.h"
#include "Swooper.h"
#include "YellowKoopaTroopa.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

const std::vector<Color> Map::backgroundColorPallete = {
    { 62, 191, 255, 255 },  // Sky blue
    { 255, 255, 225, 255 }, // Light yellow
    { 2, 38, 83, 255 },     // Dark blue
};

Map::Map(Player& player, int id, bool loadTestMap, GameWorld* gw) :

    id(id),
    maxId(3),

    maxWidth(0),
    maxHeight(0),

    player(player),
    playerOffset(0),
    backgroundId(1),
    maxBackgroundId(3),
    backgroundColor(WHITE),
    backgroundTexture(Texture()),
    drawBlackScreen(false),
    drawBlackScreenFadeAcum(0),
    drawBlackScreenFadeTime(1.5),

    // Near sight vision effect for map3
    lastValidPlayerPos({0, 0}),

    //tileSetId(1),
    //maxTileSetId(4),

    musicId(1),
    maxMusicId(9),

    //parseBlocks(parseBlocks),
    //parseItems(parseItems),
    //parseBaddies(parseBaddies),

    loadTestMap(loadTestMap),
    loadFromUserDesignedMap(false),
    currentData(nullptr),
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

    for (const auto& item : items) {
       delete item;
    }

    for (const auto& staticItem : staticItems) {
       delete staticItem;
    }

    for (const auto& baddie : baddies) {
       delete baddie;
    }

    for (const auto& block : blocks) {
       delete block;
    }

}

void Map::loadFromJsonFile(bool shouldLoadTestMap) {
    //clear current map
    //reset();

    if (parsed) {
        return;
    }

    if (loadFromUserDesignedMap) {
        loadUserData();
        return;
    }

    loadTestMap = shouldLoadTestMap;
    player.setPos(100, 1688);

    std::string jsonFilePath;
    if (loadTestMap) {
        jsonFilePath = "../resource/maps/test.json";
    } 
    
    else {
        jsonFilePath = "../resource/maps/map" + std::to_string(id) + ".json";
        backgroundId = id;
        backgroundColor = backgroundColorPallete[id - 1];
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
    }
    // backgroundId is already set to map id by default if not loading test map

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

    frontBaddieIDs.clear();

    // Load blocks
    std::vector<int> blockIDs = mapJson["layers"][3]["data"];
    std::vector<int> itemIDs = mapJson["layers"][4]["data"];
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int blockID = blockIDs[y * width + x];
            if (blockID == 0) continue;

            if (blockID < 88 || blockID > 102) {
                std::cerr << "Unsupported block ID: " << blockID << " at position (" << x << ", " << y << ")" << std::endl;
                continue; // Skip unsupported blocks
            }

            Block* newBlock = nullptr;

            if (blockID == 88) {
                newBlock = new CloudBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            }

            else if (blockID == 89) {
                newBlock = new ExclamationBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            }

            else if (blockID == 90) {
                newBlock = new EyesClosedBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            }

            else if (blockID == 91 || blockID == 92 || blockID == 93 || blockID == 94) {
                newBlock = new EyesOpenedBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            }

            else if (blockID == 95) {
                newBlock = new GrassBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            }

            else if (blockID == 96) {
                std::cerr << "Unsupport message block\n";
            }

            // Handle question blocks and their items
            else if (blockID == 97 || blockID == 98 || blockID == 99 || blockID == 100) {
                int itemID = itemIDs[y * width + x];
                if (itemID == 0) {
                    newBlock = new QuestionBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE, 0.1f, 4);
                }

                else if (itemID == 103) {
                    newBlock = new QuestionOneUpMushroomBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE, 0.1f, 4);
                }

                else if (itemID == 104) {
                    newBlock = new QuestionThreeUpMoonBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE, 0.1f, 4);
                }

                else if (itemID == 113 || itemID == 114 ) {
                    newBlock = new QuestionFireFlowerBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE, 0.1f, 4);
                }

                else if (itemID == 115) {
                    newBlock = new QuestionMushroomBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE, 0.1f, 4);
                }

                else if (itemID == 116) {
                    newBlock = new QuestionStarBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE, 0.1f, 4);
                }

                else {
                    std::cerr << "Unsupported item ID: " << itemID << " at position (" << x << ", " << y << ")" << std::endl;
                    continue; // Skip unsupported items
                }
            }

            else if (blockID == 101) {
                newBlock = new StoneBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            }

            else if (blockID == 102) {
                newBlock = new WoodBlock({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            }

            if (newBlock) {
                blocks.push_back(newBlock);
            }            
        }
    }
    
    blockIDs.clear();

    // Load static items
    // std::vector<int> staticItemIDs = mapJson["layers"][4]["data"];
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int staticItemID = itemIDs[y * width + x];
            if (staticItemID == 0) continue;

            Item* newStaticItem = nullptr;

            if (staticItemID >= 108 && staticItemID <= 111) { // Coins
                newStaticItem = new Coin({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            } 

            else if (staticItemID == 112) { // Course Clear Token
                newStaticItem = new CourseClearToken({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            } 

            else if (staticItemID >= 117 && staticItemID <= 120) { // Yoshi Coins
                newStaticItem = new YoshiCoin({1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH}, {TILE_WIDTH, TILE_WIDTH}, WHITE);
            } 

            else {
                std::cerr << "Unsupported static item ID: " << staticItemID << " at position (" << x << ", " << y << ")" << std::endl;
                continue; // Skip unsupported static items

            }

            if (newStaticItem) {
                staticItems.push_back(newStaticItem);
            }
        }
    }

    itemIDs.clear();
 
    parsed = true;

    fin.close();
}

void Map::loadUserData() {

    if (parsed) {
        return;
    }

    if (!currentData) {
        std::cerr << "No user data to load." << std::endl;
        return;
    }

    backgroundColor = currentData->backgroundColor;
    backgroundId = currentData->backgroundID;
    id = 100;
    
    // User-designed maps are 200x60 tiles (12000 elements total)
    int width = 200;
    int height = 60;
    maxWidth = width * TILE_WIDTH;
    maxHeight = height * TILE_WIDTH;
    
    // Set default player position
    player.setPos(10, 10);


    // Set music ID to default (could be extended to be configurable)
    musicId = 1;

    // Load entities from the single layer - all will be touchable tiles
    const std::vector<int>& entitiesID = currentData->entitiesID;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int entityID = entitiesID[y * width + x];
            if (entityID == 0) continue;

            Vector2 pos = {1.0f * x * TILE_WIDTH, 1.0f * y * TILE_WIDTH};
            Vector2 dim = {TILE_WIDTH * 1.0f, TILE_WIDTH * 1.0f};

            // Handle different entity types based on ID ranges
            
            // Tiles (make all as touchable tiles since it's single layer)
            if (entityID >= 1 && entityID <= 87) {
                touchableTiles.push_back(new Tile(pos, dim, BLACK, "tile_" + std::to_string(entityID), true));
            }
            
            // Blocks (88-102)
            else if (entityID >= 88 && entityID <= 102) {
                Block* newBlock = nullptr;

                if (entityID == 88) {
                    newBlock = new CloudBlock(pos, dim, WHITE);
                } else if (entityID == 89) {
                    newBlock = new ExclamationBlock(pos, dim, WHITE);
                } else if (entityID == 90) {
                    newBlock = new EyesClosedBlock(pos, dim, WHITE);
                } else if (entityID >= 91 && entityID <= 94) {
                    newBlock = new EyesOpenedBlock(pos, dim, WHITE);
                } else if (entityID == 95) {
                    newBlock = new GrassBlock(pos, dim, WHITE);
                } else if (entityID >= 97 && entityID <= 100) {
                    // Question blocks - default to regular question block
                    newBlock = new QuestionBlock(pos, dim, WHITE, 0.1f, 4);
                } else if (entityID == 101) {
                    newBlock = new StoneBlock(pos, dim, WHITE);
                } else if (entityID == 102) {
                    newBlock = new WoodBlock(pos, dim, WHITE);
                }

                if (newBlock) {
                    blocks.push_back(newBlock);
                }
            }
            
            // Items that should become question blocks (103-104, 113-116)
            else if (entityID == 103 || entityID == 104 || (entityID >= 113 && entityID <= 116)) {
                Block* newBlock = nullptr;

                if (entityID == 103) {
                    newBlock = new QuestionOneUpMushroomBlock(pos, dim, WHITE, 0.1f, 4);
                } else if (entityID == 104) {
                    newBlock = new QuestionThreeUpMoonBlock(pos, dim, WHITE, 0.1f, 4);
                } else if (entityID == 113 || entityID == 114) {
                    newBlock = new QuestionFireFlowerBlock(pos, dim, WHITE, 0.1f, 4);
                } else if (entityID == 115) {
                    newBlock = new QuestionMushroomBlock(pos, dim, WHITE, 0.1f, 4);
                } else if (entityID == 116) {
                    newBlock = new QuestionStarBlock(pos, dim, WHITE, 0.1f, 4);
                }

                if (newBlock) {
                    blocks.push_back(newBlock);
                }
            }
            
            // Static Items (108-112, 117-120) - only coins, course clear token, and yoshi coins remain as static
            else if ((entityID >= 108 && entityID <= 112) || (entityID >= 117 && entityID <= 120)) {
                Item* newStaticItem = nullptr;

                if (entityID >= 108 && entityID <= 111) {
                    newStaticItem = new Coin(pos, dim, WHITE);
                } else if (entityID == 112) {
                    newStaticItem = new CourseClearToken(pos, dim, WHITE);
                } else if (entityID >= 117 && entityID <= 120) {
                    newStaticItem = new YoshiCoin(pos, dim, WHITE);
                }

                if (newStaticItem) {
                    staticItems.push_back(newStaticItem);
                }
            }
            
            // Baddies (121-158)
            else if (entityID >= 121 && entityID <= 158) {
                Baddie* newBaddie = nullptr;

                if (entityID == 121) {
                    newBaddie = new BlueKoopaTroopa(pos, {32,54}, {-100,0}, BLUE);
                } else if (entityID == 123) {
                    newBaddie = new BobOmb(pos, {24,30}, {-100,0}, BLACK);
                } else if (entityID == 125) { // bullet bill
                    newBaddie = new BulletBill({pos.x, pos.y + 2 * TILE_WIDTH}, {32,28}, {-200,0}, BLACK);
                } else if (entityID == 126) { // buzzy beetle
                    newBaddie = new BuzzyBeetle(pos, {32,32}, {-80,0}, BLUE);
                } else if (entityID == 128) { // flying goomba
                    newBaddie = new FlyingGoomba(pos, {66,50}, {-100,0}, MAROON);
                } else if (entityID == 132) { // goomba
                    newBaddie = new Goomba(pos, {32,30}, {-100,0}, MAROON);
                } else if (entityID == 134) { // green koopa troopa
                    newBaddie = new GreenKoopaTroopa(pos, {32,54}, {-100,0}, GREEN);
                } else if (entityID == 136) { // jumping piranha plant
                    newBaddie = new JumpingPiranhaPlant({pos.x + 16, pos.y + 34}, {32,42}, RED);
                } else if (entityID == 140) { // monty mole
                    newBaddie = new MontyMole(pos, {32,30}, {-200,0}, BROWN);
                } else if (entityID == 142) { // mummy beetle
                    newBaddie = new MummyBeetle(pos, {32,32}, {-80,0}, GRAY);
                } else if (entityID == 144) { // muncher
                    newBaddie = new Muncher(pos, {32,30}, BROWN);
                } else if (entityID == 146) { // piranha plant
                    newBaddie = new PiranhaPlant(pos, {32,66}, RED);
                } else if (entityID == 148) { // red koopa troopa
                    newBaddie = new RedKoopaTroopa(pos, {32,54}, {-100,0}, RED);
                } else if (entityID == 152) { // rex
                    newBaddie = new Rex(pos, {40,64}, {-100,0}, VIOLET);
                } else if (entityID == 154) { // swooper
                    newBaddie = new Swooper(pos, {32,34}, {-100,0}, BLACK);
                } else if (entityID == 157) { // yellow koopa troopa
                    newBaddie = new YellowKoopaTroopa(pos, {32,54}, {-100,0}, YELLOW);
                }

                if (newBaddie) {
                    baddies.push_back(newBaddie);
                    frontBaddies.push_back(newBaddie); // Add to front baddies for drawing
                }
            }
        }
    }

    parsed = true;
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
                -backgroundTexture.width + i * backgroundTexture.width - playerOffset * 0.06,
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

    for (const auto& tile : touchableTiles) {
        tile->draw();
    
    }
    
    player.draw();

    Vector2 pos{10.0f, GetScreenHeight() - 20.0f};
    Vector2 drawPos = GetScreenToWorld2D(pos, *camera);
    
    if (id == 3) {  // Near sight vision effect for map3
        // Update last valid Player position when he's not dying
        if (player.getState() != SPRITE_STATE_DYING) {
            lastValidPlayerPos = player.getPos();
        }
        
        for (float radius = 0; radius < GetScreenWidth() * 1.5f; radius += GetScreenWidth() / 100.0f) {
            DrawRing(lastValidPlayerPos, radius, GetScreenWidth() * 1.5f, 0.0f, 360.0f, 32, Fade(BLACK, 0.07f));
        }
    }

    DrawFPS(drawPos.x, drawPos.y);

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

       std::map<std::string, Music>& musics = ResourceManager::getInstance().getMusics();
       const std::string key(TextFormat("music%d", musicId));

       if (player.isInvincible()) {
           // Stop regular music if playing
           if (IsMusicStreamPlaying(musics[key])) {
               StopMusicStream(musics[key]);
           }
           // Play invincible music if not already playing
           if (!IsMusicStreamPlaying(musics["invincible"])) {
               PlayMusicStream(musics["invincible"]);
               SeekMusicStream(musics["invincible"], 1);
           } else {
               UpdateMusicStream(musics["invincible"]);
           }
       }
       else {
           // Stop invincible music if playing
           if (IsMusicStreamPlaying(musics["invincible"])) {
               StopMusicStream(musics["invincible"]);
           }
           // Play regular music if not already playing
           if (!IsMusicStreamPlaying(musics[key])) {
               PlayMusicStream(musics[key]);
           } else {
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

void Map::setPlayerOffset(float playerOffset) {
   this->playerOffset = playerOffset;
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

void Map::setCurrentData(UserMapData* data) {
    this->currentData = data;
}

void Map::setLoadFromUserDesignedMap(bool loadFromUser) {
    this->loadFromUserDesignedMap = loadFromUser;
    reset();
}

int Map::getId() const {
    return id;
}

void Map::reset() {

    maxWidth = 0;
    maxHeight = 0;
    playerOffset = 0;
    drawBlackScreen = false;
    drawBlackScreenFadeAcum = 0;

    // Reset near sight vision position
    lastValidPlayerPos = {0, 0};

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

    // Stop all music streams properly before resetting
    std::map<std::string, Music>& musics = ResourceManager::getInstance().getMusics();
    StopMusicStream(musics[std::string(TextFormat("music%d", musicId))]);
    StopMusicStream(musics["invincible"]);
    
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
