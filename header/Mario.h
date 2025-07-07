#pragma once

class GameWorld;
class Map;

#include "CollisionProbe.h"
#include "CollisionType.h"
#include "Direction.h"
#include "Fireball.h"
#include "MarioType.h"
#include "raylib.h"
#include "Sprite.h"
#include <vector>

class Mario : public Sprite {

	float speedX; // Base horizontal speed (walking speed)
	float maxSpeedX; // Maximum horizontal speed (running speed)
	float jumpSpeed; // Initial upward speed when jumping
	float dyingVelY; // Vertical speed when dying
	
	bool immortal; 
	bool invulnerable; // Temporarily invulnerable after being hit
	float invulnerableTime; // Time in seconds for invulnerability
	float invulnerableTimeAcum; // Accumulator for invulnerability time
	bool invulnerableBlink; // Whether the sprite is blinking during invulnerability (in rendering)

	bool ducking; // Whether the player is ducking, is 'true' when holding DOWN
	bool lookingUp; // Whether the player is looking up, is 'true' when holding UP
	bool running; // Whether the player is running, is 'true' when holding CTRL
	bool invincible; // Whether the player is invincible, is 'true' when Mario has Star power

	float frameTimeWalking; // Time between frames when walking
	float frameTimeRunning; // Time between frames when running (faster animation)

	float activationWidth; // Width of an activation area (for interacting with objects)

	int lives;
	int coins;
	int yoshiCoins;
	int points;
	float maxTime;
	float ellapsedTime;

	MarioType type; // Current Mario form
	
	CollisionProbe cpE1; // Right-side collision detector
	CollisionProbe cpW1; // Left-side collision detector

	MarioType reservedPowerUp; // Power-Up stored in reserve

	std::vector<Fireball> fireballs;

	float runningAcum;
	float runningTime;
	bool drawRunningFrames; // Whether to draw running frames (faster) or not

	float movingAcum; // Accumulator for moving frames

	float transitioningFrameTime; // Time between frames during transformation
	float transitioningFrameAcum; // Time passed sinced last transition frame
	int transitionSteps; // Number of frames in normal transition (SMALL to SUPER)
	int superToFlowerTransitionSteps; // Number of frames in SUPER to FLOWER transition
	int transitionCurrentFrame; // Current displayed frame in a transition
	int transitionCurrentFramePos; // Position in the transition sequence
	const int transitionFrameOrder[11] = { 0, 1, 0, 1, 0, 1, 2, 1, 2, 1, 2 }; // SMALL -> SUPER
	const int reverseTransitionFrameOrder[11] = { 2, 1, 2, 1, 2, 1, 0, 1, 0, 1, 0 }; // SUPER -> SMALL
	const int superToFlowerTransitionFrameOrder[11] = { 0, 1, 0, 1, 0, 1, 0, 1 }; // SUPER -> FLOWER

	float invincibleTime; // Duration of invicibility
	float invincibleAcum; // Accumulator to track how long Mario has been invincible

	bool playerDownMusicStreamPlaying;
	bool gameOverMusicStreamPlaying;

	Vector2 lastPos; // Position in the previous frame (used to detect falling)
	SpriteState lastStateBeforeTransition; // To resume the correct state after a power-up transition
	GameWorld* gw; 
	Map* map;

public:

    Mario(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed, bool immortal);
    ~Mario() override;

    void update() override;
    void draw() override;
    void updateCollisionProbes() override;
    void drawHud() const;

    CollisionType checkCollision(Sprite* sprite) override;
    CollisionType checkCollisionBaddie(Sprite* sprite);

    void setImmortal(bool immortal);
    void setActivationWidth(float activationWidth);

    float getSpeedX() const;
    float getMaxSpeedX() const;
    float getJumpSpeed() const;
    bool isImmortal() const;
    float getActivationWidth() const;

    void setLives(int lives);
    void setCoins(int coins);
    void setYoshiCoins(int yoshiCoins);
    void setPoints(int points);
    void setMaxTime(float maxTime);
    void setLastStateBeforeTransition(SpriteState lastStateBeforeTransition);

    void setGameWorld(GameWorld* gw);
    void setMap(Map* map);

    GameWorld* getGameWorld() const;
    Map* getMap() const;

    int getLives() const;
    int getCoins() const;
    int getYoshiCoins() const;
    int getPoints() const;
    int getRemainingTime() const;

    void addLives(int lives);
    void removeLives(int lives);
    void addCoins(int coins);
    void addYoshiCoins(int yoshiCoins);
    void removeCoins(int coins);
    void addPoints(int points);
    void removePoints(int points);

    void changeToSmall();
    void changeToSuper();
    void changeToFlower();

    void setReservedPowerUp(MarioType reservedPowerUp);
    MarioType getReservedPowerUp() const;
	/*void releaseReservedPowerUp();*/ // Needs Item class implementation

    MarioType getType() const;
    void setInvulnerable(bool invulnerable);
    bool isInvulnerable() const;

    void setInvincible(bool invincible);
    bool isInvincible() const;

    bool isTransitioning() const;

    void reset(bool removePowerUps);
    void resetAll();

    void playPlayerDownMusicStream();
    void playGameOverMusicStream();
    bool isPlayerDownMusicStreamPlaying() const;
    bool isGameOverMusicStreamPlaying() const;

    Vector2 getSouthCollisionProbePos() const;

};