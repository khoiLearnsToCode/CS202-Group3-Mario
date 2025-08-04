#include "MapEditorScreen1.h"
#include "MapEditorScreen2.h"
#include "GameWorld.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>

MapEditorScreen2::MapEditorScreen2(MapEditorScreen1* parentScreen)
    : Screen(), currentMapData(nullptr), MainState(MAP_EDITOR_STATE_IDLE),
                                    AuxiliaryState(MAP_EDITOR_STATE_TILE), isDragging(false) {

        // Initialize camera for grid view
        gridCamera.target = {0.0f, 0.0f};
        gridCamera.offset = {200.0f, 200.0f}; // Start with some offset
        gridCamera.rotation = 0.0f;
        gridCamera.zoom = 1.0f;
        
        lastMousePosition = {0.0f, 0.0f};

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
    
    // Handle zoom with mouse wheel (only when mouse is in grid area)
    float toolsAreaWidth = GetScreenWidth() * 0.2f;
    if (mousePosition.x < GetScreenWidth() - toolsAreaWidth) {
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
        
        // Handle panning with right mouse button
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            lastMousePosition = mousePosition;
            isDragging = true;
        }
        
        if (isDragging && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = Vector2Subtract(mousePosition, lastMousePosition);
            gridCamera.target = Vector2Subtract(gridCamera.target, Vector2Scale(delta, 1.0f / gridCamera.zoom));
            lastMousePosition = mousePosition;
        }
        
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            isDragging = false;
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
    
    // Draw instructions
    const char* instructions = "ESC: Return to menu | Mouse Wheel: Zoom | Right Click + Drag: Pan | Left Side: 200x60 Tile Grid";
    Vector2 instructSize = MeasureTextEx(font, instructions, 20.0f, 0.0f);
    DrawTextEx(font, instructions, 
               {20.0f, GetScreenHeight() - 40.0f}, 
               20.0f, 0.0f, DARKGRAY);
}

void MapEditorScreen2::drawGrid() {
    // Grid parameters - 200x60 TILES
    const int gridWidthInTiles = 200;
    const int gridHeightInTiles = 60;
    const float baseTileSize = 32.0f; // Base tile size in pixels
    
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
    float actualTileSize = baseTileSize * gridCamera.zoom;
    
    // Calculate grid bounds in world space
    float gridWorldWidth = gridWidthInTiles * baseTileSize;
    float gridWorldHeight = gridHeightInTiles * baseTileSize;
    
    // Draw grid background (behind tiles)
    DrawRectangle(0, 0, (int)gridWorldWidth, (int)gridWorldHeight, Fade(LIGHTGRAY, 0.3f));
    DrawRectangleLines(0, 0, (int)gridWorldWidth, (int)gridWorldHeight, BLACK);
    
    // Draw grid lines (visible when zoomed in enough)
    if (actualTileSize >= 8.0f) {
        // Draw major grid lines (every 10 tiles)
        for (int x = 0; x <= gridWidthInTiles; x += 10) {
            float lineX = x * baseTileSize;
            DrawLine((int)lineX, 0, (int)lineX, (int)gridWorldHeight, GRAY);
        }
        
        for (int y = 0; y <= gridHeightInTiles; y += 10) {
            float lineY = y * baseTileSize;
            DrawLine(0, (int)lineY, (int)gridWorldWidth, (int)lineY, GRAY);
        }
        
        // Draw minor grid lines (every tile) when zoomed in enough
        if (actualTileSize >= 16.0f) {
            for (int x = 0; x <= gridWidthInTiles; x++) {
                float lineX = x * baseTileSize;
                DrawLine((int)lineX, 0, (int)lineX, (int)gridWorldHeight, Fade(GRAY, 0.5f));
            }
            
            for (int y = 0; y <= gridHeightInTiles; y++) {
                float lineY = y * baseTileSize;
                DrawLine(0, (int)lineY, (int)gridWorldWidth, (int)lineY, Fade(GRAY, 0.5f));
            }
        }
    }
    
    // Draw individual tiles with entity data if available
    if (currentMapData && currentMapData->entitiesID.size() == 12000) {
        for (int y = 0; y < gridHeightInTiles; y++) {
            for (int x = 0; x < gridWidthInTiles; x++) {
                int index = y * gridWidthInTiles + x;
                if (index < currentMapData->entitiesID.size()) {
                    int entityID = currentMapData->entitiesID[index];
                    
                    // Draw different colors based on entity ID
                    if (entityID != 0) {
                        float tileX = x * baseTileSize;
                        float tileY = y * baseTileSize;
                        
                        Color tileColor = GREEN; // Default color for non-empty tiles
                        if (entityID == 1) tileColor = BROWN;      // Ground/blocks
                        else if (entityID == 2) tileColor = BLUE; // Water
                        else if (entityID == 3) tileColor = RED;  // Enemies
                        else if (entityID == 4) tileColor = YELLOW; // Items
                        
                        Rectangle tileRect = {tileX, tileY, baseTileSize, baseTileSize};
                        DrawRectangleRec(tileRect, tileColor);
                        
                        // Draw tile borders when zoomed in enough
                        if (actualTileSize >= 12.0f) {
                            DrawRectangleLinesEx(tileRect, 1.0f, BLACK);
                        }
                    }
                }
            }
        }
    }
    
    EndMode2D();
    EndScissorMode();
    
    // Draw tools area on the right side (20% of screen width x full screen height)
    float toolsAreaX = GetScreenWidth() - toolsAreaWidth;
    float toolsAreaY = 0.0f;
    float toolsAreaHeight = GetScreenHeight();
    
    Rectangle toolsArea = {toolsAreaX, toolsAreaY, toolsAreaWidth, toolsAreaHeight};
    DrawRectangleRec(toolsArea, Fade(DARKGRAY, 0.1f)); // Light background
    DrawRectangleLinesEx(toolsArea, 2.0f, DARKGRAY);
    
    Font font = ResourceManager::getInstance().getFont("SuperMario256");
    const char* toolsText = "TOOLS AREA\n(Reserved for\nfuture implementation)";
    Vector2 toolsTextSize = MeasureTextEx(font, toolsText, 24.0f, 0.0f);
    Vector2 toolsTextPos = {
        toolsArea.x + (toolsArea.width - toolsTextSize.x) / 2.0f,
        toolsArea.y + (toolsArea.height - toolsTextSize.y) / 2.0f
    };
    DrawTextEx(font, toolsText, toolsTextPos, 24.0f, 0.0f, DARKGRAY);
    
    // Draw grid info and controls
    const char* gridInfo = TextFormat("Grid: %dx%d tiles | Zoom: %.1fx | Mouse Wheel: Zoom | Right Click: Pan", 
                                     gridWidthInTiles, gridHeightInTiles, gridCamera.zoom);
    DrawTextEx(font, gridInfo, 
               {20.0f, GetScreenHeight() - 80.0f}, 
               18.0f, 0.0f, BLACK);
}
