#pragma once

#include "Baddie.h"
#include "raylib.h"

class GreenKoopaTroopa : public Baddie {
    
public:

    GreenKoopaTroopa( Vector2 pos, Vector2 dim, Vector2 vel, Color color );
    ~GreenKoopaTroopa() override;

    void update() override;
    void draw() override;

};