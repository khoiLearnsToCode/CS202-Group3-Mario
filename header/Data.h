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
	Data() : mapID(0), score(0), lives(0), coins(0), yoshiCoins(0), clearanceTime(0) {}
	Data(int mID, int sc, int lv, int c, int yC, int cT)
		: mapID(mID), score(sc), lives(lv), coins(c), yoshiCoins(yC), clearanceTime(cT) {}
};

struct savedData
{
    int mapID;
    int lives;
    int score;
    int coins;
    PlayerType pT;
    bool isLuigi;
    savedData() :mapID(1), lives(5), score(0), coins(0), pT(PlayerType::PLAYER_TYPE_SMALL), isLuigi(false) {}
    savedData(int mID, int l, int sc, int c, PlayerType pt, bool Luigi) :mapID(mID), lives(l), score(sc), coins(c), pT(pt), isLuigi(Luigi) {}
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