
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
StoneBlock::~StoneBlock() {
	std::cout << "Destroying StoneBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

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
WoodBlock::~WoodBlock() {
	std::cout << "Destroying WoodBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

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
GrassBlock::~GrassBlock() {
	std::cout << "Destroying GrassBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

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
CloudBlock::~CloudBlock() {
	std::cout << "Destroying CloudBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

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
	if (!hit) {
		frameAcum += GetFrameTime();
		if (frameAcum >= frameTime) {
			frameAcum = 0;
			currentFrame = (currentFrame + 1) % maxFrames;
		}
	}
}
void QuestionBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block97"), pos.x, pos.y, color);
}
void QuestionBlock::doHit(Mario& mario, Map* map) {
	if (!hit)
		hit = true;
	std::string itemType;
	if (mario.getType() == MARIO_TYPE_SMALL) {
		itemType = "Mushroom";
	}
	else if (mario.getType() == MARIO_TYPE_SUPER) {
		itemType = "FireFlower";
	}
	else {
		itemType = "";
	}
	if (!itemType.empty()) {
		Vector2 itemPos = { pos.x, pos.y - dim.y };
		Vector2 itemDim = { 32, 32 }; // Assuming a standard item size
		Vector2 itemVel = { 0, -150 };
		Item* item = FactoryItem::createItem(itemType, itemPos, itemDim, itemVel, WHITE, true, true, false);
		if (item) {
			// Add the item to the game world
			delete item;
		}
	}
}
QuestionBlock::QuestionBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
QuestionBlock::QuestionBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
QuestionBlock::~QuestionBlock() {
	std::cout << "Destroying QuestionBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

//QuestionMushroomBlock
void QuestionMushroomBlock::update() {
	
}
void QuestionMushroomBlock::draw() {
	DrawTexture(ResourceManager::getInstance().getTexture("block97"), pos.x, pos.y, color);
}
void QuestionMushroomBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		hit = true;
		Vector2 itemPos = { pos.x, pos.y - dim.y };
		Vector2 itemDim = { 32, 32 };
		Vector2 itemVel = { 0, -150 };
		Item* item = FactoryItem::createItem("Mushroom", itemPos, itemDim, itemVel, ORANGE, true, true, false);
		if (item) {
			//add item to the game world
			if (mario.getType() != MARIO_TYPE_SMALL) {
				item->updateMario(mario);
			}
			delete item;
		}
		mario.addPoints(200); 
	}
}
QuestionMushroomBlock::QuestionMushroomBlock(Vector2 pos, Vector2 dim, Color color)
	: QuestionBlock(pos, dim, color) {}
QuestionMushroomBlock::QuestionMushroomBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: QuestionBlock(pos, dim, color, frameTime, maxFrames) {}
QuestionMushroomBlock::~QuestionMushroomBlock() {
	std::cout << "Destroying QuestionMushroomBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

// QuestionFireFlowerBlock
void QuestionFireFlowerBlock::update() {

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
		if (item) {
			// add the item to the game world
			if (mario.getType() != MARIO_TYPE_FLOWER) {
				item->updateMario(mario);
			}
			delete item;
		}
		mario.addPoints(200);
	}
}
QuestionFireFlowerBlock::QuestionFireFlowerBlock(Vector2 pos, Vector2 dim, Color color)
	: QuestionBlock(pos, dim, color) {}
QuestionFireFlowerBlock::QuestionFireFlowerBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: QuestionBlock(pos, dim, color, frameTime, maxFrames) {}
QuestionFireFlowerBlock::~QuestionFireFlowerBlock() {
	std::cout << "Destroying QuestionFireFlowerBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

// QuestionStarBlock
void QuestionStarBlock::update() {

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
		if (item) {
			// Add the item to the game world
			delete item; 
			mario.setInvincible(true); 
		}
		mario.addPoints(1000); 
	}
}
QuestionStarBlock::QuestionStarBlock(Vector2 pos, Vector2 dim, Color color)
	: QuestionBlock(pos, dim, color) {}
QuestionStarBlock::QuestionStarBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: QuestionBlock(pos, dim, color, frameTime, maxFrames) {}
QuestionStarBlock::~QuestionStarBlock() {
	std::cout << "Destroying QuestionStarBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

// QuestionOneUpMushroomBlock
void QuestionOneUpMushroomBlock::update() {

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
		if (item) {
			// Add the item to the game world
			delete item; 
			mario.addLives(1); 
		}
		mario.addPoints(200); 
	}
}
QuestionOneUpMushroomBlock::QuestionOneUpMushroomBlock(Vector2 pos, Vector2 dim, Color color)
	: QuestionBlock(pos, dim, color) {}
QuestionOneUpMushroomBlock::QuestionOneUpMushroomBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: QuestionBlock(pos, dim, color, frameTime, maxFrames) {}
QuestionOneUpMushroomBlock::~QuestionOneUpMushroomBlock() {
	std::cout << "Destroying QuestionOneUpMushroomBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

// QuestionThreeUpMoonBlock
void QuestionThreeUpMoonBlock::update() {

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
		if (item) {
			// Add the item to the game world
			delete item; 
			mario.addLives(3); 
		}
		mario.addPoints(500); 
	}
}
QuestionThreeUpMoonBlock::QuestionThreeUpMoonBlock(Vector2 pos, Vector2 dim, Color color)
	: QuestionBlock(pos, dim, color) {}
QuestionThreeUpMoonBlock::QuestionThreeUpMoonBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: QuestionBlock(pos, dim, color, frameTime, maxFrames) {}
QuestionThreeUpMoonBlock::~QuestionThreeUpMoonBlock() {
	std::cout << "Destroying QuestionThreeUpMoonBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

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
		// Add the item to the game world
		delete item;
	}
	mario.addPoints(100);
	mario.addCoins(1);
}
ExclamationBlock::ExclamationBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
ExclamationBlock::ExclamationBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
ExclamationBlock::~ExclamationBlock() {
	std::cout << "Destroying ExclamationBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

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
			// Add the item to the game world
			delete item;
			mario.addCoins(1);
		}
		mario.addPoints(100); 
	}
}
InvisibleBlock::InvisibleBlock(Vector2 pos, Vector2 dim, Color color)
	: Block(pos, dim, color) {}
InvisibleBlock::InvisibleBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames)
	: Block(pos, dim, color, frameTime, maxFrames) {}
InvisibleBlock::~InvisibleBlock() {
	std::cout << "Destroying InvisibleBlock at position: " << pos.x << ", " << pos.y << std::endl;
}

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
MessageBlock::~MessageBlock() {
	std::cout << "Destroying MessageBlock at position: " << pos.x << ", " << pos.y << std::endl;
}


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