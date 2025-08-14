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

void LeaderBoardScreen::draw() {
    DrawTexture(backgroundTexture, (GetScreenWidth() - backgroundTexture.width) / 2.0f,
        (GetScreenHeight() - backgroundTexture.height) / 2.0f, WHITE);
	const char* title = "Leaderboard";
    Vector2 titleSize = MeasureTextEx(font, title, fontSize * 1.5f, 2.0f);
    DrawTextEx(font, title, { (GetScreenWidth() - titleSize.x) / 2.0f, 150.0f },
        fontSize * 1.5f, 2.0f, BLACK);
    float y = 200.0f;
    if (leaderboardDataAsStrings.empty()) {
		const char* noData = "No data available";
        Vector2 noDataSize = MeasureTextEx(font, noData, fontSize, 2.0f);
        DrawTextEx(font, noData, { (GetScreenWidth() - noDataSize.x) / 2.0f, y },
            fontSize, 2.0f, WHITE);
    }
    else {
        for (size_t i = 0; i < leaderboardDataAsStrings.size(); i += 6) {
            std::string entry = "Rank " + std::to_string(i / 6 + 1) + ": Point: " + leaderboardDataAsStrings[i] +
                ", Lives: " + leaderboardDataAsStrings[i + 1] + ", Coin: " + leaderboardDataAsStrings[i + 2] +
                ", YoshiCoin: " + leaderboardDataAsStrings[i + 3] + ", Time: " +
                leaderboardDataAsStrings[i + 4] + ", Date: " + leaderboardDataAsStrings[i + 5];
            Vector2 entrySize = MeasureTextEx(font, entry.c_str(), fontSize, 2.0f);
            DrawTextEx(font, entry.c_str(), { (GetScreenWidth() - entrySize.x) / 2.0f, y },
                fontSize, 2.0f, WHITE);
            y += fontSize + 10.0f;
        }
    }
}

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

