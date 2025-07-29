#pragma once

#include <iostream>
#include <vector>
#include "Block.h"
#include "Baddie.h"
#include "Item.h"
#include "MarioType.h"
#include "raylib.h"

struct Data{
    int mapID;
    int score;
    int lives;
    int coins;
    int yoshiCoins;
    int clearanceTime;

	Data(int mID, int sc, int lv, int c, int yC, int cT)
		: mapID(mID), score(sc), lives(lv), coins(c), yoshiCoins(yC), clearanceTime(cT) {}
};

struct bigData : public Data {
    std::vector<Block*> blocks;
    std::vector<Baddie*> baddies;
    std::vector<Item*> items;
    std::vector<Item*> staticItems;

    bigData(int mID, int sc, int lv, int c, int yC, MarioType mType)
        : Data(mID, sc, lv, c, yC, mType) {}

	bigData(int mID, int sc, int lv, int c, int yC, int cT, std::vector<Block*> blks,
			std::vector<Baddie*> bds, std::vector<Item*> its, std::vector<Item*> staticIts)
		: Data(mID, sc, lv, c, yC, cT), blocks(blks), baddies(bds), items(its), staticItems(staticIts) {}

	bigData(const Data& data, std::vector<Block*> blks, std::vector<Baddie*> bds, std::vector<Item*> its, std::vector<Item*> staticIts)
		: Data(data), blocks(blks), baddies(bds), items(its), staticItems(staticIts) {}

};