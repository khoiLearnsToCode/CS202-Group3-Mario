#include "CareTaker.h"
using json = nlohmann::json;

CareTaker::CareTaker(GameWorld* gw) : gw(gw) {
    fin.open("../resource/savedGame.json");
    if (!fin.is_open()) {
        std::cerr << "WARNING: Error opening save data file." << std::endl;
    }

    json j;
    fin >> j;

    if (!j.contains("numberOfSavedGames")) {
        std::cerr << "WARNING: No saved games found." << std::endl;
        fin.close();
        return;
    }
    int numberOfSavedGames = j["numberOfSavedGames"];
    for (int i = 0; i < numberOfSavedGames; ++i) {
        int mapID = j["savedGames"][i]["mapID"];
        int remainingTime = j["savedGames"][i]["remainingTime"];
        int score = j["savedGames"][i]["score"];
        int lives = j["savedGames"][i]["lives"];
        std::string date = j["savedGames"][i]["date"];
        Data data(mapID, remainingTime, score, lives);
        Memento* memento = new ConcreteMemento(data, date);
        mementos.push_back(memento);
    }  
    fin.close(); 
}

CareTaker::~CareTaker() {
    fout.open("../savedGame.json");
    if (!fout.is_open()) {
        std::cerr << "Error opening save data file for writing." << std::endl;
    }

    json j;
    j["numberOfSavedGames"] = mementos.size();
    for (int i = 0; i < mementos.size(); ++i) {
        Data data = mementos[i]->getData();
        j["savedGames"][i]["mapID"] = data.mapID;
        j["savedGames"][i]["remainingTime"] = data.remainingTime;
        j["savedGames"][i]["score"] = data.score;
        j["savedGames"][i]["lives"] = data.lives;
        j["savedGames"][i]["date"] = mementos[i]->getDate();
    }
    fout << j.dump(4);
    fout.close();
    for (auto memento : mementos) {
        delete memento;
    }
    mementos.clear();
}

// Not implemented yet
void CareTaker::save() {   
    Memento* memento = gw->dataFromGameWorldToSave();
    mementos.push_back(memento);
    std::cout << "Game saved successfully." << std::endl;
}

void CareTaker::restore(int index) {
    if (index < 0 || index >= mementos.size()) {
        std::cerr << "Invalid index for restore." << std::endl;
        return;
    }
    Memento* memento = mementos[index];
    gw->restoreDataFromMemento(memento);
    mementos.erase(mementos.begin() + index);
    std::cout << "Game restored successfully from index " << index << "." << std::endl;
}

void CareTaker::showSavedData() const {
    if (mementos.empty()) {
        std::cout << "No saved games available." << std::endl;
        return;
    }
    for (int i = 0; i < mementos.size(); ++i) {
        std::cout << "Saved Game " << i + 1 << ": " << mementos[i]->display() << std::endl;
    }
}

std::vector<Memento*> CareTaker::getSavedData() const {
    // copy the mementos to a new vector to avoid exposing internal state
    std::vector<Memento*> savedData;
    for (const auto& memento : mementos) {
        savedData.push_back(new ConcreteMemento(memento->getData(), memento->getDate()));
    }
    return savedData;
}