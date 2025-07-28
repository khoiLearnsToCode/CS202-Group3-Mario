#include "LeaderBoardScreen.h"

LeaderBoardScreen::LeaderBoardScreen() : Screen(), 
                                         font(ResourceManager::getInstance().getFont("SuperMario256")),
                                         fontSize(20.0f), isLatestDataLoaded(false) {

    Image img = LoadImageFromTexture(textures["settingBackground"]);
    ImageResize(&img, img.width * 0.5f, img.height * 0.5f);
    backgroundTexture = LoadTextureFromImage(img);
    UnloadImage(img);

    Image leaderboardLogoImage = LoadImageFromTexture(textures["leaderboard"]);
    ImageResize(&leaderboardLogoImage, leaderboardLogoImage.width * 0.25f, leaderboardLogoImage.height * 0.25f);
    leaderboardLogo = LoadTextureFromImage(leaderboardLogoImage);
    UnloadImage(leaderboardLogoImage);

    border = { (GetScreenWidth() - backgroundTexture.width) / 2.0f, 
               (GetScreenHeight() - backgroundTexture.height) / 2.0f, 
               (float)backgroundTexture.width, 
               (float)backgroundTexture.height };   
}

LeaderBoardScreen::~LeaderBoardScreen() {
    UnloadTexture(backgroundTexture);
    UnloadTexture(leaderboardLogo);
}

void LeaderBoardScreen::draw() {}

bool LeaderBoardScreen::leaderBoardShouldClose() const {
    Vector2 mousePos = GetMousePosition();
    return (!CheckCollisionPointRec(mousePos, border)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void LeaderBoardScreen::setLeaderboardDataAsStrings(const std::vector<std::string>& data) {
    if (isLatestDataLoaded){
        return;
    }
    leaderboardDataAsStrings.clear();
    for (const auto& entry : data) {
        leaderboardDataAsStrings.push_back(entry);
    }
    isLatestDataLoaded = true;
}

void LeaderBoardScreen::setLatestDataLoaded(bool isLoaded) {
    isLatestDataLoaded = isLoaded;
}

