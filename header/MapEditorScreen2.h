#pragma once

#include "Screen.h"
#include "Button.h"
#include "Data.h"
#include "ResourceManager.h"
#include "MapEditorState.h"

class MapEditorScreen1; 

class MapEditorScreen2 : public Screen {
private:
    // Static constants
    static const int GRID_WIDTH_IN_TILES;
    static const int GRID_HEIGHT_IN_TILES;
    static const float BASE_TILE_SIZE;
    static const int MIN_ERASER_SIZE;
    static const int MAX_ERASER_SIZE; 
    static const int MIN_BRUSH_SIZE;
    static const int MAX_BRUSH_SIZE;
    
    UserMapData* currentMapData;
    Font& font1;
    Font& font2;

    friend class MapEditorScreen1; 
    void setCurrentMapData(UserMapData* mapData);

    MapEditorState MainState;
    MapEditorState AuxiliaryState;
    
    // Camera/view parameters for zoom and pan
    Camera2D gridCamera;
    Vector2 lastMousePosition;
    bool isDragging;
    
    // Tool selection
    int selectedEntityID;
    int lastSelectedEntityID; // Store the last selected entity for brush mode
    int selectedCategoryIndex;
    std::vector<std::string> categories;
    std::vector<std::vector<int>> categoryEntityIDs;
    Vector2 toolsScrollOffset;
    
    // Hover state
    Vector2 hoveredGridPos;
    bool isHoveringGrid;
    
    // Drawing state
    bool isDrawing;
    
    // Eraser settings
    int eraserSize;
    
    // Brush settings
    int brushSize;
    
    // Color picker settings
    float colorPickerRed;
    float colorPickerGreen;
    float colorPickerBlue;
    float colorPickerAlpha;

    const std::vector<Color> colorPalette;


public:
    MapEditorScreen2(MapEditorScreen1* parentScreen);
    ~MapEditorScreen2() override = default;

    MapEditorScreen2(const MapEditorScreen2&) = delete;
    MapEditorScreen2& operator=(const MapEditorScreen2&) = delete;

    void draw() override;
    void update();
    
    // Additional methods for map editing functionality can be added here
private:
    void drawGrid();
    void drawToolsArea();
    void drawPermanentToolsSection(float toolsAreaX, float toolsAreaWidth, float toolsAreaHeight);
    void initializeCategories();
    void handleToolSelection(Vector2 mousePos);
    void handleGridPlacement(Vector2 mousePos);
    void updateHoverState(Vector2 mousePos);
    void drawEraserSizeArea(float toolsAreaX, float toolsAreaWidth, float& currentY);
    void drawBrushSizeArea(float toolsAreaX, float toolsAreaWidth, float& currentY);
    void drawColorPicker(float toolsAreaX, float toolsAreaWidth, float& currentY);
    void drawMinimap(float toolsAreaX, float toolsAreaWidth, float& currentY);
    
public:
};