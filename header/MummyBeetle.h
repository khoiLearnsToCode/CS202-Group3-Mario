#pragma once

#include "Baddie.h"
#include "raylib.h"

class MummyBeetle : public Baddie {
    
public:

    MummyBeetle( Vector2 pos, Vector2 dim, Vector2 vel, Color color );
    ~MummyBeetle() override;

    void update() override;
    void draw() override;

};