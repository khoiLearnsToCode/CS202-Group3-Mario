#include "Baddies.h"
#include "ResourceManager.h"
#include "GameWorld.h"
#include <cmath>

// Goomba
Goomba::Goomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 1, 100) {
}

void Goomba::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void Goomba::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "goomba" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType Goomba::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// FlyingGoomba
FlyingGoomba::FlyingGoomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 1, 200), flyAmplitude(50.0f) {
}

void FlyingGoomba::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += flyAmplitude * sin(animationTime * 2.0f) * GetFrameTime();
    animationTime += GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void FlyingGoomba::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "flyingGoomba" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType FlyingGoomba::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// RedKoopaTroopa
RedKoopaTroopa::RedKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 2, 200), isInShell(false) {
}

void RedKoopaTroopa::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    if (isInShell) {
        vel.x = 0;
        frameAcum += GetFrameTime();
        if (frameAcum >= frameTime) {
            currentFrame = (currentFrame + 1) % maxFrames;
            frameAcum = 0;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void RedKoopaTroopa::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = isInShell ? "redKoopaTroopaShell" : "redKoopaTroopa" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType RedKoopaTroopa::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        if (!isInShell) {
            isInShell = true;
            hitsToDie--;
            PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        }
        else {
            onHit();
        }
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// GreenKoopaTroopa
GreenKoopaTroopa::GreenKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 2, 200), isInShell(false) {
}

void GreenKoopaTroopa::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    if (isInShell) {
        vel.x = 0;
        frameAcum += GetFrameTime();
        if (frameAcum >= frameTime) {
            currentFrame = (currentFrame + 1) % maxFrames;
            frameAcum = 0;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void GreenKoopaTroopa::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = isInShell ? "greenKoopaTroopaShell" : "greenKoopaTroopa" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType GreenKoopaTroopa::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        if (!isInShell) {
            isInShell = true;
            hitsToDie--;
            PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        }
        else {
            onHit();
        }
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// BlueKoopaTroopa
BlueKoopaTroopa::BlueKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 2, 200), isInShell(false) {
}

void BlueKoopaTroopa::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    if (isInShell) {
        vel.x = 0;
        frameAcum += GetFrameTime();
        if (frameAcum >= frameTime) {
            currentFrame = (currentFrame + 1) % maxFrames;
            frameAcum = 0;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void BlueKoopaTroopa::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = isInShell ? "blueKoopaTroopaShell" : "blueKoopaTroopa" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType BlueKoopaTroopa::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        if (!isInShell) {
            isInShell = true;
            hitsToDie--;
            PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        }
        else {
            onHit();
        }
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// YellowKoopaTroopa
YellowKoopaTroopa::YellowKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 2, 200), isInShell(false) {
}

void YellowKoopaTroopa::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    if (isInShell) {
        vel.x = 0;
        frameAcum += GetFrameTime();
        if (frameAcum >= frameTime) {
            currentFrame = (currentFrame + 1) % maxFrames;
            frameAcum = 0;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void YellowKoopaTroopa::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = isInShell ? "yellowKoopaTroopaShell" : "yellowKoopaTroopa" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType YellowKoopaTroopa::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        if (!isInShell) {
            isInShell = true;
            hitsToDie--;
            PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        }
        else {
            onHit();
        }
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// BuzzyBeetle
BuzzyBeetle::BuzzyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 2, 200), isInShell(false) {
}

void BuzzyBeetle::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    if (isInShell) {
        vel.x = 0;
        frameAcum += GetFrameTime();
        if (frameAcum >= frameTime) {
            currentFrame = (currentFrame + 1) % maxFrames;
            frameAcum = 0;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void BuzzyBeetle::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = isInShell ? "buzzyBeetleShell" : "buzzyBeetle" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType BuzzyBeetle::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        if (!isInShell) {
            isInShell = true;
            hitsToDie--;
            PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        }
        else {
            onHit();
        }
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// MummyBeetle
MummyBeetle::MummyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 2, 200), isInShell(false) {
}

void MummyBeetle::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    if (isInShell) {
        vel.x = 0;
        frameAcum += GetFrameTime();
        if (frameAcum >= frameTime) {
            currentFrame = (currentFrame + 1) % maxFrames;
            frameAcum = 0;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void MummyBeetle::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = isInShell ? "mummyBeetleShell" : "mummyBeetle" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType MummyBeetle::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        if (!isInShell) {
            isInShell = true;
            hitsToDie--;
            PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        }
        else {
            onHit();
        }
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// BobOmb
BobOmb::BobOmb(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 1, 200), explosionTimer(3.0f) {
}

void BobOmb::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > explosionTimer) {
            state = SPRITE_STATE_DEAD;
            PlaySound(ResourceManager::getInstance().getSound("bobOmbExplode"));
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void BobOmb::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "bobOmb" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType BobOmb::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// PiranhaPlant
PiranhaPlant::PiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, DIRECTION_RIGHT, 1, 300), emergeTimer(2.0f) {
}

void PiranhaPlant::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    emergeTimer -= GetFrameTime();
    if (emergeTimer <= 0) {
        emergeTimer = 4.0f;
        pos.y = pos.y == 0 ? dim.y : 0; // Xuất hiện/ẩn trong ống
    }
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void PiranhaPlant::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "piranhaPlant" + std::to_string(currentFrame);
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType PiranhaPlant::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type != COLLISION_TYPE_NONE && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return type;
    }
    return type;
}

// JumpingPiranhaPlant
JumpingPiranhaPlant::JumpingPiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, DIRECTION_RIGHT, 1, 300), jumpTimer(2.0f) {
}

void JumpingPiranhaPlant::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    jumpTimer -= GetFrameTime();
    if (jumpTimer <= 0) {
        vel.y = -300.0f; // Nhảy lên
        jumpTimer = 2.0f;
    }
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void JumpingPiranhaPlant::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "jumpingPiranhaPlant" + std::to_string(currentFrame);
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType JumpingPiranhaPlant::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type != COLLISION_TYPE_NONE && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return type;
    }
    return type;
}

// BulletBill
BulletBill::BulletBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 1, 200) {
}

void BulletBill::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void BulletBill::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "bulletBill" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType BulletBill::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// BanzaiBill
BanzaiBill::BanzaiBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 2, 500) {
}

void BanzaiBill::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void BanzaiBill::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "banzaiBill" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType BanzaiBill::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// MontyMole
MontyMole::MontyMole(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 1, 200), emergeTimer(2.0f) {
}

void MontyMole::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    emergeTimer -= GetFrameTime();
    if (emergeTimer <= 0) {
        vel.y = -200.0f; // Nhảy lên từ dưới đất
        emergeTimer = 3.0f;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void MontyMole::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "montyMole" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType MontyMole::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// Rex
Rex::Rex(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 2, 300) {
}

void Rex::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    vel.y += GameWorld::gravity * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void Rex::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "rex" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType Rex::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// Muncher
Muncher::Muncher(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, DIRECTION_RIGHT, 9999, 0), isInvulnerable(true) {
}

void Muncher::update() {
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void Muncher::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "muncher" + std::to_string(currentFrame);
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType Muncher::checkCollision(Sprite* sprite) {
    return Sprite::checkCollision(sprite); // Muncher bất tử, không xử lý chết
}

// Swooper
Swooper::Swooper(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Baddie(pos, dim, vel, color, 0.1f, 2, vel.x < 0 ? DIRECTION_LEFT : DIRECTION_RIGHT, 1, 200), swoopTimer(3.0f) {
}

void Swooper::update() {
    if (state == SPRITE_STATE_DYING) {
        setAttributesOnDying();
        animationTime += GetFrameTime();
        if (animationTime > 0.5f) {
            state = SPRITE_STATE_DEAD;
        }
        return;
    }
    swoopTimer -= GetFrameTime();
    if (swoopTimer <= 0) {
        vel.y = 200.0f; // Lao xuống
        swoopTimer = 3.0f;
    }
    else if (swoopTimer > 1.5f) {
        vel.y = -100.0f; // Bay lên
    }
    pos.x += vel.x * GetFrameTime();
    pos.y += vel.y * GetFrameTime();
    updateCollisionProbes();
    frameAcum += GetFrameTime();
    if (frameAcum >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcum = 0;
    }
}

void Swooper::draw() {
    if (state != SPRITE_STATE_DEAD) {
        std::string textureKey = "swooper" + std::to_string(currentFrame) + (facingDirection == DIRECTION_RIGHT ? "R" : "L");
        DrawTexture(ResourceManager::getInstance().getTexture(textureKey), pos.x, pos.y, WHITE);
    }
}

CollisionType Swooper::checkCollision(Sprite* sprite) {
    CollisionType type = Sprite::checkCollision(sprite);
    if (type == COLLISION_TYPE_NORTH && sprite->getState() == SPRITE_STATE_JUMPING) {
        onHit();
        PlaySound(ResourceManager::getInstance().getSound("enemyDefeated"));
        return COLLISION_TYPE_NORTH;
    }
    return type;
}

// BaddieFactory
Baddie* BaddieFactory::createBaddie(const std::string& type, Vector2 pos, Vector2 vel) {
    if (type == "Goomba") return new Goomba(pos, { 32, 30 }, vel, MAROON);
    if (type == "FlyingGoomba") return new FlyingGoomba(pos, { 32, 30 }, vel, MAROON);
    if (type == "RedKoopaTroopa") return new RedKoopaTroopa(pos, { 32, 54 }, vel, RED);
    if (type == "GreenKoopaTroopa") return new GreenKoopaTroopa(pos, { 32, 54 }, vel, GREEN);
    if (type == "BlueKoopaTroopa") return new BlueKoopaTroopa(pos, { 32, 54 }, vel, BLUE);
    if (type == "YellowKoopaTroopa") return new YellowKoopaTroopa(pos, { 32, 54 }, vel, YELLOW);
    if (type == "BuzzyBeetle") return new BuzzyBeetle(pos, { 32, 30 }, vel, DARKGRAY);
    if (type == "MummyBeetle") return new MummyBeetle(pos, { 32, 30 }, vel, GRAY);
    if (type == "BobOmb") return new BobOmb(pos, { 32, 30 }, vel, BLACK);
    if (type == "PiranhaPlant") return new PiranhaPlant(pos, { 32, 48 }, vel, GREEN);
    if (type == "JumpingPiranhaPlant") return new JumpingPiranhaPlant(pos, { 32, 48 }, vel, GREEN);
    if (type == "BulletBill") return new BulletBill(pos, { 32, 24 }, vel, BLACK);
    if (type == "BanzaiBill") return new BanzaiBill(pos, { 64, 48 }, vel, BLACK);
    if (type == "MontyMole") return new MontyMole(pos, { 32, 30 }, vel, BROWN);
    if (type == "Rex") return new Rex(pos, { 48, 48 }, vel, PURPLE);
    if (type == "Muncher") return new Muncher(pos, { 32, 32 }, vel, BLACK);
    if (type == "Swooper") return new Swooper(pos, { 32, 30 }, vel, BLUE);
    return nullptr;
}