#include "LoadGame.h"
#include <string>
#include <sstream>
using json = nlohmann::json;
LoadGame::LoadGame(GameWorld* gw) :gw(gw), data(nullptr)
{
    gw->setLoadGame(this);
    fin.open("../../../../resource/savedGame.json");
    if (!fin.is_open()) {
        std::cerr << "WARNING: Error opening savedGame.json file." << std::endl;
        return;
    }

    try {
        json j;
        fin >> j;
        fin.close();

        if (j.contains("savedgame")) {
            int mapID = j["savedgame"]["mapID"];
            int score = j["savedgame"]["score"];
            int lives = j["savedgame"]["lives"];
            int coins = j["savedgame"]["coins"];
            PlayerType pt = static_cast<PlayerType>(j["savedgame"]["PlayerType"]);
            bool isLuigi = j["savedgame"]["isLuigi"];
            savedData* newData = new savedData(mapID, lives, score, coins, pt, isLuigi);
            if(!data) data = newData;
            else {
                delete data;
                std::cout << "Data already loaded, ignoring new data." << std::endl;
                data = newData;
			}
        }
    }
    catch (const std::exception& e) {
        std::cerr << "WARNING: Error parsing savedGame.json: " << e.what() << std::endl;
        fin.close();
    }
}
LoadGame::~LoadGame() {
    delete data;
}
void LoadGame::save()
{
    savedData* newdata = gw->dataFromGameWorldToLoad();
    if(!newdata) {
        std::cout << "No data to save from GameWorld." << std::endl;
        return;
	}
    if (data != nullptr) {
        delete data;
    }
    data = newdata;


    std::ofstream fout("../../../../resource/savedGame.json");
    if (!fout.is_open()) {
        std::cerr << "Error: Cannot open savedGame.json for writing." << std::endl;
        return;
    }

    try {
        json j;
        j["savedgame"] = json::object();
        json entry;
        entry["mapID"] = data->mapID;
        entry["score"] = data->score;
        entry["lives"] = data->lives;
        entry["coins"] = data->coins;
        entry["PlayerType"] = static_cast<int>(data->pT);
        entry["isLuigi"] = data->isLuigi;
        j["savedgame"] = entry;
        fout << j.dump(4);
        fout.close();
        std::cout << "Game state saved to file." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error writing to savedGame.json: " << e.what() << std::endl;
        fout.close();
    }
    std::cout << "Score saved to savedGame." << std::endl;
}
void LoadGame::load()
{
    if (data != nullptr) {
        gw->dataFromLoadToGameWorld(data);
    }
    else {
        std::cout << "No saved data to load." << std::endl;
    }
}
savedData* LoadGame::getsavedData()
{
    return data;
}
