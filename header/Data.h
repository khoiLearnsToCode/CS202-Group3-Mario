#pragma once

#include <iostream>

struct Data{
    int mapID;
    int remainingTime; // in seconds
    int score;
    int lives;

    Data(int mID, int rTime, int sc, int lv)
        : mapID(mID), remainingTime(rTime), score(sc), lives(lv) {}

};