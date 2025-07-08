#pragma once

#include "Sprite.h"
#include "raylib.h"
#include <string>

// Lớp cơ sở trừu tượng cho Baddies
class Baddie : public Sprite {
public:
    Baddie(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float frameTime, int maxFrames, Direction facingDirection, int hitsToDie, int earnedPoints)
        : Sprite(pos, dim, vel, color, frameTime, maxFrames, facingDirection, hitsToDie, earnedPoints), animationTime(0.0f) {
    }
    virtual ~Baddie() = default;

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual CollisionType checkCollision(Sprite* sprite) override = 0;
    virtual std::string getType() const = 0;
    virtual void onHit() { if (--hitsToDie <= 0) setState(SPRITE_STATE_DYING); }
    virtual void setAttributesOnDying() { vel = { 0, 0 }; animationTime = 0.0f; }

protected:
    float animationTime; // Thời gian tích lũy cho trạng thái DYING
};

// Lớp Goomba
class Goomba : public Baddie {
public:
    Goomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "Goomba"; }
};

// Lớp FlyingGoomba
class FlyingGoomba : public Baddie {
public:
    FlyingGoomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "FlyingGoomba"; }

private:
    float flyAmplitude; // Biên độ bay lên xuống
};

// Lớp RedKoopaTroopa
class RedKoopaTroopa : public Baddie {
public:
    RedKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "RedKoopaTroopa"; }

private:
    bool isInShell; // Trạng thái rút vào mai
};

// Lớp GreenKoopaTroopa
class GreenKoopaTroopa : public Baddie {
public:
    GreenKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "GreenKoopaTroopa"; }

private:
    bool isInShell; // Trạng thái rút vào mai
};

// Lớp BlueKoopaTroopa
class BlueKoopaTroopa : public Baddie {
public:
    BlueKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "BlueKoopaTroopa"; }

private:
    bool isInShell; // Trạng thái rút vào mai
};

// Lớp YellowKoopaTroopa
class YellowKoopaTroopa : public Baddie {
public:
    YellowKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "YellowKoopaTroopa"; }

private:
    bool isInShell; // Trạng thái rút vào mai
};

// Lớp BuzzyBeetle
class BuzzyBeetle : public Baddie {
public:
    BuzzyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "BuzzyBeetle"; }

private:
    bool isInShell; // Trạng thái rút vào mai
};

// Lớp MummyBeetle
class MummyBeetle : public Baddie {
public:
    MummyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "MummyBeetle"; }

private:
    bool isInShell; // Trạng thái rút vào mai
};

// Lớp BobOmb
class BobOmb : public Baddie {
public:
    BobOmb(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "BobOmb"; }

private:
    float explosionTimer; // Thời gian trước khi nổ
};

// Lớp PiranhaPlant
class PiranhaPlant : public Baddie {
public:
    PiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "PiranhaPlant"; }

private:
    float emergeTimer; // Thời gian xuất hiện/ẩn trong ống
};

// Lớp JumpingPiranhaPlant
class JumpingPiranhaPlant : public Baddie {
public:
    JumpingPiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "JumpingPiranhaPlant"; }

private:
    float jumpTimer; // Thời gian giữa các lần nhảy
};

// Lớp BulletBill
class BulletBill : public Baddie {
public:
    BulletBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "BulletBill"; }
};

// Lớp BanzaiBill
class BanzaiBill : public Baddie {
public:
    BanzaiBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "BanzaiBill"; }
};

// Lớp MontyMole
class MontyMole : public Baddie {
public:
    MontyMole(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "MontyMole"; }

private:
    float emergeTimer; // Thời gian xuất hiện từ dưới đất
};

// Lớp Rex
class Rex : public Baddie {
public:
    Rex(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "Rex"; }
};

// Lớp Muncher
class Muncher : public Baddie {
public:
    Muncher(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "Muncher"; }

private:
    bool isInvulnerable; // Không thể bị tiêu diệt
};

// Lớp Swooper
class Swooper : public Baddie {
public:
    Swooper(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    void update() override;
    void draw() override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override { return "Swooper"; }

private:
    float swoopTimer; // Thời gian lao xuống
};

// Factory Pattern để tạo Baddies
class BaddieFactory {
public:
    static Baddie* createBaddie(const std::string& type, Vector2 pos, Vector2 vel);
};

