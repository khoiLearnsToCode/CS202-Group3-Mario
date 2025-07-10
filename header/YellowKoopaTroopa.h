#pragma once

#include "Baddie.h"
#include "raylib.h"

class YellowKoopaTroopa : public Baddie {
    
public:

    YellowKoopaTroopa( Vector2 pos, Vector2 dim, Vector2 vel, Color color );
    ~YellowKoopaTroopa() override;

    void update() override;
    void draw() override;

};