#pragma once

#include "Screen.h"
#include "Button.h"
#include "ResourceManager.h"
#include "raylib.h"
#include "json.hpp"
#include "Data.h"
#include "GameState.h"
#include "MapEditorScreen2.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

class GameWorld;

class MapEditorScreen1 : public Screen {
private:
    std::unordered_map<std::string, Button*> buttons;
    Font& font;
    float fontSize;
    bool showSavedMapDialog;
    Rectangle dialogBox;
    
    // Scroll-related variables
    std::vector<std::string> availableMaps;
    std::vector<UserMapData> userDesignedMaps; // Store user-designed map data
    Rectangle scrollViewRec;
    Rectangle contentRec;
    Vector2 scrollOffset;
    int selectedMapIndex;

    MapEditorScreen2* mapEditorScreen2;
    void setMapEditorScreen2(MapEditorScreen2* screen); 
    friend class MapEditorScreen2;
    
    // Double-click detection
    int lastClickedIndex;
    float lastClickTime;
    const float doubleClickTimeWindow = 0.5f;
    
    // Helper methods
    void loadAvailableMaps();
    void loadUserDesignedMapsFromFilesystem();
    void drawScrollableMapList();
    void loadUserDesignedMap(int mapIndex);
    void saveMapToFile(const UserMapData& mapData, const std::string& filename);
    UserMapData loadMapFromFile(const std::string& filepath);
    std::string getUserDesignedMapsDirectory() const;

public:
    MapEditorScreen1();
    ~MapEditorScreen1() override;

    MapEditorScreen1(const MapEditorScreen1&) = delete;
    MapEditorScreen1& operator=(const MapEditorScreen1&) = delete;

    void draw() override;
    void update();
    bool isDialogOpen() const;
    std::unordered_map<std::string, Button*>& getButtons();
    Button* getButton(const std::string& key) const;
    void createNewMap();

     
};
