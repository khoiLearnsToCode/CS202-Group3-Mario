#pragma once

#include "Screen.h"
#include "raylib.h"
#include "ResourceManager.h"
#include "Button.h"


class LeaderBoardScreen : public Screen {
public:
    LeaderBoardScreen();
    LeaderBoardScreen(const LeaderBoardScreen&) = delete;
    LeaderBoardScreen& operator=(const LeaderBoardScreen&) = delete;
    ~LeaderBoardScreen() override;

    void draw() override;
    bool leaderBoardShouldClose() const;
    void setLatestDataLoaded(bool isLoaded);

    friend class CareTaker;

    Button* getReturnButton() const;

private:
    Texture2D backgroundTexture;
    Texture2D leaderboardLogo;
    Rectangle border;
    Font& font;
    float fontSize;
    bool isLatestDataLoaded;
    std::vector<std::string> leaderboardDataAsStrings;

    void setLeaderboardDataAsStrings(const std::vector<std::string>& data);

    Button* returnButton;
};
