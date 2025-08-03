#include "MapEditorScreen1.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <filesystem>
#include <ctime>

using json = nlohmann::json;

MapEditorScreen1::MapEditorScreen1() : Screen(), 
                                       font(ResourceManager::getInstance().getFont("SuperMario256")),
                                       fontSize(40.0f),
                                       showSavedMapDialog(false),
                                       scrollOffset({0, 0}),
                                       selectedMapIndex(-1),
                                       lastClickedIndex(-1),
                                       lastClickTime(0.0f) {
    
    
        // Create buttons for map editor options
    buttons.emplace("NEW MAP", new ButtonTextTexture("NEW MAP", "longButton", 
                    {GetScreenWidth() / 2.0f - 128.0f, GetScreenHeight() / 2.0f - 50.0f}, 
                    2.0f, WHITE, font, fontSize));
        
    buttons.emplace("SAVED MAP", new ButtonTextTexture("SAVED MAP", "longButton", 
                    {GetScreenWidth() / 2.0f - 128.0f, GetScreenHeight() / 2.0f + 25.0f}, 
                    2.0f, WHITE, font, fontSize));
    
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
    for (auto& [key, button] : buttons) {
        delete button;
        button = nullptr;
    }
    buttons.clear();
}

void MapEditorScreen1::update() {
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
        
        // Handle scroll wheel input when mouse is over the scroll view
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, scrollViewRec)) {
            float wheel = GetMouseWheelMove();
            scrollOffset.y += wheel * 30.0f; // Scroll speed
            
            // Clamp scroll offset
            float maxScroll = 0.0f;
            float contentHeight = availableMaps.size() * 50.0f; // 50px per item
            if (contentHeight > scrollViewRec.height) {
                maxScroll = contentHeight - scrollViewRec.height;
                scrollOffset.y = std::clamp(scrollOffset.y, -maxScroll, 0.0f);
            } else {
                scrollOffset.y = 0.0f;
            }
        }
        
        // Handle map selection and double-click detection
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, scrollViewRec)) {
            float relativeY = mousePos.y - scrollViewRec.y + (-scrollOffset.y);
            int clickedIndex = (int)(relativeY / 50.0f);
            
            if (clickedIndex >= 0 && clickedIndex < availableMaps.size()) {
                float currentTime = GetTime();
                
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
                } else {
                    // Single click - just select the item
                    selectedMapIndex = clickedIndex;
                    lastClickedIndex = clickedIndex;
                    lastClickTime = currentTime;
                }
            }
        }
        
        // Reset double-click tracking if clicking outside the scroll view
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(mousePos, scrollViewRec)) {
            lastClickedIndex = -1;
            lastClickTime = 0.0f;
        }
        
        // Return early to prevent other button interactions while dialog is open
        return;
    }
    
    // Only check for button interactions when dialog is not open
    Button* selectMapButton = getButton("SAVED MAP");
    if (selectMapButton && selectMapButton->isReleased()) {
        showSavedMapDialog = true;
        loadAvailableMaps(); // Refresh the list
    }
}

void MapEditorScreen1::draw() {
    // Draw yellow background
    ClearBackground(YELLOW);
    
    // Draw title
    const char* title = "MAP EDITOR";
    Vector2 titleSize = MeasureTextEx(font, title, fontSize + 20.0f, 0.0f);
    DrawTextEx(font, title, 
               {(GetScreenWidth() - titleSize.x) / 2.0f, GetScreenHeight() / 2.0f - 150.0f}, 
               fontSize + 20.0f, 0.0f, BLACK);
    
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
        Vector2 titleSize = MeasureTextEx(font, dialogTitle, fontSize, 0.0f);
        DrawTextEx(font, dialogTitle, 
                   {dialogBox.x + (dialogBox.width - titleSize.x) / 2.0f, dialogBox.y + 20.0f}, 
                   fontSize, 0.0f, BLACK);
        
        // Draw scrollable map list
        drawScrollableMapList();
        
        // Draw close instruction
        const char* closeText = "Click outside to close | Double-click to select";
        Vector2 closeSize = MeasureTextEx(font, closeText, fontSize - 20.0f, 0.0f);
        DrawTextEx(font, closeText, 
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
        // Draw a simple message if no maps are available
        const char* noMapsText = "No saved maps found";
        Vector2 textSize = MeasureTextEx(font, noMapsText, fontSize - 10.0f, 0.0f);
        Vector2 textPos = {
            scrollViewRec.x + (scrollViewRec.width - textSize.x) / 2.0f,
            scrollViewRec.y + (scrollViewRec.height - textSize.y) / 2.0f
        };
        
        // Draw background
        DrawRectangleRec(scrollViewRec, Fade(LIGHTGRAY, 0.3f));
        DrawRectangleLinesEx(scrollViewRec, 2.0f, GRAY);
        
        // Draw text
        DrawTextEx(font, noMapsText, textPos, fontSize - 10.0f, 0.0f, DARKGRAY);
        return;
    }
    
    // Validate scroll view rectangle
    if (scrollViewRec.width <= 0 || scrollViewRec.height <= 0) {
        return;
    }
    
    // Begin scissor mode to clip content to scroll view
    BeginScissorMode((int)scrollViewRec.x, (int)scrollViewRec.y, 
                     (int)scrollViewRec.width, (int)scrollViewRec.height);
    
    // Draw scroll view background
    DrawRectangleRec(scrollViewRec, Fade(LIGHTGRAY, 0.3f));
    DrawRectangleLinesEx(scrollViewRec, 2.0f, GRAY);
    
    // Calculate content dimensions
    float itemHeight = 50.0f;
    float totalContentHeight = availableMaps.size() * itemHeight;
    contentRec = {scrollViewRec.x, scrollViewRec.y, scrollViewRec.width, totalContentHeight};
    
    // Draw map items
    for (size_t i = 0; i < availableMaps.size(); i++) {
        float itemY = scrollViewRec.y + (i * itemHeight) + scrollOffset.y;
        
        // Only draw items that are visible
        if (itemY + itemHeight >= scrollViewRec.y && itemY <= scrollViewRec.y + scrollViewRec.height) {
            Rectangle itemRect = {scrollViewRec.x, itemY, 
                                scrollViewRec.width, itemHeight};
            
            // Validate item rectangle
            if (itemRect.width <= 0 || itemRect.height <= 0) {
                continue;
            }
            
            // Draw selection highlight
            if ((int)i == selectedMapIndex) {
                DrawRectangleRec(itemRect, Fade(BLUE, 0.3f));
                DrawRectangleLinesEx(itemRect, 2.0f, BLUE);
            } else {
                // Draw hover effect
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, itemRect) && 
                    CheckCollisionPointRec(mousePos, scrollViewRec)) {
                    DrawRectangleRec(itemRect, Fade(GRAY, 0.2f));
                }
            }
            
            // Draw map name - ensure we don't exceed string bounds
            if (!availableMaps[i].empty()) {
                float textSize = fontSize - 15.0f;
                Vector2 textPos = {itemRect.x + 10.0f, itemRect.y + (itemRect.height - textSize) / 2.0f};
                DrawTextEx(font, availableMaps[i].c_str(), textPos, textSize, 0.0f, BLACK);
            }
        }
    }
    
    // Draw scrollbar if content exceeds view height
    if (totalContentHeight > scrollViewRec.height && totalContentHeight > 0) {
        float scrollbarWidth = 15.0f;
        float scrollbarX = scrollViewRec.x + scrollViewRec.width - scrollbarWidth;
        float scrollbarHeight = scrollViewRec.height;
        
        // Draw scrollbar background
        Rectangle scrollbarBg = {scrollbarX, scrollViewRec.y, scrollbarWidth, scrollbarHeight};
        DrawRectangleRec(scrollbarBg, Fade(DARKGRAY, 0.3f));
        
        // Calculate thumb position and size
        float thumbHeight = (scrollViewRec.height / totalContentHeight) * scrollbarHeight;
        thumbHeight = std::fmax(thumbHeight, 20.0f); // Minimum thumb size
        
        float maxScroll = totalContentHeight - scrollViewRec.height;
        if (maxScroll > 0) {
            float scrollRatio = -scrollOffset.y / maxScroll;
            scrollRatio = std::clamp(scrollRatio, 0.0f, 1.0f); // Clamp between 0 and 1
            float thumbY = scrollViewRec.y + (scrollRatio * (scrollbarHeight - thumbHeight));
            
            Rectangle scrollThumb = {scrollbarX, thumbY, scrollbarWidth, thumbHeight};
            DrawRectangleRec(scrollThumb, DARKGRAY);
            DrawRectangleLinesEx(scrollThumb, 1.0f, BLACK);
        }
    }
    
    EndScissorMode();
}

void MapEditorScreen1::loadUserDesignedMap(int mapIndex) {
    if (mapIndex >= 0 && mapIndex < userDesignedMaps.size()) {
        const UserMapData& mapData = userDesignedMaps[mapIndex];
        std::cout << "Loading user-designed map: " << mapData.displayName << std::endl;
        std::cout << "Entities count: " << mapData.entitiesID.size() << std::endl;
        std::cout << "Background color: [" << (int)mapData.backgroundColor.r << ", " 
                  << (int)mapData.backgroundColor.g << ", " << (int)mapData.backgroundColor.b 
                  << ", " << (int)mapData.backgroundColor.a << "]" << std::endl;
        std::cout << "Background ID: " << mapData.backgroundID << std::endl;
        
        // TODO: Implement map loading functionality
        // This would typically involve:
        // 1. Setting up the map editor with the loaded data
        // 2. Transitioning to the map editor screen
        
        std::cout << "Map loaded from: " << mapData.filename << std::endl;
    } else {
        std::cerr << "Invalid map index: " << mapIndex << std::endl;
    }
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
    std::string mapsDirectory = getUserDesignedMapsDirectory();
    std::string fullPath = mapsDirectory + "/" + filename;
    
    try {
        // Create directory if it doesn't exist
        std::filesystem::create_directories(mapsDirectory);
        
        // Create JSON structure
        json mapJson;
        mapJson["EntitiesID"] = mapData.entitiesID;
        mapJson["backgroundColor"] = {mapData.backgroundColor.r, mapData.backgroundColor.g, 
                                     mapData.backgroundColor.b, mapData.backgroundColor.a};
        mapJson["backgroundID"] = mapData.backgroundID;
        
        // Write to file
        std::ofstream file(fullPath);
        if (file.is_open()) {
            file << mapJson.dump(2); // Pretty print with 2-space indentation
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
