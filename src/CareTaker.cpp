#include "CareTaker.h"
#include <string>
using json = nlohmann::json;

CareTaker::CareTaker(GameWorld* gw) : gw(gw) {
    fin.open("../resource/leaderboardData.json");
    if (!fin.is_open()) {
        std::cerr << "WARNING: Error opening leaderBoardData.json file." << std::endl;
        return;
    }

    try {
        json j;
        fin >> j;
        fin.close();

        if (j.contains("leaderboard")) {
            for (const auto& item : j["leaderboard"]) {
                int score = item["score"];
                int lives = item["lives"];
                int coins = item["coins"];
                int yoshiCoins = item["yoshiCoins"];
                int clearanceTime = item["clearanceTime"];
                std::string date = item["date"];
                Data data(0, score, lives, coins, yoshiCoins, clearanceTime);
                Memento* memento = new ConcreteMemento(data, date);
                leaderboardMementos.push_back(memento);
            }
            std::sort(leaderboardMementos.begin(), leaderboardMementos.end(),
                [](const Memento* a, const Memento* b) {
                    return a->getData().score > b->getData().score;
                });
            if (leaderboardMementos.size() > 5) {
                for (size_t i = 5; i < leaderboardMementos.size(); ++i) {
                    delete leaderboardMementos[i];
                }
                leaderboardMementos.resize(5);
            }
        }
    }
    catch (const std::exception& e) {
		std::cerr << "WARNING: Error parsing leaderBoardData.json: " << e.what() << std::endl;
        fin.close();
    }
    // fin.open("../resource/savedGame.json");
    // if (!fin.is_open()) {
    //     std::cerr << "WARNING: Error opening save data file." << std::endl;
    // }

    // json j;
    // fin >> j;

    // if (!j.contains("numberOfSavedGames")) {
    //     std::cerr << "WARNING: No saved games found." << std::endl;
    //     fin.close();
    //     return;
    // }
    // int numberOfSavedGames = j["numberOfSavedGames"];
    // for (int i = 0; i < numberOfSavedGames; ++i) {
    //     int mapID = j["savedGames"][i]["mapID"];
    //     int remainingTime = j["savedGames"][i]["remainingTime"];
    //     int score = j["savedGames"][i]["score"];
    //     int lives = j["savedGames"][i]["lives"];
    //     std::string date = j["savedGames"][i]["date"];
    //     Data data(mapID, remainingTime, score, lives);
    //     Memento* memento = new ConcreteMemento(data, date);
    //     mementos.push_back(memento);
    // }  
    // fin.close(); 

    leaderboardMementos.clear();
}

CareTaker::~CareTaker() {
    fout.open("../../../../resource/leaderboardData.json");
    if (!fout.is_open()) {
		std::cerr << "Error opening leaderBoardData.json for writing." << std::endl;
    }
    else {
        try {
            json j;
            j["leaderboard"] = json::array();
            for (const auto& memento : leaderboardMementos) {
                Data data = memento->getData();
                json entry;
                entry["score"] = data.score;
                entry["lives"] = data.lives;
                entry["coins"] = data.coins;
                entry["yoshiCoins"] = data.yoshiCoins;
                entry["clearanceTime"] = data.clearanceTime;
                entry["date"] = memento->getDate();
                j["leaderboard"].push_back(entry);
            }
            fout << j.dump(4);
            fout.close();
        }
        catch (const std::exception& e) {
			std::cerr << "WARNING: Error writing to leaderBoardData.json: " << e.what() << std::endl;
            fout.close();
        }
    }
    // fout.open("../savedGame.json");
    // if (!fout.is_open()) {
    //     std::cerr << "Error opening save data file for writing." << std::endl;
    // }

    // json j;
    // j["numberOfSavedGames"] = mementos.size();
    // for (int i = 0; i < mementos.size(); ++i) {
    //     Data data = mementos[i]->getData();
    //     j["savedGames"][i]["mapID"] = data.mapID;
    //     j["savedGames"][i]["remainingTime"] = data.remainingTime;
    //     j["savedGames"][i]["score"] = data.score;
    //     j["savedGames"][i]["lives"] = data.lives;
    //     j["savedGames"][i]["date"] = mementos[i]->getDate();
    
    // }
    // fout << j.dump(4);
    // fout.close();
    for (auto memento : mementos) {
        delete memento;
    }
    mementos.clear();
    for (auto memento : leaderboardMementos) {
        delete memento;
    }
    leaderboardMementos.clear();
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
    delete memento; 
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

void CareTaker::saveToCareTakerLeaderBoard() {
    Memento* memento = gw->dataFromGameWorldToLeaderboard();
    leaderboardMementos.push_back(memento);

    // sort leaderboardMementos based on score
    std::sort(leaderboardMementos.begin(), leaderboardMementos.end(), [](const Memento* a, const Memento* b) {
        return a->getData().score > b->getData().score;
    });

    // keep only top 5, memory leak prevention
    if (leaderboardMementos.size() > 5) {
        for (int i = 5; i < leaderboardMementos.size(); ++i) {
            delete leaderboardMementos[i];
        }
        leaderboardMementos.resize(5);
    }
    std::cout << "Score saved to leaderboard." << std::endl;
}

void CareTaker::releaseLeaderBoardData() const {
    if (leaderboardMementos.empty()) {
        std::cout << "No leaderboard data available." << std::endl;
        return;
    }

    std::vector<std::string> leaderboardData;
    for (const auto& memento : leaderboardMementos) {
        Data data = memento->getData();
        leaderboardData.push_back(std::to_string(data.score));
        leaderboardData.push_back(std::to_string(data.lives));
        leaderboardData.push_back(std::to_string(data.coins));
        leaderboardData.push_back(std::to_string(data.yoshiCoins));
        leaderboardData.push_back(std::to_string(data.clearanceTime));
        leaderboardData.push_back(memento->getDate());
    }  
    gw->leaderBoardScreen->setLeaderboardDataAsStrings(leaderboardData);  
}

std::vector<Memento*>& CareTaker::getLeaderBoard() {
    return leaderboardMementos;
}
