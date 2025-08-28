#pragma once

#include "Baddie.h"
#include "raylib.h"

class Goomba : public Baddie {
    
public:

    Goomba( Vector2 pos, Vector2 dim, Vector2 vel, Color color );
    ~Goomba() override;

    void update() override;
    void draw() override;

};