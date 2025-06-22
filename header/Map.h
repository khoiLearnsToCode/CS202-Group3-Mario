#pragma once

class Block;
class GameWorld;

//#include "Baddie.h"
//#include "Block.h"
//#include "Drawable.h"
//#include "Item.h"
//#include "Mario.h"
#include "raylib.h"
#include "json.hpp"
#include "Tile.h"
#include <vector>

class Map : public virtual Drawable {

    std::vector<Tile*> tiles;
    std::vector<Tile*> backScenarioTiles;
    std::vector<Tile*> frontScenarioTiles;
    std::vector<Block*> blocks;
    std::vector<Block*> messageBlocks;
    //std::vector<Item*> items;
    //std::vector<Item*> staticItems;
    //std::vector<Baddie*> baddies;
    //std::vector<Baddie*> frontBaddies;  // auxiliary drawing vector for map placement
    //std::vector<Baddie*> backBaddies;   // auxiliary drawing vector for map placement

	static const std::string MAPS_FOLDER_PREFIX;

    int id;
    int maxId;

    float maxWidth;
    float maxHeight;

    //Mario& mario;
    float marioOffset;

    int backgroundId;
    int maxBackgroundId;
    int tileSetId;
    //int maxTileSetId;
    //int musicId;
    //int maxMusicId;
    Color backgroundColor;
    Texture2D backgroundTexture;
    //bool drawBlackScreen;
    //float drawBlackScreenFadeAcum;
    //float drawBlackScreenFadeTime;

    

    //bool parseBlocks;
    //bool parseItems;
    //bool parseBaddies;

    bool loadTestMap;
    bool parsed;

    //bool drawMessage;
    //std::string message;
    Camera2D* camera;
    GameWorld* gw;

public:

    static constexpr int TILE_WIDTH = 16;

    Map(Mario& mario, int id, bool loadTestMap, bool parseBlocks, bool parseItems, bool parseBaddies, GameWorld* gw);
    ~Map() override;
    void draw() override;

    //void parseMap();
    void loadFromJsonFile(const std::string& jsonFilePath);

    void setMarioOffset(float marioOffset);
    void setDrawBlackScreen(bool drawBlackScreen);
    void setDrawMessage(bool drawMessage);
    void setMessage(std::string message);
    void setCamera(Camera2D* camera);
    void setGameWorld(GameWorld* gw);

    std::vector<Tile*>& getTiles();
    std::vector<Block*>& getBlocks();
    //std::vector<Item*>& getItems();
    //std::vector<Item*>& getStaticItems();
    //std::vector<Baddie*>& getBaddies();
    float getMaxWidth() const;
    float getMaxHeight() const;

    //void playMusic() const;
    void reset();
    bool next();
    void first();
    //void pauseGameToShowMessage() const;

    //void eraseBaddieFromDrawingVectors(Baddie* baddie);

};