#include "MapEditorScreen1.h"
#include "MapEditorScreen2.h"
#include "GameWorld.h"
#include "raylib.h"
#include "raymath.h"
//#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <iostream>

// Define static constants
const float MapEditorScreen2::BASE_TILE_SIZE = 32.0f;

MapEditorScreen2::MapEditorScreen2(MapEditorScreen1* parentScreen)
    : Screen(), currentMapData(nullptr), MainState(MAP_EDITOR_STATE_IDLE),
                                    AuxiliaryState(MAP_EDITOR_STATE_TILE), isDragging(false),
                                    selectedEntityID(0), selectedCategoryIndex(0), toolsScrollOffset({0.0f, 0.0f}),
                                    hoveredGridPos({-1, -1}), isHoveringGrid(false), isDrawing(false), 
                                    eraserSize(1) {

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
    
    // Draw instructions and hover info
    const char* instructions = "ESC: Return to menu | Mouse Wheel: Zoom | Right Click + Drag: Pan | Left Click + Drag: Draw";
    Vector2 instructSize = MeasureTextEx(font, instructions, 20.0f, 0.0f);
    DrawTextEx(font, instructions, 
               {20.0f, GetScreenHeight() - 40.0f}, 
               20.0f, 0.0f, DARKGRAY);
               
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
                if (index < currentMapData->entitiesID.size()) {
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
                            // Items
                            tileColor = GOLD;
                            DrawRectangleRec(tileRect, tileColor);
                        } else if (entityID >= 121 && entityID <= 158) {
                            // Baddies
                            tileColor = RED;
                            DrawRectangleRec(tileRect, tileColor);
                        } else {
                            // Unknown entity
                            tileColor = PURPLE;
                            DrawRectangleRec(tileRect, tileColor);
                        }
                        
                        // Draw tile borders when zoomed in enough (for non-textured tiles)
                        if (actualTileSize >= 12.0f && (entityID < 1 || entityID > 102 || 
                            textures.find("tile_" + std::to_string(entityID)) == textures.end() &&
                            textures.find("block" + std::to_string(entityID)) == textures.end())) {
                            DrawRectangleLinesEx(tileRect, 1.0f, BLACK);
                        }
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
    categories = {"ERASER", "TILES", "BLOCKS", "ITEMS", "BADDIES"};
    categoryEntityIDs.resize(categories.size());
    
    // Eraser (ID 0)
    categoryEntityIDs[0] = {0};
    
    // Tiles (IDs 1-87)
    for (int i = 1; i <= 87; i++) {
        categoryEntityIDs[1].push_back(i);
    }
    
    // Blocks (IDs 88-102)
    for (int i = 88; i <= 102; i++) {
        categoryEntityIDs[2].push_back(i);
    }
    
    // Items (IDs 103-120)
    for (int i = 103; i <= 120; i++) {
        categoryEntityIDs[3].push_back(i);
    }
    
    // Baddies (IDs 121-158)
    for (int i = 121; i <= 158; i++) {
        categoryEntityIDs[4].push_back(i);
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
    } else {
        DrawTextEx(font, "Selected: ERASER", 
                   {toolsAreaX + 10.0f, 40.0f}, 
                   16.0f, 0.0f, RED);
    }
    
    float currentY = 70.0f;
    
    // Draw eraser size slider if eraser is selected
    if (selectedEntityID == 0) {
        drawEraserSizeArea(toolsAreaX, toolsAreaWidth, currentY);
    }
    float categoryHeight = 30.0f;
    float itemSize = 32.0f;
    float itemSpacing = 4.0f;
    
    // Draw categories
    for (int catIndex = 0; catIndex < categories.size(); catIndex++) {
        Color categoryColor = (catIndex == selectedCategoryIndex) ? BLUE : DARKGRAY;
        
        // Draw category header
        Rectangle categoryRect = {toolsAreaX + 5.0f, currentY, toolsAreaWidth - 10.0f, categoryHeight};
        DrawRectangleRec(categoryRect, Fade(categoryColor, 0.3f));
        DrawRectangleLinesEx(categoryRect, 1.0f, categoryColor);
        
        DrawTextEx(font, categories[catIndex].c_str(), 
                   {toolsAreaX + 10.0f, currentY + 5.0f}, 
                   18.0f, 0.0f, categoryColor);
        
        currentY += categoryHeight + 5.0f;
        
        // If this category is selected, show its items
        if (catIndex == selectedCategoryIndex) {
            float itemX = toolsAreaX + 10.0f;
            float itemY = currentY;
            int itemsPerRow = (int)((toolsAreaWidth - 20.0f) / (itemSize + itemSpacing));
            
            for (int i = 0; i < categoryEntityIDs[catIndex].size(); i++) {
                int entityID = categoryEntityIDs[catIndex][i];
                
                // Calculate position
                int row = i / itemsPerRow;
                int col = i % itemsPerRow;
                float x = itemX + col * (itemSize + itemSpacing);
                float y = itemY + row * (itemSize + itemSpacing);
                
                // Check if we're still within the tools area
                if (y + itemSize > toolsAreaHeight - 20.0f) break;
                
                Rectangle itemRect = {x, y, itemSize, itemSize};
                
                // Highlight selected item with outline (draw first, bigger than item)
                if (entityID == selectedEntityID) {
                    // Draw bigger outline around the item
                    float outlineSize = 2.0f;
                    Rectangle outlineRect = {x - outlineSize, y - outlineSize, 
                                           itemSize + outlineSize * 2, itemSize + outlineSize * 2};
                    DrawRectangleLinesEx(outlineRect, 3.0f, ORANGE);
                }
                
                // Don't draw white background - keep transparent
                
                // Draw entity representation
                if (entityID == 0) {
                    // Eraser - draw X
                    DrawLine((int)x + 4, (int)y + 4, (int)(x + itemSize - 4), (int)(y + itemSize - 4), RED);
                    DrawLine((int)(x + itemSize - 4), (int)y + 4, (int)x + 4, (int)(y + itemSize - 4), RED);
                } else if (entityID >= 1 && entityID <= 87) {
                    // Tiles - try to draw texture or colored square
                    std::string tileKey = "tile_" + std::to_string(entityID);
                    if (textures.find(tileKey) != textures.end()) {
                        DrawTexturePro(textures[tileKey], 
                                     {0, 0, (float)textures[tileKey].width, (float)textures[tileKey].height},
                                     itemRect, {0, 0}, 0.0f, WHITE);
                    } else {
                        // Draw colored square without white background
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
                        // Draw colored square without white background
                        DrawRectangleRec(itemRect, BROWN);
                    }
                } else if (entityID >= 103 && entityID <= 120) {
                    // Items - draw colored square without white background
                    DrawRectangleRec(itemRect, GOLD);
                } else if (entityID >= 121 && entityID <= 158) {
                    // Baddies - draw colored square without white background
                    DrawRectangleRec(itemRect, RED);
                }
                
                // Draw thin border for better visibility (for all items)
                DrawRectangleLinesEx(itemRect, 1.0f, BLACK);
            }
            
            // Update currentY for next category
            int rows = ((int)categoryEntityIDs[catIndex].size() + itemsPerRow - 1) / itemsPerRow;
            currentY += rows * (itemSize + itemSpacing) + 10.0f;
        }
        
        currentY += 5.0f;
    }
}

void MapEditorScreen2::handleToolSelection(Vector2 mousePos) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
    
    float toolsAreaWidth = GetScreenWidth() * 0.2f;
    float toolsAreaX = GetScreenWidth() - toolsAreaWidth;
    
    float currentY = 70.0f;
    
    // Skip eraser size area if eraser is selected
    if (selectedEntityID == 0) {
        currentY += 60.0f; // Skip the eraser size area
    }
    float categoryHeight = 30.0f;
    float itemSize = 32.0f;
    float itemSpacing = 4.0f;
    
    // Check category headers
    for (int catIndex = 0; catIndex < categories.size(); catIndex++) {
        Rectangle categoryRect = {toolsAreaX + 5.0f, currentY, toolsAreaWidth - 10.0f, categoryHeight};
        
        if (CheckCollisionPointRec(mousePos, categoryRect)) {
            selectedCategoryIndex = catIndex;
            return;
        }
        
        currentY += categoryHeight + 5.0f;
        
        // If this is the selected category, check for item clicks
        if (catIndex == selectedCategoryIndex) {
            float itemX = toolsAreaX + 10.0f;
            float itemY = currentY;
            int itemsPerRow = (int)((toolsAreaWidth - 20.0f) / (itemSize + itemSpacing));
            
            for (int i = 0; i < categoryEntityIDs[catIndex].size(); i++) {
                int entityID = categoryEntityIDs[catIndex][i];
                
                int row = i / itemsPerRow;
                int col = i % itemsPerRow;
                float x = itemX + col * (itemSize + itemSpacing);
                float y = itemY + row * (itemSize + itemSpacing);
                
                if (y + itemSize > GetScreenHeight() - 20.0f) break;
                
                Rectangle itemRect = {x, y, itemSize, itemSize};
                
                if (CheckCollisionPointRec(mousePos, itemRect)) {
                    selectedEntityID = entityID;
                    return;
                }
            }
            
            int rows = ((int)categoryEntityIDs[catIndex].size() + itemsPerRow - 1) / itemsPerRow;
            currentY += rows * (itemSize + itemSpacing) + 10.0f;
        }
        
        currentY += 5.0f;
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
    
    // Only draw while holding left mouse button
    if (!isDrawing) return;
    
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
        } else {
            // Normal single tile placement
            int index = gridY * GRID_WIDTH_IN_TILES + gridX;
            if (index >= 0 && index < currentMapData->entitiesID.size()) {
                currentMapData->entitiesID[index] = selectedEntityID;
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

