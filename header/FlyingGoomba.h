#pragma once

#include "Baddie.h"
#include "raylib.h"

class FlyingGoomba : public Baddie {
private:
    float moveDistance;      // Theo dõi khoảng cách di chuyển ngang
    float maxMoveDistance;   // Khoảng cách tối đa trước khi đảo ngược hướng
    float timeAcum;          // Thời gian tích lũy cho chuyển động hình sin
    float amplitude;         // Biên độ dao động (độ cao lên xuống)
    float frequency;         // Tần số dao động (tốc độ lên xuống)
    float initialY;          // Vị trí y ban đầu để làm mốc cho dao động
    float phaseOffset;       // Độ lệch pha để điều chỉnh sine wave
public:

    FlyingGoomba( Vector2 pos, Vector2 dim, Vector2 vel, Color color );
    ~FlyingGoomba() override;

    void update() override;
    void draw() override;
    void updateCollisionProbes() override;
    void onNorthCollision() override;
    void onSouthCollision() override;
    void onHit() override;

};