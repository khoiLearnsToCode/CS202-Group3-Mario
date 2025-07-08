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