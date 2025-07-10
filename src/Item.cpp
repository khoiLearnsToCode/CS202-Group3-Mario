#include "Item.h"
#include "ResourceManager.h"
#include "Mario.h"
#include "GameWorld.h"
#include "raylib.h"
#include "Sprite.h"
#include <cmath>



// Item base class
Item::Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float frameTime, int maxFrames, int earnedPoints) :
    Sprite(pos, dim, vel, color, frameTime, maxFrames, DIRECTION_RIGHT, 0, earnedPoints),
    onHitFrameAcum(0),
    onHitFrameTime(0.1f),
    maxOnHitFrame(4),
    currentOnHitFrame(0),
    pointsFrameAcum(0),
    pointsFrameTime(1.0f),
    pauseGameOnHit(false) {}

Item::~Item() = default;

void Item::onSouthCollision(Mario& mario) {}
bool Item::isPauseGameOnHit() {
    return pauseGameOnHit;
}

// Coin
Coin::Coin(Vector2 pos, Vector2 dim, Color color) :
    Item(pos, dim, Vector2{ 0, 0 }, color, 0.1f, 4, 200) {}

Coin::~Coin() = default;

void Coin::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        frameAcum += delta;
        if (frameAcum >= frameTime) {
            frameAcum = 0;
            currentFrame = (currentFrame + 1) % maxFrames;
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            currentOnHitFrame++;
            if (currentOnHitFrame >= maxOnHitFrame) {
                state = SPRITE_STATE_TO_BE_REMOVED;
            }
        }
        pointsFrameAcum = std::min(pointsFrameAcum + delta, pointsFrameTime);
    }
}

void Coin::draw() {
    auto& textures = ResourceManager::getInstance().getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        std::string key = TextFormat("coin%d", currentFrame);
        if (textures.find(key) != textures.end()) {
            DrawTexture(textures[key], pos.x, pos.y, WHITE);
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        std::string stardustKey = TextFormat("stardust%d", currentOnHitFrame);
        std::string pointsKey = TextFormat("guiPoints%d", earnedPoints);
        if (textures.find(stardustKey) != textures.end()) {
            DrawTexture(textures[stardustKey], pos.x, pos.y, WHITE);
        }
        if (textures.find(pointsKey) != textures.end()) {
            DrawTexture(textures[pointsKey],
                pos.x + dim.x / 2 - textures[pointsKey].width / 2,
                pos.y - textures[pointsKey].height - (50 * pointsFrameAcum / pointsFrameTime),
                WHITE);
        }
    }
    if (false) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }
}

void Coin::playCollisionSound() {
  
}

void Coin::updateMario(Mario& mario) {

}

CollisionType Coin::checkCollision(Sprite* sprite) {
    return CheckCollisionRecs( getRect(), sprite->getRect() ) ? COLLISION_TYPE_COLLIDED : COLLISION_TYPE_NONE;
}



// Mushroom
Mushroom::Mushroom(Vector2 pos, Vector2 dim, Vector2 vel, Color color, bool applyGravity, bool doCollisionOnGround, bool blinking) :
    Item(pos, dim, vel, color, 0.1f, 4, 1000),
    applyGravity(applyGravity),
    doCollisionOnGround(doCollisionOnGround),
    blinking(blinking),
    blinkingAcum(0),
    blinkingTime(0.5f),
    doBlink(false) {}

Mushroom::~Mushroom() = default;

void Mushroom::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        frameAcum += delta;
        if (frameAcum >= frameTime) {
            frameAcum = 0;
            currentFrame = (currentFrame + 1) % maxFrames;
        }
        if (applyGravity) {
            vel.y += GameWorld::gravity * delta;
        }
        pos.x += vel.x * delta;
        pos.y += vel.y * delta;
        if (doCollisionOnGround && pos.y + dim.y > GetScreenHeight()) {
            pos.y = GetScreenHeight() - dim.y;
            vel.y = 0;
        }
        if (blinking) {
            blinkingAcum += delta;
            if (blinkingAcum >= blinkingTime) {
                blinkingAcum = 0;
                doBlink = !doBlink;
            }
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            currentOnHitFrame++;
            if (currentOnHitFrame >= maxOnHitFrame) {
                state = SPRITE_STATE_TO_BE_REMOVED;
            }
        }
        pointsFrameAcum = std::min(pointsFrameAcum + delta, pointsFrameTime);
    }
    updateCollisionProbes();
}

void Mushroom::draw() {
    auto& textures = ResourceManager::getInstance().getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        std::string key = TextFormat("mushroom%d", currentFrame);
        if (textures.find(key) != textures.end() && (!blinking || doBlink)) {
            DrawTexture(textures[key], pos.x, pos.y, WHITE);
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        std::string stardustKey = TextFormat("stardust%d", currentOnHitFrame);
        std::string pointsKey = TextFormat("guiPoints%d", earnedPoints);
        if (textures.find(stardustKey) != textures.end()) {
            DrawTexture(textures[stardustKey], pos.x, pos.y, WHITE);
        }
        if (textures.find(pointsKey) != textures.end()) {
            DrawTexture(textures[pointsKey],
                pos.x + dim.x / 2 - textures[pointsKey].width / 2,
                pos.y - textures[pointsKey].height - (50 * pointsFrameAcum / pointsFrameTime),
                WHITE);
        }
    }
    if (false) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }
}

void Mushroom::playCollisionSound() {
    
}

void Mushroom::updateMario(Mario& mario) {
    
}

void Mushroom::onSouthCollision(Mario& mario) {
  
}



// OneUpMushroom
OneUpMushroom::OneUpMushroom(Vector2 pos, Vector2 dim, Vector2 vel, Color color) :
    Item(pos, dim, vel, color, 0, 0, 1) {}

OneUpMushroom::~OneUpMushroom() = default;

void OneUpMushroom::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE) {
        pos.x += vel.x * delta * (facingDirection == DIRECTION_RIGHT ? 1 : -1);
        pos.y += vel.y * delta;
        vel.y += GameWorld::gravity * delta;
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            state = SPRITE_STATE_TO_BE_REMOVED;
        }
        pointsFrameAcum = std::min(pointsFrameAcum + delta, pointsFrameTime);
    }
    updateCollisionProbes();
}

void OneUpMushroom::draw() {
    auto& textures = ResourceManager::getInstance().getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        if (textures.find("1UpMushroom") != textures.end()) {
            DrawTexture(textures["1UpMushroom"], pos.x, pos.y, WHITE);
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        std::string key = "gui1Up";
        if (textures.find(key) != textures.end()) {
            DrawTexture(textures[key],
                pos.x + dim.x / 2 - textures[key].width / 2,
                pos.y - textures[key].height - (50 * pointsFrameAcum / pointsFrameTime),
                WHITE);
        }
    }
    if (false) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }
}

void OneUpMushroom::playCollisionSound() {
    
}

void OneUpMushroom::updateMario(Mario& mario) {

}


// FireFlower
FireFlower::FireFlower(Vector2 pos, Vector2 dim, Vector2 vel, Color color, bool doCollisionOnGround, bool blinking) :
    Item(pos, dim, vel, color, 0.2f, 2, 1000),
    doCollisionOnGround(doCollisionOnGround),
    blinking(blinking),
    blinkingAcum(0),
    blinkingTime(0.1f),
    doBlink(false) {
    pauseGameOnHit = true;
}

FireFlower::~FireFlower() = default;

void FireFlower::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE) {
        frameAcum += delta;
        if (frameAcum >= frameTime) {
            frameAcum = 0;
            currentFrame = (currentFrame + 1) % maxFrames;
        }
        pos.y += vel.y * delta;
        if (doCollisionOnGround && pos.y + dim.y > GetScreenHeight()) {
            pos.y = GetScreenHeight() - dim.y;
            vel.y = 0;
        }
        if (blinking) {
            blinkingAcum += delta;
            if (blinkingAcum >= blinkingTime) {
                blinkingAcum = 0;
                doBlink = !doBlink;
            }
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            state = SPRITE_STATE_TO_BE_REMOVED;
        }
        pointsFrameAcum = std::min(pointsFrameAcum + delta, pointsFrameTime);
    }
}

void FireFlower::draw() {
    auto& textures = ResourceManager::getInstance().getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        std::string key = TextFormat("fireFlower%d", currentFrame);
        if (textures.find(key) != textures.end() && !doBlink) {
            DrawTexture(textures[key], pos.x, pos.y, WHITE);
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        std::string pointsKey = TextFormat("guiPoints%d", earnedPoints);
        if (textures.find(pointsKey) != textures.end()) {
            DrawTexture(textures[pointsKey],
                pos.x + dim.x / 2 - textures[pointsKey].width / 2,
                pos.y - textures[pointsKey].height - (50 * pointsFrameAcum / pointsFrameTime),
                WHITE);
        }
    }
    if (false) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }
}

void FireFlower::playCollisionSound() {
    
}

void FireFlower::updateMario(Mario& mario) {
    
}

void FireFlower::onSouthCollision(Mario& mario) {

}


// Star
Star::Star(Vector2 pos, Vector2 dim, Vector2 vel, Color color) :
    Item(pos, dim, vel, color, 0, 0, 1000) {}

Star::~Star() = default;

void Star::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE) {
        pos.x += vel.x * delta * (facingDirection == DIRECTION_RIGHT ? 1 : -1);
        pos.y += vel.y * delta;
        vel.y += GameWorld::gravity * delta;
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            state = SPRITE_STATE_TO_BE_REMOVED;
        }
        pointsFrameAcum = std::min(pointsFrameAcum + delta, pointsFrameTime);
    }
    updateCollisionProbes();
}

void Star::draw() {
    auto& textures = ResourceManager::getInstance().getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        if (textures.find("star") != textures.end()) {
            DrawTexture(textures["star"], pos.x, pos.y, WHITE);
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        std::string pointsKey = TextFormat("guiPoints%d", earnedPoints);
        if (textures.find(pointsKey) != textures.end()) {
            DrawTexture(textures[pointsKey],
                pos.x + dim.x / 2 - textures[pointsKey].width / 2,
                pos.y - textures[pointsKey].height - (50 * pointsFrameAcum / pointsFrameTime),
                WHITE);
        }
    }
    if (false) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }
}

void Star::playCollisionSound() {

}

void Star::updateMario(Mario& mario) {

}

void Star::onSouthCollision(Mario& mario) {

}



// ThreeUpMoon
ThreeUpMoon::ThreeUpMoon(Vector2 pos, Vector2 dim, Vector2 vel, Color color) :
    Item(pos, dim, vel, color, 0, 0, 3) {}

ThreeUpMoon::~ThreeUpMoon() = default;

void ThreeUpMoon::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE) {
        pos.x += vel.x * delta * (facingDirection == DIRECTION_RIGHT ? 1 : -1);
        pos.y += vel.y * delta;
        vel.y += GameWorld::gravity * delta;
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            state = SPRITE_STATE_TO_BE_REMOVED;
        }
        pointsFrameAcum = std::min(pointsFrameAcum + delta, pointsFrameTime);
    }
    updateCollisionProbes();
}

void ThreeUpMoon::draw() {
    auto& textures = ResourceManager::getInstance().getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        if (textures.find("3UpMoon") != textures.end()) {
            DrawTexture(textures["3UpMoon"], pos.x, pos.y, WHITE);
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        std::string key = "gui3Up";
        if (textures.find(key) != textures.end()) {
            DrawTexture(textures[key],
                pos.x + dim.x / 2 - textures[key].width / 2,
                pos.y - textures[key].height - (50 * pointsFrameAcum / pointsFrameTime),
                WHITE);
        }
    }
    if (false) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }
}

void ThreeUpMoon::playCollisionSound() {

}

void ThreeUpMoon::updateMario(Mario& mario) {

}



// YoshiCoin
YoshiCoin::YoshiCoin(Vector2 pos, Vector2 dim, Color color) :
    Item(pos, dim, Vector2{ 0, 0 }, color, 0.1f, 4, 1000), countingUp(true) {
    onHitFrameTime = 0.1f;
    maxOnHitFrame = 4;
}

YoshiCoin::~YoshiCoin() = default;

void YoshiCoin::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        frameAcum += delta;
        if (frameAcum >= frameTime) {
            frameAcum = 0;
            if (countingUp) {
                currentFrame++;
                if (currentFrame >= maxFrames) {
                    currentFrame = maxFrames - 2;
                    countingUp = false;
                }
            }
            else {
                currentFrame--;
                if (currentFrame <= 0) {
                    countingUp = true;
                }
            }
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            currentOnHitFrame++;
            if (currentOnHitFrame >= maxOnHitFrame) {
                state = SPRITE_STATE_TO_BE_REMOVED;
            }
        }
        pointsFrameAcum = std::min(pointsFrameAcum + delta, pointsFrameTime);
    }
}

void YoshiCoin::draw() {
    auto& textures = ResourceManager::getInstance().getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        std::string key = TextFormat("yoshiCoin%d", currentFrame);
        if (textures.find(key) != textures.end()) {
            DrawTexture(textures[key], pos.x, pos.y, WHITE);
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        std::string stardustKey = TextFormat("stardust%d", currentOnHitFrame);
        std::string pointsKey = TextFormat("guiPoints%d", earnedPoints);
        if (textures.find(stardustKey) != textures.end()) {
            DrawTexture(textures[stardustKey], pos.x, pos.y, WHITE);
            DrawTexture(textures[stardustKey], pos.x, pos.y + 20, WHITE);
        }
        if (textures.find(pointsKey) != textures.end()) {
            DrawTexture(textures[pointsKey],
                pos.x + dim.x / 2 - textures[pointsKey].width / 2,
                pos.y - textures[pointsKey].height - (50 * pointsFrameAcum / pointsFrameTime),
                WHITE);
        }
    }
    if (false) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }
}

void YoshiCoin::playCollisionSound() {

}

void YoshiCoin::updateMario(Mario& mario) {

}

CollisionType YoshiCoin::checkCollision(Sprite* sprite) {
    return CheckCollisionRecs( getRect(), sprite->getRect() ) ? COLLISION_TYPE_COLLIDED : COLLISION_TYPE_NONE;
}


// CourseClearToken
CourseClearToken::CourseClearToken(Vector2 pos, Vector2 dim, Color color) :
    Item(pos, dim, Vector2{ 0, 0 }, color, 0, 0, 10000)
{
    onHitFrameTime = 0.1f;
    maxOnHitFrame = 4;
}

CourseClearToken::~CourseClearToken() = default;

void CourseClearToken::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE) {
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            currentOnHitFrame++;
            if (currentOnHitFrame >= maxOnHitFrame) {
                state = SPRITE_STATE_TO_BE_REMOVED;
            }
        }
        pointsFrameAcum = std::min(pointsFrameAcum + delta, pointsFrameTime);
    }
}

void CourseClearToken::draw() {
    auto& textures = ResourceManager::getInstance().getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        if (textures.find("courseClearToken") != textures.end()) {
            DrawTexture(textures["courseClearToken"], pos.x, pos.y, WHITE);
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        std::string stardustKey = TextFormat("stardust%d", currentOnHitFrame);
        std::string pointsKey = TextFormat("guiPoints%d", earnedPoints);
        if (textures.find(stardustKey) != textures.end()) {
            DrawTexture(textures[stardustKey], pos.x, pos.y, WHITE);
            DrawTexture(textures[stardustKey], pos.x + 32, pos.y, WHITE);
        }
        if (textures.find(pointsKey) != textures.end()) {
            DrawTexture(textures[pointsKey],
                pos.x + dim.x / 2 - textures[pointsKey].width / 2,
                pos.y - textures[pointsKey].height - (50 * pointsFrameAcum / pointsFrameTime),
                WHITE);
        }
    }
    if (false) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }
}

void CourseClearToken::playCollisionSound() {
 
}

void CourseClearToken::updateMario(Mario& mario) {
}

CollisionType CourseClearToken::checkCollision(Sprite* sprite) {
    return CheckCollisionRecs( getRect(), sprite->getRect() ) ? COLLISION_TYPE_COLLIDED : COLLISION_TYPE_NONE;
}



// ItemFactory
Item* FactoryItem::createItem(const std::string& type, Vector2 pos, Vector2 dim, Vector2 vel, Color color, bool applyGravity, bool doCollisionOnGround, bool blinking) {
    if (type == "Coin") return new Coin(pos, dim, color);
    if (type == "Mushroom") return new Mushroom(pos, dim, vel, color, applyGravity, doCollisionOnGround, blinking);
    if (type == "OneUpMushroom") return new OneUpMushroom(pos, dim, vel, color);
    if (type == "FireFlower") return new FireFlower(pos, dim, vel, color, doCollisionOnGround, blinking);
    if (type == "Star") return new Star(pos, dim, vel, color);
    if (type == "ThreeUpMoon") return new ThreeUpMoon(pos, dim, vel, color);
    if (type == "YoshiCoin") return new YoshiCoin(pos, dim, color);
    if (type == "CourseClearToken") return new CourseClearToken(pos, dim, color);
    return nullptr;
}