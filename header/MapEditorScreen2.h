#pragma once

#include "Screen.h"
#include "Button.h"
#include "Data.h"
#include "ResourceManager.h"
#include "MapEditorState.h"

class MapEditorScreen1; 

class MapEditorScreen2 : public Screen {
private:
    UserMapData* currentMapData;

    friend class MapEditorScreen1; 
    void setCurrentMapData(UserMapData* mapData);

    MapEditorState MainState;
    MapEditorState AuxiliaryState;
    
    // Camera/view parameters for zoom and pan
    Camera2D gridCamera;
    Vector2 lastMousePosition;
    bool isDragging;


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
    
public:
};