#pragma once
#include "raylib.h"
#include "Sprite.h"
#include "Mario.h"
#include <string>

//Base class for all blocks in the game
class Block : public Sprite {
protected:
	bool hit;

public:

	Block();
	Block(Vector2 pos, Vector2 dim, Color color);
	Block(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	Block(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames, int earnedPoints);
	~Block() override;

	void update() override = 0;
	void draw() override = 0;
	virtual void doHit(Mario& mario, Map* map);
	void resetHit();

	virtual std::string getType() const = 0;
};

class StoneBlock : public Block {
public:
	StoneBlock(Vector2 pos, Vector2 dim, Color color);
	StoneBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~StoneBlock() override;

	void update() override;
	void draw() override;
	std::string getType() const override {
		return "StoneBlock";
	}
};

class WoodBlock : public Block {
public:
	WoodBlock(Vector2 pos, Vector2 dim, Color color);
	WoodBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~WoodBlock() override;

	void update() override;
	void draw() override;
	std::string getType() const override {
		return "WoodBlock";
	}
};

class GrassBlock : public Block {
public:
	GrassBlock(Vector2 pos, Vector2 dim, Color color);
	GrassBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~GrassBlock() override;

	void update() override;
	void draw() override;
	std::string getType() const override {
		return "GrassBlock";
	}
};

class CloudBlock : public Block {
public:
	CloudBlock(Vector2 pos, Vector2 dim, Color color);
	CloudBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~CloudBlock() override;

	void update() override;
	void draw() override;
	std::string getType() const override {
		return "CloudBlock";
	}
};

class EyesClosedBlock : public Block {
public:
	EyesClosedBlock(Vector2 pos, Vector2 dim, Color color);
	EyesClosedBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~EyesClosedBlock() override;

	void update() override;
	void draw() override;
	std::string getType() const override {
		return "EyesClosedBlock";
	}
};

class EyesOpenedBlock : public Block {
public:
	EyesOpenedBlock(Vector2 pos, Vector2 dim, Color color);
	EyesOpenedBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~EyesOpenedBlock() override;

	void update() override;
	void draw() override;
	std::string getType() const override {
		return "EyesOpenedBlock";
	}
};

class QuestionBlock : public Block {
public:
	QuestionBlock(Vector2 pos, Vector2 dim, Color color);
	QuestionBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~QuestionBlock() override;

	void update() override;
	void draw() override;

	void doHit(Mario& mario, Map* map) override;
	std::string getType() const override {
		return "QuestionBlock";
	}
};

class QuestionMushroomBlock : public QuestionBlock {
public:
	QuestionMushroomBlock(Vector2 pos, Vector2 dim, Color color);
	QuestionMushroomBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~QuestionMushroomBlock() override;

	void update() override;
	void draw() override;

	void doHit(Mario& mario, Map* map) override;
	std::string getType() const override {
		return "QuestionMushroomBlock";
	}
};

class QuestionFireFlowerBlock : public QuestionBlock {
public:
	QuestionFireFlowerBlock(Vector2 pos, Vector2 dim, Color color);
	QuestionFireFlowerBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~QuestionFireFlowerBlock() override;

	void update() override;
	void draw() override;

	void doHit(Mario& mario, Map* map) override;
	std::string getType() const override {
		return "QuestionFireFlowerBlock";
	}
};

class QuestionStarBlock : public QuestionBlock {
public:
	QuestionStarBlock(Vector2 pos, Vector2 dim, Color color);
	QuestionStarBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~QuestionStarBlock() override;

	void update() override;
	void draw() override;

	void doHit(Mario& mario, Map* map) override;
	std::string getType() const override {
		return "QuestionStarBlock";
	}
};

class QuestionOneUpMushroomBlock : public QuestionBlock {
public:
	QuestionOneUpMushroomBlock(Vector2 pos, Vector2 dim, Color color);
	QuestionOneUpMushroomBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~QuestionOneUpMushroomBlock() override;

	void update() override;
	void draw() override;

	void doHit(Mario& mario, Map* map) override;
	std::string getType() const override {
		return "QuestionOneUpMushroomBlock";
	}
};

class QuestionThreeUpMoonBlock : public QuestionBlock {
public:
	QuestionThreeUpMoonBlock(Vector2 pos, Vector2 dim, Color color);
	QuestionThreeUpMoonBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~QuestionThreeUpMoonBlock() override;

	void update() override;
	void draw() override;

	void doHit(Mario& mario, Map* map) override;
	std::string getType() const override {
		return "QuestionThreeUpMoonBlock";
	}
};

class ExclamationBlock : public Block {
public:
	ExclamationBlock(Vector2 pos, Vector2 dim, Color color);
	ExclamationBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~ExclamationBlock() override;

	void update() override;
	void draw() override;

	void doHit(Mario& mario, Map* map) override;
	std::string getType() const override {
		return "ExclamationBlock";
	}
};

class InvisibleBlock : public Block {
public:
	InvisibleBlock(Vector2 pos, Vector2 dim, Color color);
	InvisibleBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~InvisibleBlock() override;

	void update() override;
	void draw() override;

	void doHit(Mario& mario, Map* map) override;
	std::string getType() const override {
		return "InvisibleBlock";
	}
};

class MessageBlock : public Block {
public:
	MessageBlock(Vector2 pos, Vector2 dim, Color color);
	MessageBlock(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
	~MessageBlock() override;

	void update() override;
	void draw() override;

	void doHit(Mario& mario, Map* map) override;
	std::string getType() const override {
		return "MessageBlock";
	}
};

//Factory Pattern for creating blocks

class BlockFactory {
public:
	static Block* createBlock(const std::string& type, Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames);
};