class GameWindow {

    int width;
    int height;
    std::string title;
    int targetFPS;
    bool initAudio;

    GameWorld gw;
    Camera2D camera;
    bool initialized;

public:

    GameWindow();
    GameWindow(int width, int height, std::string title);
    ~GameWindow();

    void init();

    // Essential getters only
    int getWidth() const;
    int getHeight() const;
    GameWorld& getGameWorld() { return gw; }
    Camera2D& getCamera() { return camera; }

    // Essential setters only
    void setTargetFPS(int fps);
};