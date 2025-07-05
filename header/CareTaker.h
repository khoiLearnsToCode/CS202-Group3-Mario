#pragma once

#include "GameWorld.h"
#include "Memento.h"
#include "json.hpp"

#include <iostream>
#include <fstream>
#include <vector>

class CareTaker {
    std::vector<Memento*> mementos;
    GameWorld* gw;
    std::ifstream fin;
    std::ofstream fout;

    public:
    CareTaker(GameWorld* gw);
    CareTaker(const CareTaker&) = delete;
    CareTaker& operator=(const CareTaker&) = delete;
    ~CareTaker();

    void save();

    void restore(int index);
    void showSavedData() const;
    std::vector<Memento*> getSavedData() const;
};