#pragma once

#include <iostream>
#include <vector>
#include "Block.h"
#include "Baddie.h"
#include "Item.h"
#include "PlayerType.h"
#include "raylib.h"

struct Data{
    int mapID;
    int score;
    int lives;
    int coins;
    int yoshiCoins;
    int clearanceTime;
    PlayerType playerType;
	Data() : mapID(0), score(0), lives(0), coins(0), yoshiCoins(0), clearanceTime(0), playerType(PlayerType::PLAYER_TYPE_SMALL) {}

	Data(int mID, int sc, int lv, int c, int yC, int cT)
		: mapID(mID), score(sc), lives(lv), coins(c), yoshiCoins(yC), clearanceTime(cT), playerType(PlayerType::PLAYER_TYPE_SMALL) {}
    Data(int mID, int sc, int lv, int c, int yC, int cT, PlayerType pT)
        : mapID(mID), score(sc), lives(lv), coins(c), yoshiCoins(yC), clearanceTime(cT), playerType(pT) {
    }
};

struct bigData : public Data {
    std::vector<Block*> blocks;
    std::vector<Baddie*> baddies;
    std::vector<Item*> items;
    std::vector<Item*> staticItems;

    bigData(int mID, int sc, int lv, int c, int yC, PlayerType mType)
        : Data(mID, sc, lv, c, yC, mType) {} 

	bigData(int mID, int sc, int lv, int c, int yC, int cT, std::vector<Block*> blks,
			std::vector<Baddie*> bds, std::vector<Item*> its, std::vector<Item*> staticIts)
		: Data(mID, sc, lv, c, yC, cT), blocks(blks), baddies(bds), items(its), staticItems(staticIts) {}

	bigData(const Data& data, std::vector<Block*> blks, std::vector<Baddie*> bds, std::vector<Item*> its, std::vector<Item*> staticIts)
		: Data(data), blocks(blks), baddies(bds), items(its), staticItems(staticIts) {}

};

// For maps designed by users
struct UserMapData {
    std::string filename;
    std::string displayName;
    std::vector<int> entitiesID;
    Color backgroundColor;
    int backgroundID;
    
    UserMapData() : entitiesID(12000, 0), backgroundColor({255, 255, 255, 255}), backgroundID(1) {}
};