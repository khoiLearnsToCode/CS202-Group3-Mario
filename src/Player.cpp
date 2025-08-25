#include "Direction.h"
#include "GameState.h"
#include "GameWorld.h"
#include "Map.h"
#include "Player.h"
#include "raylib.h"
#include "ResourceManager.h"
#include <iostream>
#include <utils.h>

// #include "FireFlower.h"
// #include "Mushroom.h"

Player::Player( Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed, bool immortal ) :
	Sprite( pos, dim, vel, color, 0, 2),
	speedX( speedX ),
	maxSpeedX( maxSpeedX ),
	jumpSpeed( jumpSpeed ),
	dyingVelY( -600 ),
	immortal( immortal ),
	invulnerable( false ),
	ducking( false ),
	lookingUp( false ),
	running( false ),
	invincible( false ),
	frameTimeWalking( 0.1 ),
	frameTimeRunning( 0.5 ),
    activationWidth(0),
    lives(5),
    coins(0),
    coinsFromPreviousMap(0),
    yoshiCoins(0),
    points(0),
    pointsFromPreviousMap(0),
    maxTime(400.0f),  // Set default time limit to 400 seconds (typical Mario game time)
    ellapsedTime(0.0f),
    type(PLAYER_TYPE_SMALL),
    reservedPowerUp(PLAYER_TYPE_SMALL),
    runningAcum(0),
    runningTime(0.5),
    drawRunningFrames(false),
    movingAcum(0),

    transitioningFrameTime(0.06),
    transitioningFrameAcum(0),
    transitionSteps(11),
    superToFlowerTransitionSteps(8),
    transitionCurrentFrame(0),
    transitionCurrentFramePos(0),

    invincibleTime(8),
    invincibleAcum(0),
    playerDownMusicStreamPlaying(false),
    gameOverMusicStreamPlaying(false),
    lastPos(pos),
    lastStateBeforeTransition(SPRITE_STATE_ACTIVE),
    gw(nullptr),
    map(nullptr) {

    setState(SPRITE_STATE_ON_GROUND);

    cpN.setColor(PINK);
    cpS.setColor(VIOLET);
    cpE.setColor(YELLOW);
    cpE1.setColor(YELLOW);
    cpW.setColor(LIME);
    cpW1.setColor(LIME);

    isLuigi = false;

}

Player::~Player() = default;

void Player::update() {
    const float delta = GetFrameTime();
    float accelRate = isLuigi ? 0.6f : 1.0f; // Luigi accelerates slower than Mario

    // The character is running if Left Ctrl or Right Ctrl is hold and he is moving (velocity is non-zero)
    running = ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && vel.x != 0.0f);

    if (running) {
        runningAcum += delta * accelRate;
        if (runningAcum >= runningTime) {
            drawRunningFrames = true;
        }
    } else {
        runningAcum = 0;
        drawRunningFrames = false;
    }

    if (invincible) {
        invincibleAcum += delta;
        if (invincibleAcum >= invincibleTime) {
            invincible = false;
            invincibleAcum = 0;
        }
    }

    if (state != SPRITE_STATE_DYING && state != SPRITE_STATE_VICTORY && state != SPRITE_STATE_WAITING_TO_NEXT_MAP) {
        ellapsedTime += delta;
    }

    // If walking: speedX, if running but not in full animation: maxSpeedX, if running in full animation: maxSpeedX * 1.3f
    const float currentSpeedX = running ? (drawRunningFrames ? maxSpeedX * 1.1f : maxSpeedX) : speedX;

    const float currentFrameTime = running && state != SPRITE_STATE_DYING ? frameTimeRunning : frameTimeWalking;

	std::map<std::string, Sound>& sounds = ResourceManager::getInstance().getSounds(); 

    if (ellapsedTime >= maxTime &&
        state != SPRITE_STATE_DYING &&
        state != SPRITE_STATE_VICTORY &&
        state != SPRITE_STATE_WAITING_TO_NEXT_MAP) {
        
        // Add elapsed time to total played time before the character dies from timeout
        if (gw != nullptr) {
            gw->addToTotalPlayedTime(ellapsedTime);
        }
        
        state = SPRITE_STATE_DYING;
        playPlayerDownMusicStream();
		removeLives(1);
		changeToSmall();
        GameWorld::state = GAME_STATE_TIME_UP;
    }

    if (vel.x != 0 || state == SPRITE_STATE_DYING) {
		frameAcum += delta;
        if (frameAcum >= currentFrameTime) {
            frameAcum = 0;
            currentFrame++;
            currentFrame %= maxFrames;
        };
    }
    else {
        currentFrame = 0;
    }

    if (state == SPRITE_STATE_DYING) {
		pos.y += dyingVelY * delta;
		updateCollisionProbes();
        dyingVelY += GameWorld::gravity * delta;
	}

    if (invulnerable) {
        invulnerableTimeAcum += delta;
		invulnerableBlink = !invulnerableBlink; // Toggle blink state
        if (invulnerableTimeAcum >= invulnerableTime) {
            invulnerableTimeAcum = 0;
            invulnerable = false;
            invulnerableBlink = false;
        }
    }

    if (state != SPRITE_STATE_DYING &&
        state != SPRITE_STATE_VICTORY &&
        state != SPRITE_STATE_WAITING_TO_NEXT_MAP) {

        if (state == SPRITE_STATE_TRANSITIONING_SMALL_TO_SUPER) {

            transitioningFrameAcum += delta;
            if (transitioningFrameAcum >= transitioningFrameTime) {
                transitioningFrameAcum = 0;
                transitionCurrentFramePos++;
                if (transitionCurrentFramePos <= transitionSteps) {
                    transitionCurrentFrame = transitionFrameOrder[transitionCurrentFramePos];
                }
                else {
                    transitionCurrentFramePos = 0;
                    state = lastStateBeforeTransition;
                    changeToSuper();
                    gw->unpauseGame();
                }
            }

        }
        else if (state == SPRITE_STATE_TRANSITIONING_SMALL_TO_FLOWER) {

            transitioningFrameAcum += delta;
            if (transitioningFrameAcum >= transitioningFrameTime) {
                transitioningFrameAcum = 0;
                transitionCurrentFramePos++;
                if (transitionCurrentFramePos <= transitionSteps) {
                    transitionCurrentFrame = transitionFrameOrder[transitionCurrentFramePos];
                }
                else {
                    transitionCurrentFramePos = 0;
                    state = lastStateBeforeTransition;
                    changeToFlower();
                    gw->unpauseGame();
                }
            }

        }
        else if (state == SPRITE_STATE_TRANSITIONING_SUPER_TO_FLOWER) {

            transitioningFrameAcum += delta;
            if (transitioningFrameAcum >= transitioningFrameTime) {
                transitioningFrameAcum = 0;
                transitionCurrentFramePos++;
                if (transitionCurrentFramePos <= superToFlowerTransitionSteps) {
                    transitionCurrentFrame = superToFlowerTransitionFrameOrder[transitionCurrentFramePos];
                }
                else {
                    transitionCurrentFramePos = 0;
                    state = lastStateBeforeTransition;
                    changeToFlower();
                    gw->unpauseGame();
                }
            }

        }
        else if (state == SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL ||
            state == SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL) {

            transitioningFrameAcum += delta;
            if (transitioningFrameAcum >= transitioningFrameTime) {
                transitioningFrameAcum = 0;
                transitionCurrentFramePos++;
                if (transitionCurrentFramePos <= transitionSteps) {
                    transitionCurrentFrame = reverseTransitionFrameOrder[transitionCurrentFramePos];
                }
                else {
                    transitionCurrentFramePos = 0;
                    state = lastStateBeforeTransition;
                    changeToSmall();
                    releaseReservedPowerUp();
                    gw->unpauseGame();
                }
            }

        }
        else {
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                facingDirection = DIRECTION_RIGHT;
                movingAcum += delta * accelRate * 2;
                vel.x = currentSpeedX * (movingAcum < 1 ? movingAcum : 1);
            }
            else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                facingDirection = DIRECTION_LEFT;
                movingAcum += delta * accelRate * 2;
                vel.x = -currentSpeedX * (movingAcum < 1 ? movingAcum : 1);
            }
            else {
                movingAcum = 0;
                if (vel.x >= -10 && vel.x <= 10) {
                    vel.x = 0;
                }
                else {
                    vel.x = vel.x * 0.9;
                }
            }

            if (state == SPRITE_STATE_ON_GROUND) {
                if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
                    ducking = true;
                    vel.x = 0;
                }
                else {
                    ducking = false;
                }
            }

            if (IsKeyPressed(KEY_SPACE)) {
                if (state == SPRITE_STATE_ON_GROUND) {
                    vel.y = jumpSpeed;
                    state = SPRITE_STATE_JUMPING;
                    PlaySound(sounds["jump"]);
                }
            }

            if ((IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)) && type == PLAYER_TYPE_FLOWER) {

                if (facingDirection == DIRECTION_RIGHT) {
                    fireballs.push_back(Fireball(Vector2{ pos.x + dim.x / 2, pos.y + dim.y / 2 - 3 }, Vector2{ 16, 16 }, Vector2{ 400, 100 }, RED, DIRECTION_RIGHT, 2));
                }
                else {
                    fireballs.push_back(Fireball(Vector2{ pos.x, pos.y + dim.y / 2 - 3 }, Vector2{ 16, 16 }, Vector2{ -400, 100 }, RED, DIRECTION_LEFT, 2));
                }
                PlaySound(sounds["fireball"]);

            }

            std::vector<int> collectedIndexes;
            for (size_t i = 0; i < fireballs.size(); i++) {
                fireballs[i].update();
                if (fireballs[i].getState() == SPRITE_STATE_TO_BE_REMOVED) {
                    collectedIndexes.push_back(i);
                }
            }
            for (int i = collectedIndexes.size() - 1; i >= 0; i--) {
                fireballs.erase(fireballs.begin() + collectedIndexes[i]);
            }

            pos.x = pos.x + vel.x * delta;
            pos.y = pos.y + vel.y * delta;

            vel.y += GameWorld::gravity * delta;

            if (static_cast<int>(lastPos.y) < static_cast<int>(pos.y)) {
                state = SPRITE_STATE_FALLING;
            }
            lastPos = pos;

        }

    }

}

void Player::draw() {

    std::map<std::string, Texture2D>& textures = ResourceManager::getInstance().getTextures();
    std::string prefix;

    switch (type) {
    default:
    case PLAYER_TYPE_SMALL:
        prefix = "small";
        break;
    case PLAYER_TYPE_SUPER:
        prefix = "super";
        break;
    case PLAYER_TYPE_FLOWER:
        prefix = "flower";
        break;
    }

    const char dir = facingDirection == DIRECTION_RIGHT ? 'R' : 'L';

    if (!isLuigi) {
        if (state == SPRITE_STATE_DYING) {
            DrawTexture(textures[std::string(TextFormat("smallMario%dDy", currentFrame))], pos.x, pos.y, WHITE);
        }
        else if (state == SPRITE_STATE_TRANSITIONING_SMALL_TO_SUPER ||
            state == SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL) {
            DrawTexture(textures[std::string(TextFormat("transitioningMarioSS%d%c", transitionCurrentFrame, dir))], pos.x, pos.y, WHITE);
        }
        else if (state == SPRITE_STATE_TRANSITIONING_SMALL_TO_FLOWER ||
            state == SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL) {
            DrawTexture(textures[std::string(TextFormat("transitioningMarioSF%d%c", transitionCurrentFrame, dir))], pos.x, pos.y, WHITE);
        }
        else if (state == SPRITE_STATE_TRANSITIONING_SUPER_TO_FLOWER) {
            if (transitionCurrentFrame == 0) {
                DrawTexture(textures[std::string(TextFormat("superMario0%c", dir))], pos.x, pos.y, WHITE);
            }
            else {
                DrawTexture(textures[std::string(TextFormat("flowerMario0%c", dir))], pos.x, pos.y, WHITE);
            }
        }
        else {

            Color tint = WHITE;

            if (invincible) {
                tint = ColorFromHSV(360 * (invincibleAcum / invincibleTime * 20), 0.3, 1);
            }

            if (!invulnerableBlink) { // Only draw if not blinking

                if (state == SPRITE_STATE_ON_GROUND) {

                    if (lookingUp) {
                        DrawTexture(textures[std::string(TextFormat("%sMario0Lu%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                    }
                    else if (ducking) {
                        DrawTexture(textures[std::string(TextFormat("%sMario0Du%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                    }
                    else if (drawRunningFrames) {
                        DrawTexture(textures[std::string(TextFormat("%sMario%dRu%c", prefix.c_str(), currentFrame, dir))], pos.x, pos.y, tint);
                    }
                    else { // idle
                        if (IsKeyPressed(KEY_LEFT_CONTROL) && type == PLAYER_TYPE_FLOWER) {
                            DrawTexture(textures[std::string(TextFormat("%sMario%dTf%c", prefix.c_str(), currentFrame, dir))], pos.x, pos.y, tint);
                        }
                        else {
                            DrawTexture(textures[std::string(TextFormat("%sMario%d%c", prefix.c_str(), currentFrame, dir))], pos.x, pos.y, tint);
                        }
                    }

                }
                else if (state == SPRITE_STATE_JUMPING) {
                    if (drawRunningFrames) {
                        DrawTexture(textures[std::string(TextFormat("%sMario0JuRu%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                    }
                    else {
                        DrawTexture(textures[std::string(TextFormat("%sMario0Ju%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                    }
                }
                else if (state == SPRITE_STATE_FALLING) {
                    DrawTexture(textures[std::string(TextFormat("%sMario0Fa%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                }
                else if (state == SPRITE_STATE_VICTORY || state == SPRITE_STATE_WAITING_TO_NEXT_MAP) {
                    DrawTexture(textures[std::string(TextFormat("%sMario0Vic", prefix.c_str()))], pos.x, pos.y, tint);
                }

            }

            for (auto& fireball : fireballs) {
                fireball.draw();
            }

        }
    } else {
        if (state == SPRITE_STATE_DYING) {
            DrawTexture(textures[std::string(TextFormat("smallLuigi%dDy", currentFrame))], pos.x, pos.y, WHITE);
        }
        else if (state == SPRITE_STATE_TRANSITIONING_SMALL_TO_SUPER ||
            state == SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL) {
            DrawTexture(textures[std::string(TextFormat("transitioningLuigiSS%d%c", transitionCurrentFrame, dir))], pos.x, pos.y, WHITE);
        }
        else if (state == SPRITE_STATE_TRANSITIONING_SMALL_TO_FLOWER ||
            state == SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL) {
            DrawTexture(textures[std::string(TextFormat("transitioningLuigiSF%d%c", transitionCurrentFrame, dir))], pos.x, pos.y, WHITE);
        }
        else if (state == SPRITE_STATE_TRANSITIONING_SUPER_TO_FLOWER) {
            if (transitionCurrentFrame == 0) {
                DrawTexture(textures[std::string(TextFormat("superLuigi0%c", dir))], pos.x, pos.y, WHITE);
            }
            else {
                DrawTexture(textures[std::string(TextFormat("flowerLuigi0%c", dir))], pos.x, pos.y, WHITE);
            }
        }
        else {

            Color tint = WHITE;

            if (invincible) {
                tint = ColorFromHSV(360 * (invincibleAcum / invincibleTime * 20), 0.3, 1);
            }

            if (!invulnerableBlink) { // Only draw if not blinking

                if (state == SPRITE_STATE_ON_GROUND) {

                    if (lookingUp) {
                        DrawTexture(textures[std::string(TextFormat("%sLuigi0Lu%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                    }
                    else if (ducking) {
                        DrawTexture(textures[std::string(TextFormat("%sLuigi0Du%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                    }
                    else if (drawRunningFrames) {
                        DrawTexture(textures[std::string(TextFormat("%sLuigi%dRu%c", prefix.c_str(), currentFrame, dir))], pos.x, pos.y, tint);
                    }
                    else { // idle
                        if (IsKeyPressed(KEY_LEFT_CONTROL) && type == PLAYER_TYPE_FLOWER) {
                            DrawTexture(textures[std::string(TextFormat("%sLuigi%dTf%c", prefix.c_str(), currentFrame, dir))], pos.x, pos.y, tint);
                        }
                        else {
                            DrawTexture(textures[std::string(TextFormat("%sLuigi%d%c", prefix.c_str(), currentFrame, dir))], pos.x, pos.y, tint);
                        }
                    }

                }
                else if (state == SPRITE_STATE_JUMPING) {
                    if (drawRunningFrames) {
                        DrawTexture(textures[std::string(TextFormat("%sLuigi0JuRu%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                    }
                    else {
                        DrawTexture(textures[std::string(TextFormat("%sLuigi0Ju%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                    }
                }
                else if (state == SPRITE_STATE_FALLING) {
                    DrawTexture(textures[std::string(TextFormat("%sLuigi0Fa%c", prefix.c_str(), dir))], pos.x, pos.y, tint);
                }
                else if (state == SPRITE_STATE_VICTORY || state == SPRITE_STATE_WAITING_TO_NEXT_MAP) {
                    DrawTexture(textures[std::string(TextFormat("%sLuigi0Vic", prefix.c_str()))], pos.x, pos.y, tint);
                }

            }

            for (auto& fireball : fireballs) {
                fireball.draw();
            }

        }
    }
    if (playerDownMusicStreamPlaying) {
		playPlayerDownMusicStream();
    }
    else if (gameOverMusicStreamPlaying) {
		playGameOverMusicStream();
    }

}

CollisionType Player::checkCollision(Sprite* sprite) {

    if (sprite->getState() != SPRITE_STATE_NO_COLLIDABLE) {

        Rectangle rect = sprite->getRect();

        for (auto& fireball : fireballs) {

            Fireball* f = &fireball;

            switch (f->checkCollision(sprite)) {
            case COLLISION_TYPE_NORTH:
                f->setVelY(-f->getVelY());
                break;
            case COLLISION_TYPE_SOUTH:
                f->setVelY(-300);
                break;
            case COLLISION_TYPE_EAST:
                f->setState(SPRITE_STATE_TO_BE_REMOVED);
                break;
            case COLLISION_TYPE_WEST:
                f->setState(SPRITE_STATE_TO_BE_REMOVED);
                break;
            default:
                break;
            }
        }

        if (cpN.checkCollision(rect)) {
            return COLLISION_TYPE_NORTH;
        }
        else if (cpS.checkCollision(rect)) {
            return COLLISION_TYPE_SOUTH;
        }
        else if (cpE.checkCollision(rect) || cpE1.checkCollision(rect)) {
            return COLLISION_TYPE_EAST;
        }
        else if (cpW.checkCollision(rect) || cpW1.checkCollision(rect)) {
            return COLLISION_TYPE_WEST;
        }

        // invisible blocks
    }
    else if (sprite->getAuxiliaryState() == SPRITE_STATE_INVISIBLE && state != SPRITE_STATE_FALLING) {
        Rectangle rect = sprite->getRect();
        if (cpN.checkCollision(rect)) {
            return COLLISION_TYPE_NORTH;
        }
    }

    return COLLISION_TYPE_NONE;

}

CollisionType Player::checkCollisionBaddie(Sprite* sprite) {
    
    if (sprite->getState() != SPRITE_STATE_NO_COLLIDABLE) {
        Rectangle rect = sprite->getRect();

        for (auto& fireball : fireballs) {
            Fireball* f = &fireball;
            if (f->checkCollision(sprite) && sprite->getState() != SPRITE_STATE_DYING) {
                f->setState(SPRITE_STATE_TO_BE_REMOVED);
                return COLLISION_TYPE_FIREBALL;
            }
        }

        if (state == SPRITE_STATE_JUMPING || vel.y > 0) {
            if (cpN.checkCollision(rect)) {
				return COLLISION_TYPE_NORTH;
			}
            else if (cpS.checkCollision(rect)) {
				return COLLISION_TYPE_SOUTH;
			}
            else if (cpE.checkCollision(rect) || cpE1.checkCollision(rect)) {
				return COLLISION_TYPE_EAST;
			} 
            else if (cpW.checkCollision(rect) || cpW1.checkCollision(rect)) {
				return COLLISION_TYPE_WEST;
			}
        }
    }

	return COLLISION_TYPE_NONE;
}

void Player::drawHud() const {
	std::map<std::string, Texture2D>& textures = ResourceManager::getInstance().getTextures(); // Getting textures from ResourceManager

	// Left side of the screen
	if (isLuigi) {
        DrawTexture(textures["guiLuigi"], 34, 32, WHITE); // Draw Luigi icon
    } else {
        DrawTexture(textures["guiMario"], 34, 32, WHITE); // Draw Mario icon
    }
	DrawTexture(textures["guiX"], 54, 49, WHITE); // Draw "X" icon for lives
	drawWhiteSmallNumber(lives < 0 ? 0 : lives, 68, 49); // Draw lives number

	// Draw Yoshi coins if needed
    /*for (int i = 0; i < yoshiCoins; i++) {
        DrawTexture(textures["guiCoin"], 34 + textures["guiMario"].width + 16 + i * textures["guiCoin"].width, 32, WHITE);
    }*/

	int leftshift = 1225;
	DrawTexture(textures["guiCoin"], GetScreenWidth() - 115 - leftshift, 32, WHITE); // Draw coin icon
	DrawTexture(textures["guiX"], GetScreenWidth() - 97 - leftshift, 34, WHITE); // Draw "X" icon for coins
	drawWhiteSmallNumber(coins, GetScreenWidth() - 34 - getSmallNumberWidth(coins) - leftshift, 34); // Draw coins number
	drawWhiteSmallNumber(points, GetScreenWidth() - 34 - getSmallNumberWidth(points) - leftshift, 50); // Draw points number

	int t = getRemainingTime();
	t = t < 0 ? 0 : t;

	DrawTexture(textures["guiTime"], GetScreenWidth() - 34 - 176 - leftshift, 32, WHITE); // Draw time icon
	drawYellowSmallNumber(t, GetScreenWidth() - 34 - 128 - getSmallNumberWidth(t) - leftshift, 50); // Draw remaining time

	// Center top of the screen
    if (reservedPowerUp == PLAYER_TYPE_SUPER) {
        DrawTexture(textures["mushroom"], GetScreenWidth() / 2 - textures["mushroom"].width / 2, 32, WHITE);
    } else if (reservedPowerUp == PLAYER_TYPE_FLOWER) {
        DrawTexture(textures["fireFlower0"], GetScreenWidth() / 2 - textures["fireFlower0"].width / 2, 32, WHITE);
    }
	DrawTexture(textures["guiNextItem"], GetScreenWidth() / 2 - textures["guiNextItem"].width / 2, 20, WHITE);
}

void Player::updateCollisionProbes() {

    cpN.setX(pos.x + dim.x / 2 - cpN.getWidth() / 2);
    if (ducking) {
        cpN.setY(pos.y + dim.y - 32);
    }
    else {
        cpN.setY(pos.y);
    }

    cpS.setX(pos.x + dim.x / 2 - cpS.getWidth() / 2);
    cpS.setY(pos.y + dim.y - cpS.getHeight());

    cpE.setX(pos.x + dim.x - cpE.getWidth());
    cpE1.setX(pos.x + dim.x - cpE1.getWidth());
    cpW.setX(pos.x);
    cpW1.setX(pos.x);

    if (type == PLAYER_TYPE_SMALL) {

        if (ducking) {
            cpE.setY(pos.y + 21 - cpE.getHeight() / 2);
            cpE1.setY(pos.y + 30 - cpE1.getHeight() / 2);
            cpW.setY(pos.y + 21 - cpW.getHeight() / 2);
            cpW1.setY(pos.y + 30 - cpW1.getHeight() / 2);
        }
        else {
            cpE.setY(pos.y + dim.y * 0.33 - cpE.getHeight() / 2);
            cpE1.setY(pos.y + dim.y * 0.66 - cpE1.getHeight() / 2);
            cpW.setY(pos.y + dim.y * 0.33 - cpW.getHeight() / 2);
            cpW1.setY(pos.y + dim.y * 0.66 - cpW1.getHeight() / 2);
        }

    }
    else {

        if (ducking) {
            cpE.setY(pos.y + 36 - cpE.getHeight() / 2);
            cpE1.setY(pos.y + 46 - cpE1.getHeight() / 2);
            cpW.setY(pos.y + 36 - cpW.getHeight() / 2);
            cpW1.setY(pos.y + 46 - cpW1.getHeight() / 2);
        }
        else {
            cpE.setY(pos.y + dim.y * 0.33 - cpE.getHeight() / 2);
            cpE1.setY(pos.y + dim.y * 0.66 - cpE1.getHeight() / 2);
            cpW.setY(pos.y + dim.y * 0.33 - cpW.getHeight() / 2);
            cpW1.setY(pos.y + dim.y * 0.66 - cpW1.getHeight() / 2);
        }

    }

}

float Player::getSpeedX() const {
    return speedX;
}

float Player::getMaxSpeedX() const {
    return maxSpeedX;
}

float Player::getJumpSpeed() const {
    return jumpSpeed;
}

float Player::getActivationWidth() const {
    return activationWidth;
}

void Player::setImmortal(bool immortal) {
    this->immortal = immortal;
}

bool Player::isImmortal() const {
    return immortal;
}

void Player::setActivationWidth(float activationWidth) {
    this->activationWidth = activationWidth;
}

void Player::setLives(int lives) {
    this->lives = lives;
}

void Player::setCoins(int coins) {
    this->coins = coins;
}

void Player::setCoinsFromPreviousMap(int coinsFromPreviousMap) {
    this->coinsFromPreviousMap = coinsFromPreviousMap;
}

void Player::setYoshiCoins(int yoshiCoins) {
    this->yoshiCoins = yoshiCoins;
}

void Player::setPoints(int points) {
    this->points = points;
}

void Player::setPointsFromPreviousMap(int pointsFromPreviousMap) {
    this->pointsFromPreviousMap = pointsFromPreviousMap;
}

int Player::getRemainingTime() const {
    return static_cast<int>(maxTime - ellapsedTime);
}

float Player::getEllapsedTime() const {
    return ellapsedTime;
}

void Player::setMaxTime(float maxTime) {
    this->maxTime = maxTime;
}

void Player::setLastStateBeforeTransition(SpriteState lastStateBeforeTransition) {
    this->lastStateBeforeTransition = lastStateBeforeTransition;
}
void Player::setType(PlayerType pT)
{
    if (pT == PLAYER_TYPE_SMALL) this->changeToSmall();
    else if (pT == PLAYER_TYPE_SUPER) this->changeToSuper();
    else if (pT == PLAYER_TYPE_FLOWER) this->changeToFlower();
}

void Player::setRemainingTime(int remainingTime) {
    ellapsedTime = maxTime - static_cast<float>(remainingTime); 
    if (ellapsedTime < 0) ellapsedTime = 0; 
}

void Player::setGameWorld(GameWorld* gw) {
    this->gw = gw;
}

void Player::setMap(Map* map) {
    this->map = map;
}

GameWorld* Player::getGameWorld() const {
    return gw;
}

Map* Player::getMap() const {
    return map;
}

int Player::getLives() const {
    return lives;
}

int Player::getCoins() const {
    return coins;
}

int Player::getCoinsFromPreviousMap() const {
    return coinsFromPreviousMap;
}

int Player::getYoshiCoins() const {
    return yoshiCoins;
}

int Player::getPoints() const {
    return points;
}

int Player::getPointsFromPreviousMap() const {
    return pointsFromPreviousMap;
}

void Player::addLives(int lives) {
    this->lives += lives;
}

void Player::removeLives(int lives) {
    this->lives -= lives;
}

void Player::addCoins(int coins) {
    this->coins += coins;
}

void Player::addYoshiCoins(int yoshiCoins) {
    this->yoshiCoins += yoshiCoins;
}

void Player::removeCoins(int coins) {
    this->coins -= coins;
}

void Player::addPoints(int points) {
    this->points += points;
}

void Player::removePoints(int points) {
    this->points -= points;
}

void Player::changeToSmall() {
    type = PLAYER_TYPE_SMALL;
    pos.y = pos.y + 12;
    dim.y = 40;
    maxFrames = 2;
}

void Player::changeToSuper() {
    type = PLAYER_TYPE_SUPER;
    dim.y = 56;
    maxFrames = 3;
}

void Player::changeToFlower() {
    type = PLAYER_TYPE_FLOWER;
    dim.y = 56;
    maxFrames = 3;
}

void Player::setReservedPowerUp(PlayerType reservedPowerUp) {
    this->reservedPowerUp = reservedPowerUp;
}

PlayerType Player::getReservedPowerUp() const {
    return reservedPowerUp;
}

void Player::releaseReservedPowerUp() {

    Item* item = nullptr;
	Vector2 itemPos = { getX() + getWidth() / 2 - 16, getY() - 32 }; // Set item position to be above the player
    itemPos.x -= 16;

    if (reservedPowerUp == PLAYER_TYPE_SUPER) {
        item = new Mushroom(itemPos, Vector2{ 32, 32 }, Vector2{ 0, 150 }, RED, false, true, true);
    }
    else if (reservedPowerUp == PLAYER_TYPE_FLOWER) {
        item = new FireFlower(itemPos, Vector2{ 32, 32 }, Vector2{ 0, 150 }, RED, true, true);
    }

    if (item != nullptr) {
        item->setState(SPRITE_STATE_ACTIVE);
        item->setFacingDirection(facingDirection);
        map->getItems().push_back(item);
        PlaySound(ResourceManager::getInstance().getSounds()["reserveItemRelease"]);
    }

    reservedPowerUp = PLAYER_TYPE_SMALL;

}

PlayerType Player::getType() const {
    return type;
}

void Player::setInvulnerable(bool invulnerable) {
    this->invulnerable = invulnerable;
}

bool Player::isInvulnerable() const {
    return invulnerable;
}

void Player::setInvincible(bool invincible) {
    this->invincible = invincible;
}

bool Player::isInvincible() const {
    return invincible;
}

bool Player::isTransitioning() const {
    return state == SPRITE_STATE_TRANSITIONING_SMALL_TO_SUPER ||
        state == SPRITE_STATE_TRANSITIONING_SMALL_TO_FLOWER ||
        state == SPRITE_STATE_TRANSITIONING_SUPER_TO_FLOWER ||
        state == SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL ||
        state == SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL;
}

void Player::reset(bool removePowerUps) {
    reset(removePowerUps, true);
}

void Player::reset(bool removePowerUps, bool resetPointsToSaved) {

    if (removePowerUps) {
        changeToSmall();
        reservedPowerUp = PLAYER_TYPE_SMALL;
    }
    vel.x = 0;
    vel.y = 0;
    dyingVelY = -600;
    state = SPRITE_STATE_ON_GROUND;
    facingDirection = DIRECTION_RIGHT;
    ducking = false;
    lookingUp = false;
    running = false;
    ellapsedTime = 0;
    invulnerable = false;
    invulnerableTimeAcum = 0;
    invulnerableBlink = false;
    yoshiCoins = 0;
    
    // Reset points and coins to saved values from previous maps only when Mario dies (resetPointsToSaved = true)
    // When advancing to next map, keep current points and coins (resetPointsToSaved = false)
    if (resetPointsToSaved) {
        points = pointsFromPreviousMap;
        coins = coinsFromPreviousMap;
    }

}

void Player::resetAll() {
    lives = 5;
    coins = 0;
    coinsFromPreviousMap = 0;
    yoshiCoins = 0;
    points = 0;
    pointsFromPreviousMap = 0;
    reset(true);
}

void Player::playPlayerDownMusicStream() {

    std::map<std::string, Music> musics = ResourceManager::getInstance().getMusics();

    if (!playerDownMusicStreamPlaying) {
        playerDownMusicStreamPlaying = true;
    }
    else {
        if (!IsMusicStreamPlaying(musics["playerDown"])) {
            PlayMusicStream(musics["playerDown"]);
        }
        else {
            UpdateMusicStream(musics["playerDown"]);
            if (static_cast<int>(GetMusicTimeLength(musics["playerDown"])) == static_cast<int>(GetMusicTimePlayed(musics["playerDown"]))) {
                StopMusicStream(musics["playerDown"]);
                playerDownMusicStreamPlaying = false;
            }
        }
    }
}

void Player::playGameOverMusicStream() {

    std::map<std::string, Music> musics = ResourceManager::getInstance().getMusics();

    if (!gameOverMusicStreamPlaying) {
        gameOverMusicStreamPlaying = true;
    }
    else {
        if (!IsMusicStreamPlaying(musics["gameOver"])) {
            PlayMusicStream(musics["gameOver"]);
        }
        else {
            UpdateMusicStream(musics["gameOver"]);
            if (static_cast<int>(GetMusicTimeLength(musics["gameOver"])) == static_cast<int>(GetMusicTimePlayed(musics["gameOver"]))) {
                StopMusicStream(musics["gameOver"]);
                gameOverMusicStreamPlaying = false;
            }
        }
    }

}

bool Player::isPlayerDownMusicStreamPlaying() const {
    return playerDownMusicStreamPlaying;
}

bool Player::isGameOverMusicStreamPlaying() const {
    return gameOverMusicStreamPlaying;
}

Vector2 Player::getSouthCollisionProbePos() const {
    return Vector2{ cpS.getX(), cpS.getY() };
}

void Player::toLuigi() {
    isLuigi = true;
    jumpSpeed = -700; // Luigi jumps higher than Mario
}

void Player::toMario() {
    isLuigi = false;
    jumpSpeed = -600; // Mario jumps lower than Luigi
}