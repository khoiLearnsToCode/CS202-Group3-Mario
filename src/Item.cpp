#include "Item.h"
#include "ResourceManager.h"
#include "Mario.h"
#include "GameWorld.h"
#include "raylib.h"
#include "Sprite.h"

Item::Item() :
    Item(Vector2(0, 0), Vector2(0, 0), Vector2(0, 0), BLACK, 0, 0, 0) {}
Item::Item(Vector2 pos, Vector2 dim, Color color, int earnedPoints) :
    Item(pos, dim, Vector2(0, 0), color, 0, 0, earnedPoints) {}
Item::Item(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames, int earnedPoints) :
    Item(pos, dim, Vector2(0, 0), color, frameTime, maxFrames, earnedPoints) {}
Item::Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, int earnedPoints) :
    Item(pos, dim, vel, color, 0, 0, earnedPoints) {}

Item::Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float frameTime, int maxFrames) :
    Item(pos, dim, vel, color, frameTime, maxFrames, 0) {}

Item::Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float frameTime, int maxFrames, int earnedPoints) :
    Sprite(pos, dim, vel, color, frameTime, maxFrames, DIRECTION_RIGHT, 0, earnedPoints),
    onHitFrameAcum(0),
    onHitFrameTime(1),
    maxOnHitFrame(1),
    currentOnHitFrame(0),
    pointsFrameAcum(0),
    pointsFrameTime(1),
    pauseGameOnHit(false) {}

Item::~Item() = default;

void Item::onSouthCollision(Mario& mario) {
}

bool Item::isPauseGameOnHit() {
    return pauseGameOnHit;
}

// Coin
Coin::Coin(Vector2 pos, Vector2 dim, Color color) :
    Item(pos, dim, color, 0.1, 4, 200) {
    onHitFrameTime = 0.1;
    maxOnHitFrame = 4;
}

Coin::~Coin() = default;

void Coin::update() {
    const float delta = GetFrameTime();
    frameAcum += delta;
    if (frameAcum >= frameTime) {
        frameAcum = 0;
        currentFrame++;
        currentFrame %= maxFrames;
    }
    if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            currentOnHitFrame++;
            if (currentOnHitFrame == maxOnHitFrame) {
                state = SPRITE_STATE_TO_BE_REMOVED;
            }
        }
        pointsFrameAcum += delta;
        if (pointsFrameAcum >= pointsFrameTime) {
            pointsFrameAcum = pointsFrameTime;
        }

    }
}

void Coin::draw() {
    std::map<std::string, Texture2D>& textures = ResourceManager::getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        DrawTexture(ResourceManager::getTextures()[std::string(TextFormat("coin%d", currentFrame))], pos.x, pos.y, WHITE);
    }
    else if (state == SPRITE_STATE_HIT) {
        DrawTexture(textures[std::string(TextFormat("stardust%d", currentOnHitFrame))], pos.x, pos.y, WHITE);
        const std::string pointsStr = TextFormat("guiPoints%d", earnedPoints);
        DrawTexture(textures[pointsStr],
            pos.x + dim.x / 2 - textures[pointsStr].width / 2,
            pos.y - textures[pointsStr].height - (50 * pointsFrameAcum / pointsFrameTime),
            WHITE);
    }
    if (GameWorld::debug) {
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
    return CheckCollisionRecs(getRect(), sprite->getRect()) ? COLLISION_TYPE_COLLIDED : COLLISION_TYPE_NONE;
}

// Mushroom
Mushroom::Mushroom(Vector2 pos, Vector2 dim, Vector2 vel, Color color) :
	Mushroom(pos, dim, vel, color, true, true, false) {}
Mushroom::Mushroom(Vector2 pos, Vector2 dim, Vector2 vel, Color color, bool applyGravity, bool doCollisionOnGround, bool blinking) :
    Item(pos, dim, vel, color, 0.1, 4),
    applyGravity(applyGravity),
    doCollisionOnGround(doCollisionOnGround),
    blinking(blinking),
    blinkingAcum(0),
    blinkingTime(0.5f),
    doBlink(false) {}
Mushroom::~Mushroom() = default;
void Mushroom::update() {
	const float delta = GetFrameTime();
	frameAcum += delta;
	if (frameAcum >= frameTime) {
		frameAcum = 0;
		currentFrame++;
		currentFrame %= maxFrames;
	}
	if (state == SPRITE_STATE_HIT) {
		onHitFrameAcum += delta;
		if (onHitFrameAcum >= onHitFrameTime) {
			onHitFrameAcum = 0;
			currentOnHitFrame++;
			if (currentOnHitFrame == maxOnHitFrame) {
				state = SPRITE_STATE_TO_BE_REMOVED;
			}
		}
		pointsFrameAcum += delta;
		if (pointsFrameAcum >= pointsFrameTime) {
			pointsFrameAcum = pointsFrameTime;
		}

	}
	if (applyGravity) {
		vel.y += 1000 * delta; // apply gravity
	}
	pos.x += vel.x * delta;
	pos.y += vel.y * delta;
	if (doCollisionOnGround && pos.y + dim.y > GetScreenHeight()) {
		pos.y = GetScreenHeight() - dim.y;
		vel.y = 0; // stop falling
	}
	if (blinking) {
		blinkingAcum += delta;
		if (blinkingAcum >= blinkingTime) {
			blinkingAcum = 0;
			doBlink = !doBlink; // toggle blinking
		}
	}
}

void Mushroom::draw() {
	std::map<std::string, Texture2D>& textures = ResourceManager::getTextures();
	if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
		if (blinking && doBlink) {
			DrawTexture(textures[std::string(TextFormat("mushroom%d", currentFrame))], pos.x, pos.y, WHITE);
		}
		else if (!blinking) {
			DrawTexture(textures[std::string(TextFormat("mushroom%d", currentFrame))], pos.x, pos.y, WHITE);
		}
	}
	else if (state == SPRITE_STATE_HIT) {
		DrawTexture(textures[std::string(TextFormat("stardust%d", currentOnHitFrame))], pos.x, pos.y, WHITE);
		const std::string pointsStr = TextFormat("guiPoints%d", earnedPoints);
		DrawTexture(textures[pointsStr],
			pos.x + dim.x / 2 - textures[pointsStr].width / 2,
			pos.y - textures[pointsStr].height - (50 * pointsFrameAcum / pointsFrameTime),
			WHITE);
	}
	if (GameWorld::debug) {
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
    Item(pos, dim, vel, color, 0, 0, 1);
}
OneUpMushroom::~OneUpMushroom() = default;
void OneUpMushroom::playCollisionSound() {
	
}
void OneUpMushroom::updateMario(Mario& mario) {
	
}
void OneUpMushroom::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE) {
        if (facingDirection == DIRECTION_RIGHT) {
            pos.x += vel.x * delta;
        }
        else {
            pos.x -= vel.x * delta;
        }
        pos.y += vel.y * delta;
        vel.y += GameWorld::gravity;
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            state = SPRITE_STATE_TO_BE_REMOVED;
        }
        pointsFrameAcum += delta;
        if (pointsFrameAcum >= pointsFrameTime) {
            pointsFrameAcum = pointsFrameTime;
        }
    }
    updateCollisionProbes();
}

void OneUpMushroom::draw() {
    std::map<std::string, Texture2D>& textures = ResourceManager::getTextures();

    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        DrawTexture(textures["1UpMushroom"], pos.x, pos.y, WHITE);
    }
    else if (state == SPRITE_STATE_HIT) {
        const std::string oneUpStr = "gui1Up";
        DrawTexture(textures[oneUpStr],
            pos.x + dim.x / 2 - textures[oneUpStr].width / 2,
            pos.y - textures[oneUpStr].height - (50 * pointsFrameAcum / pointsFrameTime),
            WHITE);
    }
    if (GameWorld::debug) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }
}

// FireFlower
FireFlower::FireFlower(Vector2 pos, Vector2 dim, Color color) :
    FireFlower(pos, dim, Vector2(0, 0), color, false, false) {
}

FireFlower::FireFlower(Vector2 pos, Vector2 dim, Vector2 vel, Color color, bool doCollisionOnGround, bool blinking) :
    Item(pos, dim, vel, color, 0.2, 2, 1000),
    doCollisionOnGround(doCollisionOnGround),
    blinking(blinking),
    blinkingAcum(0),
    blinkingTime(0.1),
    doBlink(false) {
    pauseGameOnHit = true;
}
FireFlower::~FireFlower() = default;
void FireFlower::update() {
    const float delta = GetFrameTime();
    frameAcum += delta;
    if (frameAcum >= frameTime) {
        frameAcum = 0;
        currentFrame++;
        currentFrame %= maxFrames;
    }
    if (state == SPRITE_STATE_ACTIVE) {
        pos.y += vel.y * delta;
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
        pointsFrameAcum += delta;
        if (pointsFrameAcum >= pointsFrameTime) {
            pointsFrameAcum = pointsFrameTime;
        }
    }
}

void FireFlower::draw() {
    std::map<std::string, Texture2D>& textures = ResourceManager::getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        if (!doBlink) {
            DrawTexture(textures[std::string(TextFormat("fireFlower%d", currentFrame))], pos.x, pos.y, WHITE);
        }
    }
    else if (state == SPRITE_STATE_HIT) {
        const std::string pointsStr = TextFormat("guiPoints%d", earnedPoints);
        DrawTexture(textures[pointsStr],
            pos.x + dim.x / 2 - textures[pointsStr].width / 2,
            pos.y - textures[pointsStr].height - (50 * pointsFrameAcum / pointsFrameTime),
            WHITE);
    }
    if (GameWorld::debug) {
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
        if (facingDirection == DIRECTION_RIGHT) {
            pos.x += vel.x * delta;
        }
        else {
            pos.x -= vel.x * delta;
        }
        pos.y += vel.y * delta;
        vel.y += GameWorld::gravity;
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            state = SPRITE_STATE_TO_BE_REMOVED;
        }
        pointsFrameAcum += delta;
        if (pointsFrameAcum >= pointsFrameTime) {
            pointsFrameAcum = pointsFrameTime;
        }
    }
    updateCollisionProbes();
}

void Star::draw() {
    std::map<std::string, Texture2D>& textures = ResourceManager::getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        DrawTexture(textures["star"], pos.x, pos.y, WHITE);
    }
    else if (state == SPRITE_STATE_HIT) {
        const std::string pointsStr = TextFormat("guiPoints%d", earnedPoints);
        DrawTexture(textures[pointsStr],
            pos.x + dim.x / 2 - textures[pointsStr].width / 2,
            pos.y - textures[pointsStr].height - (50 * pointsFrameAcum / pointsFrameTime),
            WHITE);
    }
    if (GameWorld::debug) {
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
//ThreeUpMoon
ThreeUpMoon::ThreeUpMoon(Vector2 pos, Vector2 dim, Vector2 vel, Color color) :
    Item(pos, dim, vel, color, 0, 0, 3) {
}

ThreeUpMoon::~ThreeUpMoon() = default;

void ThreeUpMoon::update() {
    const float delta = GetFrameTime();
    if (state == SPRITE_STATE_ACTIVE) {
        if (facingDirection == DIRECTION_RIGHT) {
            pos.x += vel.x * delta;
        }
        else {
            pos.x -= vel.x * delta;
        }
        pos.y += vel.y * delta;
        vel.y += GameWorld::gravity;
    }
    else if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            state = SPRITE_STATE_TO_BE_REMOVED;
        }
        pointsFrameAcum += delta;
        if (pointsFrameAcum >= pointsFrameTime) {
            pointsFrameAcum = pointsFrameTime;
        }
    }
    updateCollisionProbes();
}

void ThreeUpMoon::draw() {

    std::map<std::string, Texture2D>& textures = ResourceManager::getTextures();

    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        DrawTexture(textures["3UpMoon"], pos.x, pos.y, WHITE);
    }
    else if (state == SPRITE_STATE_HIT) {
        const std::string threeUpStr = "gui3Up";
        DrawTexture(textures[threeUpStr],
            pos.x + dim.x / 2 - textures[threeUpStr].width / 2,
            pos.y - textures[threeUpStr].height - (50 * pointsFrameAcum / pointsFrameTime),
            WHITE);
    }
    if (GameWorld::debug) {
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
void ThreeUpMoon::onSouthCollision(Mario& mario) {

}

//YoshiCoin
YoshiCoin::YoshiCoin(Vector2 pos, Vector2 dim, Color color) :
    Item(pos, dim, color, 0.1, 4, 1000), countingUp(true) {
    onHitFrameTime = 0.1;
    maxOnHitFrame = 4;
}

YoshiCoin::~YoshiCoin() = default;

void YoshiCoin::update() {
    const float delta = GetFrameTime();
    frameAcum += delta;
    if (frameAcum >= frameTime) {
        frameAcum = 0;
        if (countingUp) {
            currentFrame++;
            if (currentFrame == maxFrames) {
                currentFrame = 2;
                countingUp = false;
            }
        }
        else {
            currentFrame--;
            if (currentFrame == 0) {
                countingUp = true;
            }
        }
    }
    if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            currentOnHitFrame++;
            if (currentOnHitFrame == maxOnHitFrame) {
                state = SPRITE_STATE_TO_BE_REMOVED;
            }
        }
        pointsFrameAcum += delta;
        if (pointsFrameAcum >= pointsFrameTime) {
            pointsFrameAcum = pointsFrameTime;
        }
    }
}

void YoshiCoin::draw() {
    std::map<std::string, Texture2D>& textures = ResourceManager::getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        DrawTexture(textures[std::string(TextFormat("yoshiCoin%d", currentFrame))], pos.x, pos.y, WHITE);
    }
    else if (state == SPRITE_STATE_HIT) {
        DrawTexture(textures[std::string(TextFormat("stardust%d", currentOnHitFrame))], pos.x, pos.y, WHITE);
        DrawTexture(textures[std::string(TextFormat("stardust%d", currentOnHitFrame))], pos.x, pos.y + 20, WHITE);
        const std::string pointsStr = TextFormat("guiPoints%d", earnedPoints);
        DrawTexture(textures[pointsStr],
            pos.x + dim.x / 2 - textures[pointsStr].width / 2,
            pos.y - textures[pointsStr].height - (50 * pointsFrameAcum / pointsFrameTime),
            WHITE);
    }
    if (GameWorld::debug) {
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
	
}
//CourseClearToken
CourseClearToken::CourseClearToken(Vector2 pos, Vector2 dim, Color color) :
    Item(pos, dim, color, 0, 0, 10000), minY(0), maxY(0) {
    minY = pos.y;
    maxY = minY + 8 * dim.y;
    vel.y = 100;
    onHitFrameTime = 0.1;
    maxOnHitFrame = 4;
}

CourseClearToken::~CourseClearToken() = default;

void CourseClearToken::update() {
    const float delta = GetFrameTime();
    if (pos.y < minY) {
        pos.y = minY;
        vel.y = -vel.y;
    }
    else if (pos.y > maxY) {
        pos.y = maxY;
        vel.y = -vel.y;
    }
    pos.y += vel.y * delta;
    if (state == SPRITE_STATE_HIT) {
        onHitFrameAcum += delta;
        if (onHitFrameAcum >= onHitFrameTime) {
            onHitFrameAcum = 0;
            currentOnHitFrame++;
            if (currentOnHitFrame == maxOnHitFrame) {
                state = SPRITE_STATE_TO_BE_REMOVED;
            }
        }
        pointsFrameAcum += delta;
        if (pointsFrameAcum >= pointsFrameTime) {
            pointsFrameAcum = pointsFrameTime;
        }
    }
}

void CourseClearToken::draw() {
    std::map<std::string, Texture2D>& textures = ResourceManager::getTextures();
    if (state == SPRITE_STATE_ACTIVE || state == SPRITE_STATE_IDLE) {
        DrawTexture(textures["courseClearToken"], pos.x, pos.y, WHITE);
    }
    else if (state == SPRITE_STATE_HIT) {
        DrawTexture(textures[std::string(TextFormat("stardust%d", currentOnHitFrame))], pos.x, pos.y, WHITE);
        DrawTexture(textures[std::string(TextFormat("stardust%d", currentOnHitFrame))], pos.x + 32, pos.y, WHITE);
        const std::string pointsStr = TextFormat("guiPoints%d", earnedPoints);
        DrawTexture(textures[pointsStr],
            pos.x + dim.x / 2 - textures[pointsStr].width / 2,
            pos.y - textures[pointsStr].height - (50 * pointsFrameAcum / pointsFrameTime),
            WHITE);
    }
    if (GameWorld::debug) {
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
void CourseClearToken::onSouthCollision(Mario& mario) {

}
CollisionType CourseClearToken::checkCollision(Sprite* sprite) {
}

Item* ItemFactory::createItem(const std::string& type, Vector2 pos, Vector2 dim, Vector2 vel, Color color) {
	if (type == "Coin") {
		return new Coin(pos, dim, color);
	}
	else if (type == "Mushroom") {
		return new Mushroom(pos, dim, vel, color);
	}
	else if (type == "OneUpMushroom") {
		return new OneUpMushroom(pos, dim, vel, color);
	}
	else if (type == "FireFlower") {
		return new FireFlower(pos, dim, vel, color);
	}
	else if (type == "Star") {
		return new Star(pos, dim, vel, color);
	}
	else if (type == "ThreeUpMoon") {
		return new ThreeUpMoon(pos, dim, vel, color);
	}
	else if (type == "YoshiCoin") {
		return new YoshiCoin(pos, dim, color);
	}
	else if (type == "CourseClearToken") {
		return new CourseClearToken(pos, dim, color);
	}
	return nullptr;
}