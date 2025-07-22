#pragma once

#include <iostream>
#include <vector>
#include "Block.h"
#include "Baddie.h"
#include "Item.h"
#include "raylib.h"

struct Data{
    int mapID;
    int remainingTime; // in seconds
    int score;
    int lives;
    std::vector<Block*> blocks;
    std::vector<Baddie*> baddies;
    std::vector<Item*> items;
    std::vector<Item*> staticItems;

    Data(int mID, int rTime, int sc, int lv, const std::vector<Block*>& blks = {}, 
         const std::vector<Baddie*>& bds = {}, 
         const std::vector<Item*>& its = {}, 
         const std::vector<Item*>& staticIts = {})
        : mapID(mID), remainingTime(rTime), score(sc), lives(lv), blocks(blks), 
          baddies(bds), items(its), staticItems(staticIts) {}

};