#pragma once
#include "raylib.h"
#include "Sprite.h"
#include "Player.h"
#include <string>

// Base class for all items in the game
class Item : public Sprite {
protected:
    float onHitFrameAcum;
    float onHitFrameTime;
    int maxOnHitFrame;
    int currentOnHitFrame;
    float pointsFrameAcum;
    float pointsFrameTime;
    bool pauseGameOnHit;

public:

    Item();
    Item(Vector2 pos, Vector2 dim, Color color, int earnedPoints);
    Item(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames, int earnedPoints);
    Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, int earnedPoints);
    Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float frameTime, int maxFrames);
    Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float frameTime, int maxFrames, int earnedPoints);
    ~Item() override;

    void update() override = 0;
    void draw() override = 0;
    virtual void playCollisionSound() = 0;
    virtual void updatePlayer(Player& player) = 0;
    virtual void onSouthCollision(Player& player);
    virtual bool isPauseGameOnHit();
	virtual std::string getType() const = 0;
};

class Coin : public Item {
public:
    Coin(Vector2 pos, Vector2 dim, Color color);
    ~Coin() override;

    void update() override;
    void draw() override;
    void playCollisionSound() override;
    void updatePlayer(Player& player) override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override {
        return "Coin";
    }
};

class Mushroom : public Item {
	bool applyGravity; // whether the mushroom should fall due to gravity
	bool doCollisionOnGround; // whether the mushroom should collide with the ground
    bool blinking;

    float blinkingAcum;
    float blinkingTime;
    bool doBlink;
public:
    Mushroom(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    Mushroom(Vector2 pos, Vector2 dim, Vector2 vel, Color color, bool applyGravity, bool doCollisionOnGround, bool blinking);
    ~Mushroom() override;

    void update() override;
    void draw() override;
    void playCollisionSound() override;
    void updatePlayer(Player& player) override;
    void onSouthCollision(Player& player) override;
	std::string getType() const override {
		return "Mushroom";
	}
};

class OneUpMushroom : public Item {
public:

    OneUpMushroom(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~OneUpMushroom() override;

    void update() override;
    void draw() override;
    void playCollisionSound() override;
    void updatePlayer(Player& player) override;
	std::string getType() const override {
		return "OneUpMushroom";
	}
};

class FireFlower : public Item {
	bool doCollisionOnGround; // whether the fire flower should collide with the ground
    bool blinking;

    float blinkingAcum;
    float blinkingTime;
    bool doBlink;

public:

    FireFlower(Vector2 pos, Vector2 dim, Color color);
    FireFlower(Vector2 pos, Vector2 dim, Vector2 vel, Color color, bool doCollisionOnGround, bool blinking);
    ~FireFlower() override;

    void update() override;
    void draw() override;
    void playCollisionSound() override;
    void updatePlayer(Player& player) override;
    void onSouthCollision(Player& player) override;
	std::string getType() const override {
		return "FireFlower";
	}
};

class Star : public Item {
public:
    Star(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~Star() override;

    void update() override;
    void draw() override;
    void playCollisionSound() override;
    void updatePlayer(Player& player) override;
    void onSouthCollision(Player& player) override;
	std::string getType() const override {
		return "Star";
	}
};

class ThreeUpMoon : public Item {
public:

    ThreeUpMoon(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~ThreeUpMoon() override;

    void update() override;
    void draw() override;
    void playCollisionSound() override;
    void updatePlayer(Player& player) override;
	std::string getType() const override {
		return "ThreeUpMoon";
	}
};

class YoshiCoin : public Item {

    bool countingUp;

public:

    YoshiCoin(Vector2 pos, Vector2 dim, Color color);
    ~YoshiCoin() override;

    void update() override;
    void draw() override;
    void playCollisionSound() override;
    void updatePlayer(Player& player) override;
    CollisionType checkCollision(Sprite* sprite) override;
    std::string getType() const override {
        return "YoshiCoin";
    }
};

class CourseClearToken : public Item {
public:
	CourseClearToken(Vector2 pos, Vector2 dim, Color color);
	~CourseClearToken() override;

	void update() override;
	void draw() override;
	void playCollisionSound() override;
	void updatePlayer(Player& player) override;
	CollisionType checkCollision(Sprite* sprite) override;
	std::string getType() const override {
		return "CourseClearToken";
	}
};

class FactoryItem {
public:
	static Item* createItem(const std::string& itemType, Vector2 pos, Vector2 dim, Vector2 vel = { 0, 0 }, Color color = WHITE, bool applyGravity = false, bool doCollisionOnGround = true, bool blinking = false);
};