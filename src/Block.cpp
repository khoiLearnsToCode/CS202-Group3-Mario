
#include "Block.h"
#include "GameWorld.h"
#include "ResourceManager.h"
#include "raylib.h"
#include <iostream>
#include "Item.h"
Block::Block() : Sprite(), hit(false) {}
Block::Block(Vector2 pos, Vector2 dim, Color color)
	: Sprite(pos, dim, color), hit(false) {}
Block::Block(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Sprite(pos, dim, color, frameTime, maxFrames), hit(false) {}
Block::Block(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames, int earnedPoints)
	: Sprite(pos, dim, color, frameTime, maxFrames, earnedPoints), hit(false) {}
Block::~Block() {}

void Block::resetHit() {
	hit = false;
}

void Block::doHit(Mario& mario, Map* map) {
	if (!hit)
		hit = true;
}

// StoneBlock
void StoneBlock::update() {

}
void StoneBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block101"), pos.x, pos.y, color);
}
StoneBlock::StoneBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
StoneBlock::StoneBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
StoneBlock::~StoneBlock() = default;

// WoodBlock
void WoodBlock::update() {

}
void WoodBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block102"), pos.x, pos.y, color);
}
WoodBlock::WoodBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
WoodBlock::WoodBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
WoodBlock::~WoodBlock() = default;

// GrassBlock
void GrassBlock::update() {

}
void GrassBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block95"), pos.x, pos.y, color);
}
GrassBlock::GrassBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
GrassBlock::GrassBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
GrassBlock::~GrassBlock() = default; // No specific destruction logic needed

// CloudBlock
void CloudBlock::update() {

}
void CloudBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block88"), pos.x, pos.y, color);
}
CloudBlock::CloudBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
CloudBlock::CloudBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
CloudBlock::~CloudBlock() = default; // No specific destruction logic needed

// EyesClosedBlock
void EyesClosedBlock::update() {

}
void EyesClosedBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block90"), pos.x, pos.y, color);
}
EyesClosedBlock::EyesClosedBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
EyesClosedBlock::EyesClosedBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
EyesClosedBlock::~EyesClosedBlock() {
	std::cout << "Destroying EyesClosedBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

// EyesOpenedBlock
void EyesOpenedBlock::update() {

}
void EyesOpenedBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block91"), pos.x, pos.y, color);
}
EyesOpenedBlock::EyesOpenedBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
EyesOpenedBlock::EyesOpenedBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
EyesOpenedBlock::~EyesOpenedBlock() {
	std::cout << "Destroying EyesOpenedBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

// QuestionBlock
void QuestionBlock::update() {

	const float delta = GetFrameTime();

	if (hit && coinAnimationRunning) {

		coinAnimationAcum += delta;
		if (coinAnimationAcum >= coinAnimationTime) {
			coinAnimationRunning = false;
			stardustAnimationRunning = true;
			pointsAnimationRunning = true;
			coinAnimationFrame++;
			coinAnimationFrame %= maxFrames;
		}

		coinFrameAcum += delta;
		if (coinFrameAcum > frameTime) {
			coinFrameAcum = 0;
			coinAnimationFrame++;
			coinAnimationFrame %= maxFrames;
		}

		coinY += coinVelY * delta;
		coinVelY += GameWorld::gravity;

	}

	if (!hit) {
		frameAcum += GetFrameTime();
		if (frameAcum >= frameTime) {
			frameAcum = 0;
			currentFrame++;
			currentFrame %= maxFrames;
		}
	}

	if (stardustAnimationRunning) {

		stardustAnimationAcum += delta;
		if (stardustAnimationAcum >= stardustAnimationTime) {
			stardustAnimationAcum = 0;
			stardustAnimationFrame++;
			if (stardustAnimationFrame == maxStartDustAnimationFrame) {
				stardustAnimationRunning = false;
			}
		}

	}

	if (pointsAnimationRunning) {

		pointsFrameAcum += delta;
		if (pointsFrameAcum >= pointsFrameTime) {
			pointsAnimationRunning = false;
		}

	}

}
void QuestionBlock::draw() {
	if (!hit) {
		std::string key = TextFormat("block%d", 97 + currentFrame); // Assuming block97 is the first frame
		DrawTexture(ResourceManager::getInstance().getTexture(key), pos.x, pos.y, color);
	}
	else
		DrawTexture(ResourceManager::getInstance().getTexture("block91"), pos.x, pos.y, color);
}
void QuestionBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		PlaySound(ResourceManager::getInstance().getSounds()["coin"]);
		hit = true;
		coinAnimationRunning = true;
		coinY = pos.y;
		mario.addCoins(1);
		mario.addPoints(earnedPoints);
	}
}
QuestionBlock::QuestionBlock(Vector2 pos, Vector2 dim, Color color) :
	QuestionBlock(pos, dim, color, 0.1, 4) {}
QuestionBlock::QuestionBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames) :
	Block(pos, dim, color, frameTime, maxFrames, 10),
	coinAnimationTime(0.6),
	coinAnimationAcum(0),
	coinFrameAcum(0),
	coinAnimationFrame(0),
	coinAnimationRunning(false),
	coinY(0),
	coinVelY(-400),
	stardustAnimationTime(0.1),
	stardustAnimationAcum(0),
	stardustAnimationFrame(0),
	maxStartDustAnimationFrame(4),
	stardustAnimationRunning(false),
	pointsFrameAcum(0),
	pointsFrameTime(0.5),
	pointsAnimationRunning(false) {}

QuestionBlock::~QuestionBlock() = default;

//QuestionMushroomBlock
void QuestionMushroomBlock::update() {

	const float delta = GetFrameTime();

	if (!hit) {
		frameAcum += delta;
		if (frameAcum >= frameTime) {
			frameAcum = 0;
			currentFrame++;
			currentFrame %= maxFrames;
		}
	}

	if (item != nullptr) {
		item->setY(item->getY() + itemVelY * delta);
		if (item->getY() <= itemMinY) {
			item->setY(itemMinY);
			item->setState(SPRITE_STATE_ACTIVE);
			map->getItems().push_back(item);
			item = nullptr;
		}
	}

}
void QuestionMushroomBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block97"), pos.x, pos.y, color);
}
void QuestionMushroomBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		hit = true;
		Vector2 itemPos = { pos.x, pos.y - dim.y };
		Vector2 itemDim = { 32, 32 }; // Assuming a standard item size
		Vector2 itemVel = { 0, -150 }; // Initial velocity for the item
		item = FactoryItem::createItem("Mushroom", itemPos, itemDim, itemVel, RED, true, true, false);
		item->setFacingDirection(mario.getFacingDirection());
		itemMinY = pos.y - 32;
		this->map = map;
	}
}
QuestionMushroomBlock::QuestionMushroomBlock(Vector2 pos, Vector2 dim, Color color) :
	QuestionMushroomBlock(pos, dim, color, 0.1, 4) {}

QuestionMushroomBlock::QuestionMushroomBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames) :
	Block(pos, dim, color, frameTime, maxFrames),
	item(nullptr),
	itemVelY(-80),
	itemMinY(0),
	map(nullptr) {}
QuestionMushroomBlock::~QuestionMushroomBlock() = default;

// QuestionFireFlowerBlock
void QuestionFireFlowerBlock::update() {
	if (!hit) {
		frameAcum += GetFrameTime();
		if (frameAcum >= frameTime) {
			frameAcum = 0;
			currentFrame = (currentFrame + 1) % maxFrames;
		}
	}
}
void QuestionFireFlowerBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block97"), pos.x, pos.y, color);
}
void QuestionFireFlowerBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		hit = true;
		Vector2 itemPos = { pos.x, pos.y - dim.y };
		Vector2 itemDim = { 32, 32 };
		Vector2 itemVel = { 0, -150 };
		Item* item = FactoryItem::createItem("FireFlower", itemPos, itemDim, itemVel, ORANGE, true, true, false);
		item->setFacingDirection(mario.getFacingDirection());
		itemMinY = pos.y - 32;
		this->map = map;
	}
}
QuestionFireFlowerBlock::QuestionFireFlowerBlock(Vector2 pos, Vector2 dim, Color color) :
	QuestionFireFlowerBlock(pos, dim, color, 0.1, 4) {}

QuestionFireFlowerBlock::QuestionFireFlowerBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames) :
	Block(pos, dim, color, frameTime, maxFrames),
	item(nullptr),
	itemVelY(-80),
	itemMinY(0),
	map(nullptr) {}

QuestionFireFlowerBlock::~QuestionFireFlowerBlock() = default;

// QuestionStarBlock
void QuestionStarBlock::update() {
	const float delta = GetFrameTime();
	if (!hit) {
		frameAcum += delta;
		if (frameAcum >= frameTime) {
			frameAcum = 0;
			currentFrame++;
			currentFrame %= maxFrames;
		}
	}
	if (item != nullptr) {
		item->setY(item->getY() + itemVelY * delta);
		if (item->getY() <= itemMinY) {
			item->setY(itemMinY);
			item->setState(SPRITE_STATE_ACTIVE);
			map->getItems().push_back(item);
			item = nullptr;
		}
	}
}
void QuestionStarBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block97"), pos.x, pos.y, color);
}
void QuestionStarBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		hit = true;
		Vector2 itemPos = { pos.x, pos.y - dim.y };
		Vector2 itemDim = { 30, 32 }; 
		Vector2 itemVel = { 0, -150 };
		Item* item = FactoryItem::createItem("Star", itemPos, itemDim, itemVel, YELLOW, true, true, false);
		item->setFacingDirection(mario.getFacingDirection());
		itemMinY = pos.y - 32;
		this->map = map;
	}
}
QuestionStarBlock::QuestionStarBlock(Vector2 pos, Vector2 dim, Color color) :
	QuestionStarBlock(pos, dim, color, 0.1, 4) {}

QuestionStarBlock::QuestionStarBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames) :
	Block(pos, dim, color, frameTime, maxFrames),
	item(nullptr),
	itemVelY(-80),
	itemMinY(0),
	map(nullptr) {}

QuestionStarBlock::~QuestionStarBlock() = default;

// QuestionOneUpMushroomBlock
void QuestionOneUpMushroomBlock::update() {

	const float delta = GetFrameTime();

	if (!hit) {
		frameAcum += delta;
		if (frameAcum >= frameTime) {
			frameAcum = 0;
			currentFrame++;
			currentFrame %= maxFrames;
		}
	}

	if (item != nullptr) {
		item->setY(item->getY() + itemVelY * delta);
		if (item->getY() <= itemMinY) {
			item->setY(itemMinY);
			item->setState(SPRITE_STATE_ACTIVE);
			map->getItems().push_back(item);
			item = nullptr;
		}
	}

}
void QuestionOneUpMushroomBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block97"), pos.x, pos.y, color);
}
void QuestionOneUpMushroomBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		hit = true;
		Vector2 itemPos = { pos.x, pos.y - dim.y };
		Vector2 itemDim = { 32, 32 };
		Vector2 itemVel = { 0, -150 };
		Item* item = FactoryItem::createItem("OneUpMushroom", itemPos, itemDim, itemVel, YELLOW, true, true, false);
		item->setFacingDirection(mario.getFacingDirection());
		itemMinY = pos.y - 32;
		this->map = map;
	}
}
QuestionOneUpMushroomBlock::QuestionOneUpMushroomBlock(Vector2 pos, Vector2 dim, Color color) :
	QuestionOneUpMushroomBlock(pos, dim, color, 0.1, 4) {}

QuestionOneUpMushroomBlock::QuestionOneUpMushroomBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames) :
	Block(pos, dim, color, frameTime, maxFrames),
	item(nullptr),
	itemVelY(-80),
	itemMinY(0),
	map(nullptr) {}

QuestionOneUpMushroomBlock::~QuestionOneUpMushroomBlock() = default;

// QuestionThreeUpMoonBlock
void QuestionThreeUpMoonBlock::update() {

	const float delta = GetFrameTime();

	if (!hit) {
		frameAcum += delta;
		if (frameAcum >= frameTime) {
			frameAcum = 0;
			currentFrame++;
			currentFrame %= maxFrames;
		}
	}

	if (item != nullptr) {
		item->setY(item->getY() + itemVelY * delta);
		if (item->getY() <= itemMinY) {
			item->setY(itemMinY);
			item->setState(SPRITE_STATE_ACTIVE);
			map->getItems().push_back(item);
			item = nullptr;
		}
	}

}
void QuestionThreeUpMoonBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block97"), pos.x, pos.y, color);
}
void QuestionThreeUpMoonBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		hit = true;
		Vector2 itemPos = { pos.x, pos.y - dim.y };
		Vector2 itemDim = { 32, 32 };
		Vector2 itemVel = { 0, -150 };
		Item* item = FactoryItem::createItem("ThreeUpMoon", itemPos, itemDim, itemVel, YELLOW, true, true, false);
		item->setFacingDirection(mario.getFacingDirection());
		itemMinY = pos.y - 32;
		this->map = map;
	}
}
QuestionThreeUpMoonBlock::QuestionThreeUpMoonBlock(Vector2 pos, Vector2 dim, Color color) :
	QuestionThreeUpMoonBlock(pos, dim, color, 0.1, 4) {}

QuestionThreeUpMoonBlock::QuestionThreeUpMoonBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames) :
	Block(pos, dim, color, frameTime, maxFrames),
	item(nullptr),
	itemVelY(-80),
	itemMinY(0),
	map(nullptr) {}

QuestionThreeUpMoonBlock::~QuestionThreeUpMoonBlock() = default;

// ExclamationBlock
void ExclamationBlock::update() {
	if (!hit) {
		frameAcum += GetFrameTime();
		if (frameAcum >= frameTime) {
			frameAcum = 0;
			currentFrame = (currentFrame + 1) % maxFrames;
		}
	}
}
void ExclamationBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block89"), pos.x, pos.y, color);
}
void ExclamationBlock::doHit(Mario& mario, Map* map) {
	if (!hit)
		hit = true;
	Vector2 itemPos = { pos.x, pos.y - dim.y };
	Vector2 itemDim = { 32, 32 }; // Assuming a standard item size
	Vector2 itemVel = { 0, -150 };
	Item* item = FactoryItem::createItem("Coin", itemPos, itemDim, itemVel, YELLOW, true, true, false);
	if (item) {
		map->getItems().push_back(item); // Add the item to the map's items vector
	}
	mario.addPoints(100);
	mario.addCoins(1);
}
ExclamationBlock::ExclamationBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
ExclamationBlock::ExclamationBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
ExclamationBlock::~ExclamationBlock() = default; // No specific destruction logic needed

//InvisibleBlock
void InvisibleBlock::update() {

}
void InvisibleBlock::draw() {

}
void InvisibleBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		hit = true;
		Vector2 itemPos = { pos.x, pos.y - dim.y }; 
		Vector2 itemDim = { 32, 32 };
		Vector2 itemVel = { 0, -150 }; 
		Item* item = FactoryItem::createItem("Coin", itemPos, itemDim, itemVel, YELLOW, true, true, false);
		if (item) {
			map->getItems().push_back(item); // Add the item to the map's items vector
			mario.addCoins(1);
		}
		mario.addPoints(100); 
	}
}
InvisibleBlock::InvisibleBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
InvisibleBlock::InvisibleBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
InvisibleBlock::~InvisibleBlock() = default; // No specific destruction logic needed

// MessageBlock
void MessageBlock::update() {

}
void MessageBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block96"), pos.x, pos.y, color);
}
void MessageBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		hit = true;
		// Display a message or perform an action
		mario.addPoints(50);
	}
}
MessageBlock::MessageBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
MessageBlock::MessageBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
MessageBlock::~MessageBlock() = default; // No specific destruction logic needed


//Factory pattern for Block creation
Block* BlockFactory::createBlock(const std::string& type, Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames) {
	if (type == "StoneBlock") {
		return new StoneBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "WoodBlock") {
		return new WoodBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "GrassBlock") {
		return new GrassBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "CloudBlock") {
		return new CloudBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "EyesClosedBlock") {
		return new EyesClosedBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "EyesOpenedBlock") {
		return new EyesOpenedBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "QuestionBlock") {
		return new QuestionBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "QuestionMushroomBlock") {
		return new QuestionMushroomBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "QuestionFireFlowerBlock") {
		return new QuestionFireFlowerBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "QuestionStarBlock") {
		return new QuestionStarBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "QuestionOneUpMushroomBlock") {
		return new QuestionOneUpMushroomBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "QuestionThreeUpMoonBlock") {
		return new QuestionThreeUpMoonBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "ExclamationBlock") {
		return new ExclamationBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "InvisibleBlock") {
		return new InvisibleBlock(pos, dim, color, frameTime, maxFrames);
	}
	else if (type == "MessageBlock") {
		return new MessageBlock(pos, dim, color, frameTime, maxFrames);
	}
	else {
		std::cerr << "Unknown block type: " << type << std::endl;
		return nullptr; // or throw an exception
	}
}