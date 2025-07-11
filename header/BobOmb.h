#pragma once

#include "Baddie.h"
#include "raylib.h"

class BobOmb : public Baddie {
    
public:

    BobOmb( Vector2 pos, Vector2 dim, Vector2 vel, Color color );
    ~BobOmb() override;

    void update() override;
    void draw() override;

};