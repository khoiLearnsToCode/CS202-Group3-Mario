#pragma once

#include "GameWorld.h"
#include "Memento.h"
#include "json.hpp"

#include <iostream>
#include <fstream>

class CareTaker {
    Memento* savedMemento;
    std::vector<Memento*> mementos;
    std::vector<Memento*> leaderboardMementos;
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
    std:: vector<Memento*>getSavedData() const;  
    void saveToCareTakerLeaderBoard();
    void releaseLeaderBoardData() const;
    std::vector<Memento*>& getLeaderBoard();
};

