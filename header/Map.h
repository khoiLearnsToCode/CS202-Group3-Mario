#pragma once

class Block;
class GameWorld;
struct UserMapData;

#include "raylib.h"
#include "json.hpp"
#include "Tile.h"
#include "Player.h"
#include "Baddie.h"
#include "Tile.h"
#include "Block.h"
#include "Item.h"
#include "Data.h"
#include <vector>

class Map : public virtual Drawable {

    std::vector<Tile*> untouchableTiles;
    std::vector<Tile*> touchableTiles;
    // std::vector<Tile*> frontScenarioTiles;
    std::vector<Block*> blocks;
    std::vector<Item*> items;
    std::vector<Item*> staticItems;
    std::vector<Baddie*> baddies;
    std::vector<Baddie*> frontBaddies;  // auxiliary drawing vector for map placement
    std::vector<Baddie*> backBaddies;   // auxiliary drawing vector for map placement

    int id;
    int maxId;

    float maxWidth;
    float maxHeight;

    Player& player;
    float playerOffset;

    int backgroundId;
    int maxBackgroundId;
    int tileSetId;
    //int maxTileSetId;
    int musicId;
    int maxMusicId;
    Color backgroundColor;
    Texture2D backgroundTexture;
    bool drawBlackScreen;
    float drawBlackScreenFadeAcum;
    float drawBlackScreenFadeTime;

    // Near sight vision effect for map3
    Vector2 lastValidPlayerPos;

    bool loadTestMap;
    bool loadFromUserDesignedMap;
    bool parsed;
    UserMapData* currentData;

    //bool drawMessage;
    //std::string message;
    Camera2D* camera;
    GameWorld* gw;

    void loadUserData();
public:

    static constexpr int TILE_WIDTH = 32;

    Map(Player& player, int id, bool loadTestMap, GameWorld* gw);
    ~Map() override;
    void draw() override;

    //void parseMap();
    void loadFromJsonFile(bool shouldLoadTestMap = false);

    void setPlayerOffset(float playerOffset);
    void setDrawBlackScreen(bool drawBlackScreen);
    void setCamera(Camera2D* camera);
    void setGameWorld(GameWorld* gw);
    void setCurrentData(UserMapData* data);
    void setLoadFromUserDesignedMap(bool loadFromUser);

    std::vector<Tile*>& getTiles();
    std::vector<Block*>& getBlocks();
    std::vector<Item*>& getItems();
    std::vector<Item*>& getStaticItems();
    std::vector<Baddie*>& getBaddies();
    static const std::vector<Color> backgroundColorPallete;

    float getMaxWidth() const;
    float getMaxHeight() const;

    int getId() const;

    void playMusic() const;

// Reset the map to its initial state
    void reset();

// Returns true if there is a next map to load and jump to next map
    bool hasNext();
    void first();
    void second();
	void third();

    void eraseBaddieFromDrawingVectors(Baddie* baddie);

};