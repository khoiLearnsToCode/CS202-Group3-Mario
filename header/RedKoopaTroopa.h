#pragma once

#include "Baddie.h"
#include "raylib.h"

class RedKoopaTroopa : public Baddie {
    
public:

    RedKoopaTroopa( Vector2 pos, Vector2 dim, Vector2 vel, Color color );
    ~RedKoopaTroopa() override;

    void update() override;
    void draw() override;

};