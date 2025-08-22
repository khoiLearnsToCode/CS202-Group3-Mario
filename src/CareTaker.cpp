#include "CareTaker.h"
#include <string>
#include <sstream>
using json = nlohmann::json;

CareTaker::CareTaker(GameWorld* gw) : gw(gw), savedMemento(nullptr) {
    gw->setCaretaker(this);
    fin.open("../../../../resource/leaderboardData.json");
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
                std::string rawDate = item["date"];
                std::string formattedDate = rawDate;
                std::istringstream iss(rawDate);
                std::string weekday, month, day, time, year;
                std::string formattedTime = time;
                if (iss >> weekday >> month >> day >> time >> year) {
                    std::string monthNum = "01";
                    if (month == "Jan") monthNum = "01";
                    else if (month == "Feb") monthNum = "02";
                    else if (month == "Mar") monthNum = "03";
                    else if (month == "Apr") monthNum = "04";
                    else if (month == "May") monthNum = "05";
                    else if (month == "Jun") monthNum = "06";
                    else if (month == "Jul") monthNum = "07";
                    else if (month == "Aug") monthNum = "08";
                    else if (month == "Sep") monthNum = "09";
                    else if (month == "Oct") monthNum = "10";
                    else if (month == "Nov") monthNum = "11";
                    else if (month == "Dec") monthNum = "12";
                    formattedDate = year + "-" + monthNum + "-" + day;
                }
                else {
                    formattedDate = rawDate;
                    formattedTime = "";
                }

                Data data(0, score, lives, coins, yoshiCoins, clearanceTime);
                Memento* memento = new ConcreteMemento(data, formattedDate, formattedTime);
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

    fin.open("../../../../resource/savedGame.json");
    if (fin.is_open()) {
        try {
            json j;
            fin >> j;
            fin.close();
            if (j.contains("savedGame")) {
                int mapID = j["savedGame"].value("mapID", 1);
                int score = j["savedGame"].value("score", 0);
                int lives = j["savedGame"].value("lives", 3);
                int coins = j["savedGame"].value("coins", 0);
                int yoshiCoins = j["savedGame"].value("yoshiCoins", 0);
                int timeRemain = j["savedGame"].value("timeRemaining", 400);
                int typeInt = j["savedGame"].value("type", 0);
                std::string date = j["savedGame"].value("date", "");

                Data d(mapID, score, lives, coins, yoshiCoins, timeRemain, (PlayerType)typeInt);
                savedMemento = new ConcreteMemento(d, date);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "WARNING: Error parsing savedGame.json: " << e.what() << std::endl;
            fin.close();
        }
    }
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
            if (!leaderboardMementos.empty()) {
                for (const auto& memento : leaderboardMementos) {
                    Data data = memento->getData();
                    json entry;
                    entry["score"] = data.score;
                    entry["lives"] = data.lives;
                    entry["coins"] = data.coins;
                    entry["yoshiCoins"] = data.yoshiCoins;
                    entry["clearanceTime"] = data.clearanceTime;
                    entry["date"] = memento->getDate();
                    entry["time"] = memento->getTime();
                    j["leaderboard"].push_back(entry);
                }
                fout << j.dump(4);
            }
            fout.close();
        }
        catch (const std::exception& e) {
            std::cerr << "WARNING: Error writing to leaderBoardData.json: " << e.what() << std::endl;
            fout.close();
        }
    }

    fout.open("../../../../resource/savedGame.json");
    if (!fout.is_open()) {
        std::cerr << "Error opening savedGame.json for writing." << std::endl;
    }
    else {
        try {
            json j;
            j["savedGame"] = json::object();  // Single object, not array
            if (savedMemento != nullptr) {
                Data data = savedMemento->getData();
                j["savedGame"]["mapID"] = data.mapID;
                j["savedGame"]["score"] = data.score;
                j["savedGame"]["lives"] = data.lives;
                j["savedGame"]["coins"] = data.coins;
                j["savedGame"]["yoshiCoins"] = data.yoshiCoins;
                j["savedGame"]["timeRemaining"] = data.clearanceTime;
                j["savedGame"]["type"] = (int)data.playerType;
                j["savedGame"]["date"] = savedMemento->getDate();
            }
            fout << j.dump(4);
            fout.close();
        }
        catch (const std::exception& e) {
            std::cerr << "WARNING: Error writing to savedGame.json: " << e.what() << std::endl;
            fout.close();
        }
    }

    if (savedMemento != nullptr) {
        delete savedMemento;
        savedMemento = nullptr;
    }
    for (auto memento : leaderboardMementos) {
        delete memento;
    }
    leaderboardMementos.clear();
}

void CareTaker::save() {
    Memento* newMemento = gw->dataFromGameWorldToSave();
    if (savedMemento != nullptr) {
        delete savedMemento;  // Overwrite previous save
    }
    savedMemento = newMemento;
    std::cout << "Game saved successfully (overwriting previous save)." << std::endl;
}

void CareTaker::restore() {
    if (savedMemento != nullptr) {
        gw->restoreDataFromMemento(savedMemento);
        std::cout << "Game restored successfully." << std::endl;
    }
    else {
        std::cout << "No saved game available." << std::endl;
    }
}

void CareTaker::showSavedData() const {
    if (savedMemento == nullptr) {
        std::cout << "No saved game available." << std::endl;
        return;
    }
    std::cout << "Saved Game: " << savedMemento->display() << std::endl;
}

Memento* CareTaker::getSavedData() const {
    if (savedMemento == nullptr) {
        return nullptr;
    }
    return new ConcreteMemento(savedMemento->getData(), savedMemento->getDate());
}

void CareTaker::saveToCareTakerLeaderBoard() {
    Memento* memento = gw->dataFromGameWorldToLeaderboard();
    leaderboardMementos.push_back(memento);
    std::sort(leaderboardMementos.begin(), leaderboardMementos.end(), [](const Memento* a, const Memento* b) {
        return a->getData().score > b->getData().score;
        });
    if (leaderboardMementos.size() > 5) {
        for (size_t i = 5; i < leaderboardMementos.size(); ++i) {
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
        std::string rawDate = memento->getDate();
        std::string shortDate = rawDate;
        size_t pos = rawDate.find(' ');
        if (pos != std::string::npos && rawDate.length() >= pos + 10) {
            shortDate = rawDate.substr(pos + 1, 10);
        }
        leaderboardData.push_back(shortDate);
    }
    gw->leaderBoardScreen->setLeaderboardDataAsStrings(leaderboardData);
}

std::vector<Memento*>& CareTaker::getLeaderBoard() {
    return leaderboardMementos;
}