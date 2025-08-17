#include "MapEditorScreen1.h"
#include "GameWorld.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <filesystem>
#include <ctime>
#include <sstream>
#include "raygui.h"

using json = nlohmann::json;

MapEditorScreen1::MapEditorScreen1() : Screen(), 
                                       font1(ResourceManager::getInstance().getFont("fixedsys")),
                                       font2(ResourceManager::getInstance().getFont("SuperMario256")),
                                       fontSize(40.0f),
                                       showSavedMapDialog(false),
                                       scrollOffset({0, 0}),
                                       selectedMapIndex(-1),
                                       lastClickedIndex(-1),
                                       lastClickTime(0.0f),
                                       frame(0), maxFrame(4),
                                       frameTimeAccum(0.0f),
                                       frameTime(1.0f) {
    
    
        // Create buttons for map editor options
    buttons.emplace("NEW MAP", new ButtonTextTexture("NEW MAP", "longButton", 
                    {GetScreenWidth() / 2.0f - 128.0f, GetScreenHeight() / 2.0f - 50.0f}, 
                    2.0f, WHITE, font1, fontSize));
        
    buttons.emplace("SAVED MAP", new ButtonTextTexture("SAVED MAP", "longButton", 
                    {GetScreenWidth() / 2.0f - 128.0f, GetScreenHeight() / 2.0f + 25.0f}, 
                    2.0f, WHITE, font1, fontSize));
    
    buttons.emplace("BACK TO MENU", new ButtonTextTexture("returnButton", 
                    { 100.0f, GetScreenHeight() - 100.0f }, 2.0f));
        
    // Initialize dialog box (centered on screen)
    dialogBox = {
        GetScreenWidth() / 2.0f - 400.0f,
        GetScreenHeight() / 2.0f - 250.0f,
        800.0f,
        500.0f
    };
        
        // Initialize scroll view rectangle (inside the dialog box)
        scrollViewRec = {
        dialogBox.x + 20.0f,
        dialogBox.y + 80.0f,
        dialogBox.width - 40.0f,
        dialogBox.height - 140.0f
    };

    try {    
        // Load available maps
        loadAvailableMaps();
        
    } catch (const std::exception& e) {
        // If there's an error during construction, ensure we have a basic setup
        // This prevents crashes but might result in missing functionality
        std::cerr << "Error in MapEditorScreen1 constructor: " << e.what() << std::endl;
    }
}

MapEditorScreen1::~MapEditorScreen1() {
    // Clean up buttons
    for (auto& [key, button] : buttons) {
        delete button;
        button = nullptr;
    }
    buttons.clear();
    
    // Delete all existing JSON files and rewrite from current userDesignedMaps
    std::string mapsDirectory = "../../../../resource/userDesignedMaps";
    
    try {
        // Delete all existing JSON files in the directory
        if (std::filesystem::exists(mapsDirectory) && std::filesystem::is_directory(mapsDirectory)) {
            for (const auto& entry : std::filesystem::directory_iterator(mapsDirectory)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    try {
                        std::filesystem::remove(entry.path());
                        std::cout << "Deleted existing map file: " << entry.path().filename() << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Error deleting file " << entry.path() << ": " << e.what() << std::endl;
                    }
                }
            }
        }
        
        // Rewrite all maps from current userDesignedMaps vector
        for (const auto& mapData : userDesignedMaps) {
            if (!mapData.filename.empty()) {
                saveMapToFile(mapData, mapData.filename);
                std::cout << "Rewritten map file: " << mapData.filename << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error during destructor cleanup: " << e.what() << std::endl;
    }
}

void MapEditorScreen1::update() {

    updateFrameAnimation();

    // If dialog is open, handle dialog-specific input and prevent other button interactions
    if (showSavedMapDialog) {
        // Check if user clicked outside the dialog box to dismiss it
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            if (!CheckCollisionPointRec(mousePos, dialogBox)) {
                showSavedMapDialog = false;
                selectedMapIndex = -1;
            }
        }
        
        // Handle map selection and double-click detection for raygui scroll panel
        // Note: The actual click detection is now handled in the drawScrollableMapList function
        // where we can properly calculate the clicked item based on the raygui scroll state
        
        // Reset double-click tracking if clicking outside the scroll view
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(mousePos, scrollViewRec)) {
            lastClickedIndex = -1;
            lastClickTime = 0.0f;
        }
        
        // Return early to prevent other button interactions while dialog is open
        return;
    }
    
    // Only check for saved map button when dialog is not open
    Button* selectMapButton = getButton("SAVED MAP");
    if (selectMapButton && selectMapButton->isReleased()) {
        showSavedMapDialog = true;
        // Only refresh if needed (e.g., after saving a new map)
        // loadAvailableMaps(); // Uncomment this line only if you need to refresh from disk
    }
}

void MapEditorScreen1::draw() {

    // Draw background
    std::string key = TextFormat("mapEditorScreen%d", frame);
    DrawTexture(textures[key], 0, 0, WHITE);

    // Draw title
    Vector2 titlePos = {(GetScreenWidth() - textures["mapEditor"].width) / 2.0f, 0};
    DrawTexture(textures["mapEditor"], titlePos.x, titlePos.y, WHITE);

    // Draw buttons
    for (auto& [key, button] : buttons) {
        button->draw();
    }
    
    // Draw saved map dialog box if visible
    if (showSavedMapDialog) {
        // Draw semi-transparent overlay
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        
        // Draw dialog box
        DrawRectangleRec(dialogBox, WHITE);
        DrawRectangleLinesEx(dialogBox, 3.0f, BLACK);
        
        // Draw dialog title
        const char* dialogTitle = "SAVED MAPS";
        Vector2 titleSize = MeasureTextEx(font2, dialogTitle, fontSize, 0.0f);
        DrawTextEx(font2, dialogTitle, 
                   {dialogBox.x + (dialogBox.width - titleSize.x) / 2.0f, dialogBox.y + 20.0f}, 
                   fontSize, 0.0f, BLACK);
        
        // Draw scrollable map list
        drawScrollableMapList();
        
        // Draw close instruction
        const char* closeText = "Click outside to close | Double-click to select";
        Vector2 closeSize = MeasureTextEx(font1, closeText, fontSize - 20.0f, 0.0f);
        DrawTextEx(font1, closeText, 
                   {dialogBox.x + (dialogBox.width - closeSize.x) / 2.0f, 
                    dialogBox.y + dialogBox.height - 40.0f}, 
                   fontSize - 20.0f, 0.0f, DARKGRAY);
    }
}

std::unordered_map<std::string, Button*>& MapEditorScreen1::getButtons() {
    return buttons;
}

bool MapEditorScreen1::isDialogOpen() const {
    return showSavedMapDialog;
}

Button* MapEditorScreen1::getButton(const std::string& key) const {
    auto it = buttons.find(key);
    if (it != buttons.end()) {
        return it->second;
    }
    return nullptr;
}

void MapEditorScreen1::loadAvailableMaps() {
    availableMaps.clear();
    
    // Load user-designed maps from filesystem
    loadUserDesignedMapsFromFilesystem();
    
    for (const auto& mapData : userDesignedMaps) {
        std::string mapDisplay = mapData.displayName;
        availableMaps.push_back(mapDisplay);
    }
    
    // Reset scroll and selection
    scrollOffset = {0, 0};
    selectedMapIndex = -1;
    lastClickedIndex = -1;
    lastClickTime = 0.0f;
}

void MapEditorScreen1::drawScrollableMapList() {
    if (availableMaps.empty()) {
        // Draw a simple message if no maps are available using raygui
        const char* noMapsText = "No saved maps found";
        
        // Draw background panel
        GuiPanel(scrollViewRec, NULL);
        
        // Draw centered text
        Vector2 textSize = MeasureTextEx(font2, noMapsText, fontSize - 10.0f, 0.0f);
        Vector2 textPos = {
            scrollViewRec.x + (scrollViewRec.width - textSize.x) / 2.0f,
            scrollViewRec.y + (scrollViewRec.height - textSize.y) / 2.0f
        };
        DrawTextEx(font2, noMapsText, textPos, fontSize - 10.0f, 0.0f, DARKGRAY);
        return;
    }
    
    // Validate scroll view rectangle
    if (scrollViewRec.width <= 0 || scrollViewRec.height <= 0) {
        return;
    }
    
    // Calculate content dimensions
    float itemHeight = 50.0f;
    float totalContentHeight = availableMaps.size() * itemHeight;
    Rectangle content = {0, 0, scrollViewRec.width - 14, totalContentHeight}; // Leave space for scrollbar
    
    // Use raygui scroll panel
    Vector2 scroll = {-scrollOffset.x, -scrollOffset.y};
    Rectangle view = scrollViewRec;
    
    // Begin scroll panel - GuiScrollPanel handles the scroll automatically
    GuiScrollPanel(scrollViewRec, NULL, content, &scroll, &view);
    scrollOffset.x = -scroll.x;
    scrollOffset.y = -scroll.y;
    
    // Begin scissor mode for clipping
    BeginScissorMode((int)view.x, (int)view.y, (int)view.width, (int)view.height);
    
    // Handle click detection within the scroll panel
    Vector2 mousePos = GetMousePosition();
    bool clickedInsideView = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, view);
    
    // Draw map items
    for (size_t i = 0; i < availableMaps.size(); i++) {
        float itemY = view.y + (i * itemHeight) + scroll.y;
        
        // Only draw items that are visible
        if (itemY + itemHeight >= view.y && itemY <= view.y + view.height) {
            Rectangle itemRect = {view.x, itemY, view.width, itemHeight};
            
            // Validate item rectangle
            if (itemRect.width <= 0 || itemRect.height <= 0) {
                continue;
            }
            
            // Check if this item was clicked
            bool itemClicked = clickedInsideView && CheckCollisionPointRec(mousePos, itemRect);
            
            if (itemClicked) {
                float currentTime = GetTime();
                int clickedIndex = (int)i;
                
                // Check for double-click: same item clicked within time window
                if (clickedIndex == lastClickedIndex && 
                    currentTime - lastClickTime < doubleClickTimeWindow) {
                    // Double-click detected - load user-designed map
                    if (clickedIndex < userDesignedMaps.size()) {
                        loadUserDesignedMap(clickedIndex);
                    }
                    showSavedMapDialog = false;
                    selectedMapIndex = -1;
                    lastClickedIndex = -1;
                    lastClickTime = 0.0f;
                    EndScissorMode();
                    return; // Exit early to prevent further processing
                } else {
                    // Single click - just select the item
                    selectedMapIndex = clickedIndex;
                    lastClickedIndex = clickedIndex;
                    lastClickTime = currentTime;
                }
            }
            
            // Draw selection highlight
            if ((int)i == selectedMapIndex) {
                DrawRectangleRec(itemRect, Fade(BLUE, 0.3f));
                DrawRectangleLinesEx(itemRect, 2.0f, BLUE);
            } else {
                // Draw hover effect
                if (CheckCollisionPointRec(mousePos, itemRect)) {
                    DrawRectangleRec(itemRect, Fade(GRAY, 0.2f));
                }
            }
            
            // Draw item background
            DrawRectangleRec(itemRect, Fade(WHITE, 0.8f));
            DrawRectangleLinesEx(itemRect, 1.0f, LIGHTGRAY);
            
            // Draw map name - ensure we don't exceed string bounds
            if (!availableMaps[i].empty()) {
                float textSize = fontSize - 15.0f;
                Vector2 textPos = {itemRect.x + 10.0f, itemRect.y + (itemRect.height - textSize) / 2.0f};
                DrawTextEx(font2, availableMaps[i].c_str(), textPos, textSize, 0.0f, BLACK);
            }
        }
    }
    
    EndScissorMode();
}

void MapEditorScreen1::loadUserDesignedMap(int mapIndex) {
    if (mapIndex >= 0 && mapIndex < userDesignedMaps.size()) {
        UserMapData& mapData = userDesignedMaps[mapIndex];
        std::cout << "Loading user-designed map: " << mapData.displayName << std::endl;
        std::cout << "Entities count: " << mapData.entitiesID.size() << std::endl;
        std::cout << "Background color: [" << (int)mapData.backgroundColor.r << ", " 
                  << (int)mapData.backgroundColor.g << ", " << (int)mapData.backgroundColor.b 
                  << ", " << (int)mapData.backgroundColor.a << "]" << std::endl;
        std::cout << "Background ID: " << mapData.backgroundID << std::endl;
        
        // Pass the data to MapEditorScreen2 and transition to it
        if (mapEditorScreen2) {
            mapEditorScreen2->setCurrentMapData(&mapData);
            GameWorld::state = GAME_STATE_MAP_EDITOR_SCREEN2;
        }
        
        std::cout << "Map loaded from: " << mapData.filename << std::endl;
    } else {
        std::cerr << "Invalid map index: " << mapIndex << std::endl;
    }
}

void MapEditorScreen1::createNewMap() {
    std::cout << "Creating new map..." << std::endl;
    int mapIndex = userDesignedMaps.size() + 1; // New map will be added at the end
    
    // Create a new default UserMapData
    UserMapData newMapData;
    newMapData.displayName = "New Map";
    newMapData.filename = "des" + std::to_string(mapIndex) + ".json";
    userDesignedMaps.push_back(newMapData);

    // Pass the new map data to MapEditorScreen2
    if (mapEditorScreen2) {
        mapEditorScreen2->setCurrentMapData(&userDesignedMaps.back());
    }
    
    // Switch to MapEditorScreen2
    GameWorld::state = GAME_STATE_MAP_EDITOR_SCREEN2;
}

void MapEditorScreen1::loadUserDesignedMapsFromFilesystem() {
    userDesignedMaps.clear();
    
    std::string mapsDirectory = getUserDesignedMapsDirectory();
    
    try {
        if (std::filesystem::exists(mapsDirectory) && std::filesystem::is_directory(mapsDirectory)) {
            for (const auto& entry : std::filesystem::directory_iterator(mapsDirectory)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    try {
                        UserMapData mapData = loadMapFromFile(entry.path().string());
                        userDesignedMaps.push_back(mapData);
                        std::cout << "Loaded map: " << mapData.displayName << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Error loading map file " << entry.path() << ": " << e.what() << std::endl;
                    }
                }
            }
        } else {
            std::cout << "User designed maps directory does not exist: " << mapsDirectory << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading user designed maps directory: " << e.what() << std::endl;
    }
}

void MapEditorScreen1::saveMapToFile(const UserMapData& mapData, const std::string& filename) {
    std::string mapsDirectory = "../../../../resource/userDesignedMaps";
    std::string fullPath = mapsDirectory + "/" + filename;
    
    try {
        // Create directory if it doesn't exist
        std::filesystem::create_directories(mapsDirectory);
        
        // Create compact JSON structure manually
        std::stringstream jsonStream;
        jsonStream << "{\n";
        
        // Serialize EntitiesID with chunks of 200 elements per line for compactness
        jsonStream << "  \"EntitiesID\": ";
        jsonStream << serialize_vector_with_chunks(mapData.entitiesID, 200);
        jsonStream << ",\n";
        
        // Serialize backgroundColor
        jsonStream << "  \"backgroundColor\": [" 
                   << (int)mapData.backgroundColor.r << ", " 
                   << (int)mapData.backgroundColor.g << ", " 
                   << (int)mapData.backgroundColor.b << ", " 
                   << (int)mapData.backgroundColor.a << "],\n";
        
        // Serialize backgroundID
        jsonStream << "  \"backgroundID\": " << mapData.backgroundID << "\n";
        
        jsonStream << "}";
        
        // Write to file
        std::ofstream file(fullPath);
        if (file.is_open()) {
            file << jsonStream.str();
            file.close();
            std::cout << "Map saved to: " << fullPath << std::endl;
        } else {
            std::cerr << "Failed to open file for writing: " << fullPath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error saving map to file: " << e.what() << std::endl;
    }
}

UserMapData MapEditorScreen1::loadMapFromFile(const std::string& filepath) {
    UserMapData mapData;
    
    try {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filepath);
        }
        
        json mapJson;
        file >> mapJson;
        file.close();
        
        // Extract filename without path and extension for display name
        std::filesystem::path path(filepath);
        mapData.filename = path.filename().string();
        mapData.displayName = path.stem().string(); // filename without extension
        
        // Load entities array (should be 12000 elements)
        if (mapJson.contains("EntitiesID") && mapJson["EntitiesID"].is_array()) {
            mapData.entitiesID = mapJson["EntitiesID"].get<std::vector<int>>();
            // Ensure it's exactly 12000 elements
            if (mapData.entitiesID.size() != 12000) {
                mapData.entitiesID.resize(12000, 0);
            }
        } else {
            std::cerr << "EntitiesID not found or invalid in map file: " << filepath << std::endl;
            mapData.entitiesID = std::vector<int>(12000, 0);
        }
        
        // Load background color (should be 4 RGBA values)
        if (mapJson.contains("backgroundColor") && mapJson["backgroundColor"].is_array()) {
            auto bgColor = mapJson["backgroundColor"].get<std::vector<int>>();
            if (bgColor.size() >= 4) {
                mapData.backgroundColor.r = bgColor[0];
                mapData.backgroundColor.g = bgColor[1];
                mapData.backgroundColor.b = bgColor[2];
                mapData.backgroundColor.a = bgColor[3];
            }
        }
        
        // Load background ID
        if (mapJson.contains("backgroundID") && mapJson["backgroundID"].is_number()) {
            mapData.backgroundID = mapJson["backgroundID"].get<int>();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading map from file " << filepath << ": " << e.what() << std::endl;
        // Return a default map if loading fails
        mapData.filename = std::filesystem::path(filepath).filename().string();
        mapData.displayName = "Error_Loading_" + std::filesystem::path(filepath).stem().string();
        mapData.entitiesID = std::vector<int>(12000, 0);
        mapData.backgroundColor = {255, 255, 255, 255};
        mapData.backgroundID = 1;
    }
    
    return mapData;
}

std::string MapEditorScreen1::getUserDesignedMapsDirectory() const {
    // Relative path to the user designed maps directory
    return "../resource/userDesignedMaps";
}

void MapEditorScreen1::setMapEditorScreen2(MapEditorScreen2* screen) {
    mapEditorScreen2 = screen;
}

void MapEditorScreen1::updateFrameAnimation() {
    frameTimeAccum += GetFrameTime();
    if (frameTimeAccum >= frameTime) {
        frameTimeAccum = 0.0f;
        frame = (frame + 1) % maxFrame; // Loop through frames
    }
}

