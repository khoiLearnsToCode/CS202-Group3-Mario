#pragma once

#include "Baddie.h"
#include "raylib.h"

class Swooper : public Baddie {
    
public:

    Swooper( Vector2 pos, Vector2 dim, Vector2 vel, Color color );
    ~Swooper() override;

    void update() override;
    void draw() override;

};