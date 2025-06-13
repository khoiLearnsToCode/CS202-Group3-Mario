#pragma once

#include "raylib.h"
#include "Sprite.h"
#include "TileType.h"
#include <string>

class Tile : public Sprite {

protected:
    std::string key; // the tile's picture name 
    bool visible;
    bool onlyBaddies;
    //bool showCollisionOnDebug;
    TileType type;


public:

    Tile(Vector2 pos, Vector2 dim, Color color, std::string key, bool visible);
    Tile(Vector2 pos, Vector2 dim, Color color, std::string key, bool visible, TileType type);
    ~Tile() override;

    void update() override;
    void draw() override;

    bool isOnlyBaddies() const;
    TileType getType() const;

};