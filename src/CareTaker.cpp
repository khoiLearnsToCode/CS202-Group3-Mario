#include "CareTaker.h"
#include <string>
#include <sstream>
using json = nlohmann::json;

CareTaker::CareTaker(GameWorld* gw) : gw(gw) {
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
                }

                Data data(0, score, lives, coins, yoshiCoins, clearanceTime);
                Memento* memento = new ConcreteMemento(data, formattedDate , time);
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

    fin.open("../resource/savedGame.json");
    if (fin.is_open()) {
        json j; fin >> j; fin.close();
        int n = j.value("numberOfSavedGames", 0);
        for (int i = 0; i < n; ++i) {
            int mapID = j["savedGames"][i].value("mapID", 1);
            int score = j["savedGames"][i].value("score", 0);
            int lives = j["savedGames"][i].value("lives", 3);
            int coins = j["savedGames"][i].value("coins", 0);
            int yoshiCoins = j["savedGames"][i].value("yoshiCoins", 0);
            int timeRemain = j["savedGames"][i].value("timeRemaining", 400);
            int typeInt = j["savedGames"][i].value("type", 0);
            std::string date = j["savedGames"][i].value("date", "");

            Data d(mapID, score, lives, coins, yoshiCoins, timeRemain, (PlayerType)typeInt);
            Memento* m = new ConcreteMemento(d, date);
            mementos.push_back(m);
        }
    }
    //leaderboardMementos.clear();
}

CareTaker::~CareTaker() {
    //out.open("../../../../resource/leaderboardData.json");
    //f (!fout.is_open()) {
	//	std::cerr << "Error opening leaderBoardData.json for writing." << std::endl;
    //
    //lse {
    //   try {
    //       json j;
    //       j["leaderboard"] = json::array();
    //       if (!leaderboardMementos.empty()){
    //           for (const auto& memento : leaderboardMementos) {
    //               Data data = memento->getData();
    //               json entry;
    //               entry["score"] = data.score;
    //               entry["lives"] = data.lives;
    //               entry["coins"] = data.coins;
    //               entry["yoshiCoins"] = data.yoshiCoins;
    //               entry["clearanceTime"] = data.clearanceTime;
    //               entry["date"] = memento->getDate();
    //               entry["time"] = memento->getTime();
    //               j["leaderboard"].push_back(entry);
    //           }
    //           fout << j.dump(4);
    //       }
    //       fout.close();
    //   }
    //   catch (const std::exception& e) {
	//		std::cerr << "WARNING: Error writing to leaderBoardData.json: " << e.what() << std::endl;
    //       fout.close();
    //   }
    //
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
  
    ////////////////////////////////////////////////////////////////////////////////
    fout.open("../resource/savedGame.json");
    if (!fout.is_open()) {
        std::cerr << "Error opening save data file for writing." << std::endl;
    }

    json j;
    j["numberOfSavedGames"] = mementos.size();
    for (int i = 0; i < mementos.size(); ++i) {
        Data data = mementos[i]->getData();
        j["savedGames"][i]["mapID"] = data.mapID;
        j["savedGames"][i]["remainingTime"] = data.clearanceTime;
        j["savedGames"][i]["score"] = data.score;
        j["savedGames"][i]["lives"] = data.lives;
        j["savedGames"][i]["coins"] = data.coins;  
        j["savedGames"][i]["yoshiCoins"] = data.yoshiCoins;  
        j["savedGames"][i]["date"] = mementos[i]->getDate();
		j["savedGames"][i]["type"] = (int)data.playerType;
    
    }
    fout << j.dump(4);
    fout.close();
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
    std::ofstream fout("../../../../resource/leaderboardData.json");
    if (!fout.is_open()) {
        std::cerr << "Error: Cannot open leaderboardData.json for writing." << std::endl;
        return;
    }
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
        std::cout << "Leaderboard saved to file with " << leaderboardMementos.size() << " entries." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error writing to leaderboardData.json: " << e.what() << std::endl;
        fout.close();
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
