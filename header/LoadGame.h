#pragma once
#include "GameWorld.h"
#include"Data.h"
#include "json.hpp"

#include <iostream>
#include <fstream>

class LoadGame
{
private:
    savedData* data;
    GameWorld* gw;
    std::ifstream fin;
    std::ofstream fout;
public:
    LoadGame(GameWorld* gw);
    LoadGame(const LoadGame&) = delete;
    LoadGame& operator=(const LoadGame&) = delete;
    ~LoadGame();

    void save();
    void load();
    savedData* getsavedData();
};