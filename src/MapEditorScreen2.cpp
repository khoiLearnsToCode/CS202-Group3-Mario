#include "MapEditorScreen1.h"
#include "MapEditorScreen2.h"
#include "GameWorld.h"
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"
#include <iostream>

const float MapEditorScreen2::BASE_TILE_SIZE = 32.0f;

MapEditorScreen2::MapEditorScreen2(MapEditorScreen1* parentScreen)
    : Screen(), currentMapData(nullptr), MainState(MAP_EDITOR_STATE_IDLE),
                                    AuxiliaryState(MAP_EDITOR_STATE_TILE), isDragging(false),
                                    selectedEntityID(0), lastSelectedEntityID(1), selectedCategoryIndex(0), toolsScrollOffset({0.0f, 0.0f}),
                                    hoveredGridPos({-1, -1}), isHoveringGrid(false), isDrawing(false), 
                                    eraserSize(1), brushSize(1) {

        // Initialize camera for grid view
        gridCamera.target = {0.0f, 0.0f};
        gridCamera.offset = {200.0f, 200.0f}; // Start with some offset
        gridCamera.rotation = 0.0f;
        gridCamera.zoom = 1.0f;
        
        lastMousePosition = {0.0f, 0.0f};
        
        // Initialize categories and entity IDs
        initializeCategories();

        try {
            // Set the parent screen
            parentScreen->setMapEditorScreen2(this);
        } catch (const std::exception& e) {
            std::cerr << "Error initializing MapEditorScreen2: " << e.what() << std::endl;
        }
} 

// MapEditorScreen1 will handle the deletion of the currentMapData pointer

void MapEditorScreen2::setCurrentMapData(UserMapData* mapData) {
    currentMapData = mapData;
}

void MapEditorScreen2::update() {
    Vector2 mousePosition = GetMousePosition();
    
    // Handle tool selection in tools area
    float toolsAreaWidth = GetScreenWidth() * 0.2f;
    float toolsAreaX = GetScreenWidth() - toolsAreaWidth;
    
    if (mousePosition.x >= toolsAreaX) {
        // Mouse is in tools area
        handleToolSelection(mousePosition);
    } else {
        // Mouse is in grid area
        handleGridPlacement(mousePosition);
        
        // Handle hover detection for grid
        updateHoverState(mousePosition);
        
        // Handle zoom with mouse wheel
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            // Zoom towards mouse position
            Vector2 mouseWorldPos = GetScreenToWorld2D(mousePosition, gridCamera);
            
            gridCamera.offset = mousePosition;
            gridCamera.target = mouseWorldPos;
            
            // Zoom factor
            float zoomFactor = 1.0f + (0.25f * fabsf(wheel));
            if (wheel < 0) zoomFactor = 1.0f / zoomFactor;
            gridCamera.zoom = Clamp(gridCamera.zoom * zoomFactor, 0.1f, 3.0f);
        }
        
        // Handle panning with right mouse button (only if not drawing)
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !isDrawing) {
            lastMousePosition = mousePosition;
            isDragging = true;
        }
        
        if (isDragging && IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !isDrawing) {
            Vector2 delta = Vector2Subtract(mousePosition, lastMousePosition);
            gridCamera.target = Vector2Subtract(gridCamera.target, Vector2Scale(delta, 1.0f / gridCamera.zoom));
            lastMousePosition = mousePosition;
        }
        
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            isDragging = false;
        }
        
        // Stop drawing if user releases left mouse button
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isDrawing = false;
        }
    }
    
    // Handle input here
    if (IsKeyPressed(KEY_ESCAPE)) {
        // Return to MapEditorScreen1
        GameWorld::state = GAME_STATE_MAP_EDITOR_SCREEN1;
    }
}

void MapEditorScreen2::draw() {
    // Clear background with a light gray color
    ClearBackground(LIGHTGRAY);
    
    // Draw title
    Font font = ResourceManager::getInstance().getFont("SuperMario256");
    const char* title = "MAP EDITOR - DESIGN MODE";
    Vector2 titleSize = MeasureTextEx(font, title, 40.0f, 0.0f);
    DrawTextEx(font, title, 
               {20.0f, 20.0f}, 
               40.0f, 0.0f, BLACK);
    
    // Display current map name
    if (currentMapData) {
        const char* mapName = currentMapData->displayName.c_str();
        Vector2 nameSize = MeasureTextEx(font, mapName, 30.0f, 0.0f);
        DrawTextEx(font, mapName, 
                   {20.0f, 70.0f}, 
                   30.0f, 0.0f, DARKBLUE);
    }
    
    // Draw the 200x60 grid
    drawGrid();
    
    // Draw tools area
    drawToolsArea();
    
    // Draw hover coordinates at bottom right
    if (isHoveringGrid) {
        const char* hoverInfo = TextFormat("Grid: (%d, %d)", (int)hoveredGridPos.x, (int)hoveredGridPos.y);
        Vector2 hoverSize = MeasureTextEx(font, hoverInfo, 18.0f, 0.0f);
        float toolsAreaWidth = GetScreenWidth() * 0.2f;
        DrawTextEx(font, hoverInfo, 
                   {GetScreenWidth() - toolsAreaWidth - hoverSize.x - 20.0f, GetScreenHeight() - 30.0f}, 
                   18.0f, 0.0f, DARKBLUE);
    }
}

void MapEditorScreen2::drawGrid() {
    // Calculate the grid area (entire left space minus tools area)
    float toolsAreaWidth = GetScreenWidth() * 0.2f;
    float gridAreaWidth = GetScreenWidth() - toolsAreaWidth;
    float gridAreaHeight = GetScreenHeight() - 160.0f; // Leave space for title and instructions
    float gridAreaX = 0.0f;
    float gridAreaY = 120.0f;
    
    // Create a scissor region for the grid area to prevent drawing outside it
    BeginScissorMode((int)gridAreaX, (int)gridAreaY, (int)gridAreaWidth, (int)gridAreaHeight);
    
    // Draw grid area background
    DrawRectangle((int)gridAreaX, (int)gridAreaY, (int)gridAreaWidth, (int)gridAreaHeight, WHITE);
    
    // Begin camera mode for zoom and pan
    BeginMode2D(gridCamera);
    
    // Calculate actual tile size with zoom
    float actualTileSize = BASE_TILE_SIZE * gridCamera.zoom;
    
    // Calculate grid bounds in world space
    float gridWorldWidth = GRID_WIDTH_IN_TILES * BASE_TILE_SIZE;
    float gridWorldHeight = GRID_HEIGHT_IN_TILES * BASE_TILE_SIZE;
    
    // Draw grid background (behind tiles)
    Color bgColor = currentMapData ? currentMapData->backgroundColor : LIGHTGRAY;
    DrawRectangle(0, 0, (int)gridWorldWidth, (int)gridWorldHeight, bgColor);
    DrawRectangleLines(0, 0, (int)gridWorldWidth, (int)gridWorldHeight, BLACK);
    
    // Draw grid lines (visible when zoomed in enough)
    if (actualTileSize >= 8.0f) {
        // Draw major grid lines (every 10 tiles)
        for (int x = 0; x <= GRID_WIDTH_IN_TILES; x += 10) {
            float lineX = x * BASE_TILE_SIZE;
            DrawLine((int)lineX, 0, (int)lineX, (int)gridWorldHeight, GRAY);
        }
        
        for (int y = 0; y <= GRID_HEIGHT_IN_TILES; y += 10) {
            float lineY = y * BASE_TILE_SIZE;
            DrawLine(0, (int)lineY, (int)gridWorldWidth, (int)lineY, GRAY);
        }
        
        // Draw minor grid lines (every tile) when zoomed in enough
        if (actualTileSize >= 16.0f) {
            for (int x = 0; x <= GRID_WIDTH_IN_TILES; x++) {
                float lineX = x * BASE_TILE_SIZE;
                DrawLine((int)lineX, 0, (int)lineX, (int)gridWorldHeight, Fade(GRAY, 0.5f));
            }
            
            for (int y = 0; y <= GRID_HEIGHT_IN_TILES; y++) {
                float lineY = y * BASE_TILE_SIZE;
                DrawLine(0, (int)lineY, (int)gridWorldWidth, (int)lineY, Fade(GRAY, 0.5f));
            }
        }
    }
    
    // Draw individual tiles with entity data if available
    if (currentMapData && currentMapData->entitiesID.size() == 12000) {
        for (int y = 0; y < GRID_HEIGHT_IN_TILES; y++) {
            for (int x = 0; x < GRID_WIDTH_IN_TILES; x++) {
                int index = y * GRID_WIDTH_IN_TILES + x;
                int entityID = currentMapData->entitiesID[index];               
                // Draw different colors/textures based on entity ID
                if (entityID != 0) {
                    float tileX = x * BASE_TILE_SIZE;
                    float tileY = y * BASE_TILE_SIZE;
                    Rectangle tileRect = {tileX, tileY, BASE_TILE_SIZE, BASE_TILE_SIZE};
                    
                    Color tileColor = GRAY; // Default color
                    
                    // Color based on entity type
                    if (entityID >= 1 && entityID <= 87) {
                        // Tiles - try to load texture, fallback to green
                        std::string tileKey = "tile_" + std::to_string(entityID);
                        if (textures.find(tileKey) != textures.end()) {
                            DrawTexturePro(textures[tileKey], 
                                            {0, 0, (float)textures[tileKey].width, (float)textures[tileKey].height},
                                            tileRect, {0, 0}, 0.0f, WHITE);
                        } else {
                            tileColor = GREEN;
                            DrawRectangleRec(tileRect, tileColor);
                        }
                    } else if (entityID >= 88 && entityID <= 102) {
                        // Blocks - try to load texture, fallback to brown
                        std::string blockKey = "block" + std::to_string(entityID);
                        if (textures.find(blockKey) != textures.end()) {
                            DrawTexturePro(textures[blockKey], 
                                            {0, 0, (float)textures[blockKey].width, (float)textures[blockKey].height},
                                            tileRect, {0, 0}, 0.0f, WHITE);
                        } else {
                            tileColor = BROWN;
                            DrawRectangleRec(tileRect, tileColor);
                        }
                    } else if (entityID >= 103 && entityID <= 120) {
                        // Items - try to load texture, fallback to yellow
                        std::string itemKey = "";
                        if (entityID == 103) {
                            itemKey = "1UpMushroom";
                        } else if (entityID == 104) {
                            itemKey = "3UpMoon";
                        } else if (entityID >= 108 && entityID <= 111) {
                            itemKey = "coin0"; // Use first coin texture for all coin variants
                        } else if (entityID == 112) {
                            itemKey = "courseClearToken";
                        } else if (entityID >= 113 && entityID <= 114) {
                            itemKey = "fireFlower0"; // Use first fire flower texture
                        } else if (entityID == 115) {
                            itemKey = "mushroom";
                        } else if (entityID == 116) {
                            itemKey = "star";
                        } else if (entityID >= 117 && entityID <= 120) {
                            itemKey = "yoshiCoin0"; // Use first yoshi coin texture for all variants
                        }
                        
                        if (!itemKey.empty() && textures.find(itemKey) != textures.end()) {
                            DrawTexturePro(textures[itemKey], 
                                            {0, 0, (float)textures[itemKey].width, (float)textures[itemKey].height},
                                            tileRect, {0, 0}, 0.0f, WHITE);
                        } else {
                            tileColor = YELLOW;
                            DrawRectangleRec(tileRect, tileColor);
                        }
                    } else if (entityID >= 121 && entityID <= 158) {
                        // Baddies - try to load texture, fallback to red
                        std::string baddieKey = "";
                        if (entityID == 121 || entityID == 122) {
                            baddieKey = "blueKoopaTroopa0R";
                        } else if (entityID == 123 || entityID == 124) {
                            baddieKey = "bobOmb0R";
                        } else if (entityID == 125) {
                            baddieKey = "bulletBill0R";
                        } else if (entityID == 126 || entityID == 127) {
                            baddieKey = "buzzyBeetle0R";
                        } else if (entityID >= 128 && entityID <= 131) {
                            baddieKey = "flyingGoomba0R";
                        } else if (entityID == 132 || entityID == 133) {
                            baddieKey = "goomba0R";
                        } else if (entityID == 134 || entityID == 135) {
                            baddieKey = "greenKoopaTroopa0R";
                        } else if (entityID >= 136 && entityID <= 139) {
                            baddieKey = "jumpingPiranhaPlant0";
                        } else if (entityID == 140 || entityID == 141) {
                            baddieKey = "montyMole0R";
                        } else if (entityID == 142 || entityID == 143) {
                            baddieKey = "mummyBeetle0R";
                        } else if (entityID == 144 || entityID == 145) {
                            baddieKey = "muncher0";
                        } else if (entityID == 146 || entityID == 147) {
                            baddieKey = "piranhaPlant0";
                        } else if (entityID == 148 || entityID == 149) {
                            baddieKey = "redKoopaTroopa0R";
                        } else if (entityID >= 150 && entityID <= 153) {
                            baddieKey = "rex10R";
                        } else if (entityID >= 154 && entityID <= 156) {
                            baddieKey = "swooper0R";
                        } else if (entityID ==  157 || entityID == 158) {
                            baddieKey = "yellowKoopaTroopa0R";
                        }
                        
                        if (!baddieKey.empty() && textures.find(baddieKey) != textures.end()) {
                            DrawTexturePro(textures[baddieKey], 
                                            {0, 0, (float)textures[baddieKey].width, (float)textures[baddieKey].height},
                                            tileRect, {0, 0}, 0.0f, WHITE);
                        } else {
                            tileColor = RED;
                            DrawRectangleRec(tileRect, tileColor);
                        }
                    } else {
                        // Unknown entity
                        tileColor = PURPLE;
                        DrawRectangleRec(tileRect, tileColor);
                    }
                }              
            }
        }
    }
    
    // Draw hover highlight
    if (isHoveringGrid && hoveredGridPos.x >= 0 && hoveredGridPos.y >= 0 && 
        hoveredGridPos.x < GRID_WIDTH_IN_TILES && hoveredGridPos.y < GRID_HEIGHT_IN_TILES) {
        
        if (selectedEntityID == 0) {
            // Show eraser size preview
            int halfSize = eraserSize / 2;
            for (int dy = -halfSize; dy <= halfSize; dy++) {
                for (int dx = -halfSize; dx <= halfSize; dx++) {
                    int targetX = (int)hoveredGridPos.x + dx;
                    int targetY = (int)hoveredGridPos.y + dy;
                    
                    if (targetX >= 0 && targetX < GRID_WIDTH_IN_TILES && 
                        targetY >= 0 && targetY < GRID_HEIGHT_IN_TILES) {
                        float tileX = targetX * BASE_TILE_SIZE;
                        float tileY = targetY * BASE_TILE_SIZE;
                        Rectangle hoverRect = {tileX, tileY, BASE_TILE_SIZE, BASE_TILE_SIZE};
                        DrawRectangleRec(hoverRect, Fade(RED, 0.3f));
                        DrawRectangleLinesEx(hoverRect, 1.0f, RED);
                    }
                }
            }
        } else if (selectedEntityID == -1) {
            // Show brush size preview
            int halfSize = brushSize / 2;
            for (int dy = -halfSize; dy <= halfSize; dy++) {
                for (int dx = -halfSize; dx <= halfSize; dx++) {
                    int targetX = (int)hoveredGridPos.x + dx;
                    int targetY = (int)hoveredGridPos.y + dy;
                    
                    if (targetX >= 0 && targetX < GRID_WIDTH_IN_TILES && 
                        targetY >= 0 && targetY < GRID_HEIGHT_IN_TILES) {
                        float tileX = targetX * BASE_TILE_SIZE;
                        float tileY = targetY * BASE_TILE_SIZE;
                        Rectangle hoverRect = {tileX, tileY, BASE_TILE_SIZE, BASE_TILE_SIZE};
                        DrawRectangleRec(hoverRect, Fade(GREEN, 0.3f));
                        DrawRectangleLinesEx(hoverRect, 1.0f, GREEN);
                    }
                }
            }
        } else {
            // Normal single tile hover
            float tileX = hoveredGridPos.x * BASE_TILE_SIZE;
            float tileY = hoveredGridPos.y * BASE_TILE_SIZE;
            Rectangle hoverRect = {tileX, tileY, BASE_TILE_SIZE, BASE_TILE_SIZE};
            DrawRectangleRec(hoverRect, Fade(SKYBLUE, 0.5f));
            DrawRectangleLinesEx(hoverRect, 2.0f, BLUE);
        }
    }
    
    EndMode2D();
    EndScissorMode();
    
    // Draw grid info and controls
    Font font = ResourceManager::getInstance().getFont("SuperMario256");
    const char* gridInfo = TextFormat("Grid: %dx%d tiles | Zoom: %.1fx | Mouse Wheel: Zoom | Right Click: Pan", 
                                     GRID_WIDTH_IN_TILES, GRID_HEIGHT_IN_TILES, gridCamera.zoom);
    DrawTextEx(font, gridInfo, 
               {20.0f, GetScreenHeight() - 80.0f}, 
               18.0f, 0.0f, BLACK);
}

void MapEditorScreen2::initializeCategories() {
    categories = {"TILES", "BLOCKS", "ITEMS", "BADDIES"};
    categoryEntityIDs.resize(categories.size());
    
    // Tiles (IDs 1-87)
    for (int i = 1; i <= 87; i++) {
        categoryEntityIDs[0].push_back(i);
    }
    
    // Blocks (IDs 88-102)
    for (int i = 88; i <= 102; i++) {
        categoryEntityIDs[1].push_back(i);
    }
    
    // Items (IDs 103-120)
    for (int i = 103; i <= 120; i++) {
        categoryEntityIDs[2].push_back(i);
    }
    
    // Baddies (IDs 121-158)
    for (int i = 121; i <= 158; i++) {
        categoryEntityIDs[3].push_back(i);
    }
}

void MapEditorScreen2::drawToolsArea() {
    float toolsAreaWidth = GetScreenWidth() * 0.2f;
    float toolsAreaX = GetScreenWidth() - toolsAreaWidth;
    float toolsAreaY = 0.0f;
    float toolsAreaHeight = GetScreenHeight();
    
    Rectangle toolsArea = {toolsAreaX, toolsAreaY, toolsAreaWidth, toolsAreaHeight};
    DrawRectangleRec(toolsArea, Fade(DARKGRAY, 0.1f));
    DrawRectangleLinesEx(toolsArea, 2.0f, DARKGRAY);
    
    Font font = ResourceManager::getInstance().getFont("SuperMario256");
    
    // Draw title
    const char* toolsTitle = "TOOLS";
    Vector2 titleSize = MeasureTextEx(font, toolsTitle, 24.0f, 0.0f);
    DrawTextEx(font, toolsTitle, 
               {toolsAreaX + (toolsAreaWidth - titleSize.x) / 2.0f, 10.0f}, 
               24.0f, 0.0f, BLACK);
    
    // Draw selected entity info
    if (selectedEntityID > 0) {
        const char* selectedInfo = TextFormat("Selected: ID %d", selectedEntityID);
        DrawTextEx(font, selectedInfo, 
                   {toolsAreaX + 10.0f, 40.0f}, 
                   16.0f, 0.0f, DARKBLUE);
    } else if (selectedEntityID == 0) {
        DrawTextEx(font, "Selected: ERASER", 
                   {toolsAreaX + 10.0f, 40.0f}, 
                   16.0f, 0.0f, RED);
    } else if (selectedEntityID == -1) {
        DrawTextEx(font, "Selected: BRUSH", 
                   {toolsAreaX + 10.0f, 40.0f}, 
                   16.0f, 0.0f, GREEN);
    }
    
    float currentY = 70.0f;
    
    // Always reserve space for size area to keep layout consistent
    if (selectedEntityID == 0) {
        // Draw eraser size slider when eraser is selected
        drawEraserSizeArea(toolsAreaX, toolsAreaWidth, currentY);
    } else if (selectedEntityID == -1) {
        // Draw brush size slider when brush is selected
        drawBrushSizeArea(toolsAreaX, toolsAreaWidth, currentY);
    } else {
        // Reserve the same space even when neither tool is selected
        currentY += 75.0f; // Same height as size areas
    }
    
    float categoryHeight = 25.0f;
    float itemSize = 28.0f;
    float itemSpacing = 3.0f;
    float categoryWidth = (toolsAreaWidth - 20.0f) / categories.size(); // Divide width equally
    
    // Draw categories horizontally
    for (int catIndex = 0; catIndex < categories.size(); catIndex++) {
        Color categoryColor = (catIndex == selectedCategoryIndex) ? BLUE : DARKGRAY;
        
        float categoryX = toolsAreaX + 5.0f + catIndex * categoryWidth;
        
        // Draw category header
        Rectangle categoryRect = {categoryX, currentY, categoryWidth - 2.0f, categoryHeight};
        DrawRectangleRec(categoryRect, Fade(categoryColor, 0.3f));
        DrawRectangleLinesEx(categoryRect, 1.0f, categoryColor);
        
        // Draw category text (smaller to fit)
        Vector2 textSize = MeasureTextEx(font, categories[catIndex].c_str(), 12.0f, 0.0f);
        float textX = categoryX + (categoryWidth - textSize.x) / 2.0f;
        DrawTextEx(font, categories[catIndex].c_str(), 
                   {textX, currentY + 6.0f}, 
                   12.0f, 0.0f, categoryColor);
    }
    
    currentY += categoryHeight + 15.0f; // Increased spacing to lower the object sections
    
    // Show items for the selected category (all categories now contain actual entities)
    if (selectedCategoryIndex >= 0 && selectedCategoryIndex < categoryEntityIDs.size()) {
        float itemX = toolsAreaX + 10.0f;
        float itemY = currentY;
        int itemsPerRow = (int)((toolsAreaWidth - 20.0f) / (itemSize + itemSpacing));
        
        // Reserve space for permanent tools at bottom (120px)
        float maxItemY = toolsAreaHeight - 120.0f;
        
        for (int i = 0; i < categoryEntityIDs[selectedCategoryIndex].size(); i++) {
            int entityID = categoryEntityIDs[selectedCategoryIndex][i];
            
            // Calculate position
            int row = i / itemsPerRow;
            int col = i % itemsPerRow;
            float x = itemX + col * (itemSize + itemSpacing);
            float y = itemY + row * (itemSize + itemSpacing);
            
            // Check if we're still within the available area (reserve space for tools)
            if (y + itemSize > maxItemY) break;
            
            Rectangle itemRect = {x, y, itemSize, itemSize};
            
            // Highlight selected item with outline (draw first, bigger than item)
            // In brush mode, highlight the entity that will be placed (lastSelectedEntityID)
            int entityToHighlight = (selectedEntityID == -1) ? lastSelectedEntityID : selectedEntityID;
            if (entityID == entityToHighlight) {
                // Draw bigger outline around the item
                float outlineSize = 2.0f;
                Rectangle outlineRect = {x - outlineSize, y - outlineSize, 
                                       itemSize + outlineSize * 2, itemSize + outlineSize * 2};
                DrawRectangleLinesEx(outlineRect, 3.0f, ORANGE);
            }
            
            // Draw entity representation
            if (entityID >= 1 && entityID <= 87) {
                // Tiles - try to draw texture or colored square
                std::string tileKey = "tile_" + std::to_string(entityID);
                if (textures.find(tileKey) != textures.end()) {
                    DrawTexturePro(textures[tileKey], 
                                 {0, 0, (float)textures[tileKey].width, (float)textures[tileKey].height},
                                 itemRect, {0, 0}, 0.0f, WHITE);
                } else {
                    DrawRectangleRec(itemRect, GREEN);
                }
            } else if (entityID >= 88 && entityID <= 102) {
                // Blocks - try to draw texture or colored square
                std::string blockKey = "block" + std::to_string(entityID);
                if (textures.find(blockKey) != textures.end()) {
                    DrawTexturePro(textures[blockKey], 
                                 {0, 0, (float)textures[blockKey].width, (float)textures[blockKey].height},
                                 itemRect, {0, 0}, 0.0f, WHITE);
                } else {
                    DrawRectangleRec(itemRect, BROWN);
                }
            } else if (entityID >= 103 && entityID <= 120) {
                // Items - try to draw texture or colored square
                std::string itemKey = "";
                if (entityID == 103) {
                    itemKey = "1UpMushroom";
                } else if (entityID == 104) {
                    itemKey = "3UpMoon";
                } else if (entityID >= 108 && entityID <= 111) {
                    itemKey = "coin0"; // Use first coin texture for all coin variants
                } else if (entityID == 112) {
                    itemKey = "courseClearToken";
                } else if (entityID >= 113 && entityID <= 114) {
                    itemKey = "fireFlower0"; // Use first fire flower texture
                } else if (entityID == 115) {
                    itemKey = "mushroom";
                } else if (entityID == 116) {
                    itemKey = "star";
                } else if (entityID >= 117 && entityID <= 120) {
                    itemKey = "yoshiCoin0"; // Use first yoshi coin texture for all variants
                }
                
                if (!itemKey.empty() && textures.find(itemKey) != textures.end()) {
                    DrawTexturePro(textures[itemKey], 
                                 {0, 0, (float)textures[itemKey].width, (float)textures[itemKey].height},
                                 itemRect, {0, 0}, 0.0f, WHITE);
                } else {
                    DrawRectangleRec(itemRect, GOLD);
                }
            } else if (entityID >= 121 && entityID <= 158) {
                // Baddies - try to draw texture or colored square
                std::string baddieKey = "";
                if (entityID == 121 || entityID == 122) {
                    baddieKey = "blueKoopaTroopa0R";
                } else if (entityID == 123 || entityID == 124) {
                    baddieKey = "bobOmb0R";
                } else if (entityID == 125) {
                    baddieKey = "bulletBill0R";
                } else if (entityID == 126 || entityID == 127) {
                    baddieKey = "buzzyBeetle0R";
                } else if (entityID >= 128 && entityID <= 131) {
                    baddieKey = "flyingGoomba0R";
                } else if (entityID == 132 || entityID == 133) {
                    baddieKey = "goomba0R";
                } else if (entityID == 134 || entityID == 135) {
                    baddieKey = "greenKoopaTroopa0R";
                } else if (entityID >= 136 && entityID <= 139) {
                    baddieKey = "jumpingPiranhaPlant0";
                } else if (entityID == 140 || entityID == 141) {
                    baddieKey = "montyMole0R";
                } else if (entityID == 142 || entityID == 143) {
                    baddieKey = "mummyBeetle0R";
                } else if (entityID == 144 || entityID == 145) {
                    baddieKey = "muncher0";
                } else if (entityID == 146 || entityID == 147) {
                    baddieKey = "piranhaPlant0";
                } else if (entityID == 148 || entityID == 149) {
                    baddieKey = "redKoopaTroopa0R";
                } else if (entityID >= 150 && entityID <= 153) {
                    baddieKey = "rex10R";
                } else if (entityID >= 154 && entityID <= 156) {
                    baddieKey = "swooper0R";
                } else if (entityID == 157 || entityID == 158) {
                    baddieKey = "yellowKoopaTroopa0R";
                }
                
                if (!baddieKey.empty() && textures.find(baddieKey) != textures.end()) {
                    DrawTexturePro(textures[baddieKey], 
                                 {0, 0, (float)textures[baddieKey].width, (float)textures[baddieKey].height},
                                 itemRect, {0, 0}, 0.0f, WHITE);
                } else {
                    DrawRectangleRec(itemRect, RED);
                }
            }
        }
    }
    
    // Draw permanent tools section at the bottom right
    drawPermanentToolsSection(toolsAreaX, toolsAreaWidth, toolsAreaHeight);
}

void MapEditorScreen2::handleToolSelection(Vector2 mousePos) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
    
    float toolsAreaWidth = GetScreenWidth() * 0.2f;
    float toolsAreaX = GetScreenWidth() - toolsAreaWidth;
    float toolsAreaHeight = GetScreenHeight();
    
    // Check permanent tools section first (at the bottom)
    float sectionHeight = 100.0f;
    float sectionY = toolsAreaHeight - sectionHeight - 10.0f;
    float buttonSize = 32.0f;
    float buttonSpacing = 10.0f;
    float buttonsY = sectionY + 25.0f;
    
    // Check eraser button
    float eraserX = toolsAreaX + 15.0f;
    Rectangle eraserRect = {eraserX, buttonsY, buttonSize, buttonSize};
    if (CheckCollisionPointRec(mousePos, eraserRect)) {
        selectedEntityID = 0; // Eraser
        return;
    }
    
    // Check brush button
    float brushX = eraserX + buttonSize + buttonSpacing;
    Rectangle brushRect = {brushX, buttonsY, buttonSize, buttonSize};
    if (CheckCollisionPointRec(mousePos, brushRect)) {
        selectedEntityID = -1; // Brush
        return;
    }
    
    float currentY = 70.0f;
    
    // Always skip the same space for size area to keep layout consistent
    currentY += 75.0f; // Same height as size areas (whether shown or not)
    
    float categoryHeight = 25.0f;
    float itemSize = 28.0f;
    float itemSpacing = 3.0f;
    float categoryWidth = (toolsAreaWidth - 20.0f) / categories.size();
    
    // Check category headers (now horizontal, only for TILES, BLOCKS, ITEMS, BADDIES)
    for (int catIndex = 0; catIndex < categories.size(); catIndex++) {
        float categoryX = toolsAreaX + 5.0f + catIndex * categoryWidth;
        Rectangle categoryRect = {categoryX, currentY, categoryWidth - 2.0f, categoryHeight};
        
        if (CheckCollisionPointRec(mousePos, categoryRect)) {
            selectedCategoryIndex = catIndex;
            return;
        }
    }
    
    // Check for item clicks in the selected category area
    currentY += categoryHeight + 15.0f; // Match the spacing from drawToolsArea
    
    if (selectedCategoryIndex >= 0 && selectedCategoryIndex < categoryEntityIDs.size()) {
        float itemX = toolsAreaX + 10.0f;
        float itemY = currentY;
        int itemsPerRow = (int)((toolsAreaWidth - 20.0f) / (itemSize + itemSpacing));
        
        // Reserve space for permanent tools at bottom (120px)
        float maxItemY = toolsAreaHeight - 120.0f;
        
        for (int i = 0; i < categoryEntityIDs[selectedCategoryIndex].size(); i++) {
            int entityID = categoryEntityIDs[selectedCategoryIndex][i];
            
            int row = i / itemsPerRow;
            int col = i % itemsPerRow;
            float x = itemX + col * (itemSize + itemSpacing);
            float y = itemY + row * (itemSize + itemSpacing);
            
            if (y + itemSize > maxItemY) break;
            
            Rectangle itemRect = {x, y, itemSize, itemSize};
            
            if (CheckCollisionPointRec(mousePos, itemRect)) {
                // Store the last selected entity if it's not a special tool (eraser/brush)
                if (entityID > 0) {
                    lastSelectedEntityID = entityID;
                }
                selectedEntityID = entityID;
                return;
            }
        }
    }
}

void MapEditorScreen2::handleGridPlacement(Vector2 mousePos) {
    if (!currentMapData) return;
    
    // Start drawing when left mouse button is pressed
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        isDrawing = true;
    }
    
    // Stop drawing when left mouse button is released
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        isDrawing = false;
    }
    
    // Brush mode: Draw on mouse press OR when hovering while drawing (continuous brush)
    bool shouldDraw = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || 
                     (isDrawing && IsMouseButtonDown(MOUSE_BUTTON_LEFT));
    
    if (!shouldDraw) return;
    
    // Convert mouse position to world position
    Vector2 worldPos = GetScreenToWorld2D(mousePos, gridCamera);
    
    // Convert world position to grid coordinates
    int gridX = (int)(worldPos.x / BASE_TILE_SIZE);
    int gridY = (int)(worldPos.y / BASE_TILE_SIZE);
    
    // Check bounds
    if (gridX >= 0 && gridX < GRID_WIDTH_IN_TILES && gridY >= 0 && gridY < GRID_HEIGHT_IN_TILES) {
        // Handle eraser size
        if (selectedEntityID == 0) {
            // Erase in a square pattern based on eraser size
            int halfSize = eraserSize / 2;
            for (int dy = -halfSize; dy <= halfSize; dy++) {
                for (int dx = -halfSize; dx <= halfSize; dx++) {
                    int targetX = gridX + dx;
                    int targetY = gridY + dy;
                    
                    if (targetX >= 0 && targetX < GRID_WIDTH_IN_TILES && 
                        targetY >= 0 && targetY < GRID_HEIGHT_IN_TILES) {
                        int index = targetY * GRID_WIDTH_IN_TILES + targetX;
                        if (index >= 0 && index < currentMapData->entitiesID.size()) {
                            currentMapData->entitiesID[index] = 0; // Erase (set to 0)
                        }
                    }
                }
            }
        } else if (selectedEntityID == -1) {
            // Brush mode: Use the last selected entity (not -1)
            int entityToPlace = lastSelectedEntityID;
            
            // Place in a square pattern based on brush size
            int halfSize = brushSize / 2;
            for (int dy = -halfSize; dy <= halfSize; dy++) {
                for (int dx = -halfSize; dx <= halfSize; dx++) {
                    int targetX = gridX + dx;
                    int targetY = gridY + dy;
                    
                    if (targetX >= 0 && targetX < GRID_WIDTH_IN_TILES && 
                        targetY >= 0 && targetY < GRID_HEIGHT_IN_TILES) {
                        int index = targetY * GRID_WIDTH_IN_TILES + targetX;
                        if (index >= 0 && index < currentMapData->entitiesID.size()) {
                            currentMapData->entitiesID[index] = entityToPlace;
                        }
                    }
                }
            }
        } else {
            // Single placement mode: Place tiles smoothly as mouse moves
            int index = gridY * GRID_WIDTH_IN_TILES + gridX;
            if (index >= 0 && index < currentMapData->entitiesID.size()) {
                // Only place if the current tile is different to avoid redundant operations
                if (currentMapData->entitiesID[index] != selectedEntityID) {
                    currentMapData->entitiesID[index] = selectedEntityID;
                }
            }
        }
    }
}

void MapEditorScreen2::updateHoverState(Vector2 mousePos) {
    // Check if mouse is in grid area (not in tools area)
    float toolsAreaWidth = GetScreenWidth() * 0.2f;
    float gridAreaX = 0.0f;
    float gridAreaY = 120.0f;
    float gridAreaWidth = GetScreenWidth() - toolsAreaWidth;
    float gridAreaHeight = GetScreenHeight() - 160.0f;
    
    if (mousePos.x >= gridAreaX && mousePos.x <= gridAreaX + gridAreaWidth &&
        mousePos.y >= gridAreaY && mousePos.y <= gridAreaY + gridAreaHeight) {
        
        // Convert mouse position to world position
        Vector2 worldPos = GetScreenToWorld2D(mousePos, gridCamera);
        
        // Convert world position to grid coordinates
        int gridX = (int)(worldPos.x / BASE_TILE_SIZE);
        int gridY = (int)(worldPos.y / BASE_TILE_SIZE);
        
        // Check bounds
        if (gridX >= 0 && gridX < GRID_WIDTH_IN_TILES && gridY >= 0 && gridY < GRID_HEIGHT_IN_TILES) {
            hoveredGridPos = {(float)gridX, (float)gridY};
            isHoveringGrid = true;
        } else {
            isHoveringGrid = false;
        }
    } else {
        isHoveringGrid = false;
    }
}

void MapEditorScreen2::drawEraserSizeArea(float toolsAreaX, float toolsAreaWidth, float& currentY) {
    Font font = ResourceManager::getInstance().getFont("SuperMario256");
    
    // Draw eraser size label
    const char* sizeLabel = "Eraser Size:";
    DrawTextEx(font, sizeLabel, 
               {toolsAreaX + 10.0f, currentY}, 
               16.0f, 0.0f, BLACK);
    currentY += 25.0f;
    
    // Draw size value
    const char* sizeValue = TextFormat("%d x %d", eraserSize, eraserSize);
    DrawTextEx(font, sizeValue, 
               {toolsAreaX + 10.0f, currentY}, 
               14.0f, 0.0f, DARKBLUE);
    currentY += 20.0f;
    
    // Draw raygui slider
    Rectangle sliderRect = {toolsAreaX + 10.0f, currentY, toolsAreaWidth - 30.0f, 20.0f};
    
    // Convert eraser size to slider value (0.0 to 1.0)
    float sliderValue = (float)(eraserSize - MIN_ERASER_SIZE) / (float)(MAX_ERASER_SIZE - MIN_ERASER_SIZE);
    
    // Use GuiSlider with custom range
    GuiSlider(sliderRect, NULL, NULL, &sliderValue, 0.0f, 1.0f);
    
    // Convert back to eraser size and ensure odd numbers only
    int newSize = MIN_ERASER_SIZE + (int)(sliderValue * (MAX_ERASER_SIZE - MIN_ERASER_SIZE));
    
    // Ensure odd number
    if (newSize % 2 == 0) {
        // If even, round to nearest odd number
        newSize = (newSize < MAX_ERASER_SIZE) ? newSize + 1 : MAX_ERASER_SIZE;
    }
    
    eraserSize = Clamp(newSize, MIN_ERASER_SIZE, MAX_ERASER_SIZE);
    
    currentY += 30.0f;
}

void MapEditorScreen2::drawBrushSizeArea(float toolsAreaX, float toolsAreaWidth, float& currentY) {
    Font font = ResourceManager::getInstance().getFont("SuperMario256");
    
    // Draw brush size label
    const char* sizeLabel = "Brush Size:";
    DrawTextEx(font, sizeLabel, 
               {toolsAreaX + 10.0f, currentY}, 
               16.0f, 0.0f, BLACK);
    currentY += 25.0f;
    
    // Draw size value
    const char* sizeValue = TextFormat("%d x %d", brushSize, brushSize);
    DrawTextEx(font, sizeValue, 
               {toolsAreaX + 10.0f, currentY}, 
               14.0f, 0.0f, DARKGREEN);
    currentY += 20.0f;
    
    // Draw raygui slider
    Rectangle sliderRect = {toolsAreaX + 10.0f, currentY, toolsAreaWidth - 30.0f, 20.0f};
    
    // Convert brush size to slider value (0.0 to 1.0)
    float sliderValue = (float)(brushSize - MIN_BRUSH_SIZE) / (float)(MAX_BRUSH_SIZE - MIN_BRUSH_SIZE);
    
    // Use GuiSlider with custom range
    GuiSlider(sliderRect, NULL, NULL, &sliderValue, 0.0f, 1.0f);
    
    // Convert back to brush size and ensure odd numbers only
    int newSize = MIN_BRUSH_SIZE + (int)(sliderValue * (MAX_BRUSH_SIZE - MIN_BRUSH_SIZE));
    
    // Ensure odd number
    if (newSize % 2 == 0) {
        // If even, round to nearest odd number
        newSize = (newSize < MAX_BRUSH_SIZE) ? newSize + 1 : MAX_BRUSH_SIZE;
    }
    
    brushSize = Clamp(newSize, MIN_BRUSH_SIZE, MAX_BRUSH_SIZE);
    
    currentY += 30.0f;
}

void MapEditorScreen2::drawPermanentToolsSection(float toolsAreaX, float toolsAreaWidth, float toolsAreaHeight) {
    Font font = ResourceManager::getInstance().getFont("SuperMario256");
    
    float sectionHeight = 100.0f;
    float sectionY = toolsAreaHeight - sectionHeight - 10.0f;
    
    // Draw background for permanent tools section
    Rectangle toolsSectionRect = {toolsAreaX + 5.0f, sectionY, toolsAreaWidth - 10.0f, sectionHeight};
    DrawRectangleRec(toolsSectionRect, Fade(DARKBLUE, 0.1f));
    DrawRectangleLinesEx(toolsSectionRect, 2.0f, DARKBLUE);
    
    // Draw section title
    const char* sectionTitle = "TOOLS";
    Vector2 titleSize = MeasureTextEx(font, sectionTitle, 16.0f, 0.0f);
    DrawTextEx(font, sectionTitle, 
               {toolsAreaX + 10.0f, sectionY + 5.0f}, 
               16.0f, 0.0f, DARKBLUE);
    
    // Draw eraser and brush buttons
    float buttonSize = 32.0f;
    float buttonSpacing = 10.0f;
    float buttonsY = sectionY + 25.0f;
    
    // Eraser button
    float eraserX = toolsAreaX + 15.0f;
    Rectangle eraserRect = {eraserX, buttonsY, buttonSize, buttonSize};
    Color eraserColor = (selectedEntityID == 0) ? RED : GRAY;
    DrawRectangleRec(eraserRect, Fade(eraserColor, 0.3f));
    DrawRectangleLinesEx(eraserRect, 2.0f, eraserColor);
    
    // Draw X for eraser
    DrawLine((int)eraserX + 6, (int)buttonsY + 6, (int)(eraserX + buttonSize - 6), (int)(buttonsY + buttonSize - 6), eraserColor);
    DrawLine((int)(eraserX + buttonSize - 6), (int)buttonsY + 6, (int)eraserX + 6, (int)(buttonsY + buttonSize - 6), eraserColor);
    
    // Eraser label
    DrawTextEx(font, "ERASER", 
               {eraserX, buttonsY + buttonSize + 3.0f}, 
               10.0f, 0.0f, eraserColor);
    
    // Brush button
    float brushX = eraserX + buttonSize + buttonSpacing;
    Rectangle brushRect = {brushX, buttonsY, buttonSize, buttonSize};
    Color brushColor = (selectedEntityID == -1) ? GREEN : GRAY;
    DrawRectangleRec(brushRect, Fade(brushColor, 0.3f));
    DrawRectangleLinesEx(brushRect, 2.0f, brushColor);
    
    // Draw brush icon (filled square)
    float iconSize = buttonSize * 0.6f;
    float iconOffset = (buttonSize - iconSize) / 2.0f;
    DrawRectangle((int)(brushX + iconOffset), (int)(buttonsY + iconOffset), (int)iconSize, (int)iconSize, brushColor);
    
    // Brush label
    DrawTextEx(font, "BRUSH", 
               {brushX, buttonsY + buttonSize + 3.0f}, 
               10.0f, 0.0f, brushColor);
    
    // Show size info only for eraser (brush info is already shown in slider area)
    float infoY = buttonsY + buttonSize + 20.0f;
    if (selectedEntityID == 0) {
        const char* sizeInfo = TextFormat("Size: %dx%d", eraserSize, eraserSize);
        DrawTextEx(font, sizeInfo, 
                   {toolsAreaX + 10.0f, infoY}, 
                   12.0f, 0.0f, RED);
    }
}

