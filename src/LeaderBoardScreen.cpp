#include "LeaderBoardScreen.h"

LeaderBoardScreen::LeaderBoardScreen() : Screen(), 
                                         font(ResourceManager::getInstance().getFont("SuperMario256")),
                                         fontSize(20.0f), isLatestDataLoaded(false){

    Image img = LoadImageFromTexture(textures["leaderboardScreen"]);
    //ImageResize(&img, img.width, img.height);
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
    returnButton = new ButtonTextTexture("returnButton", { 125.0f, 800.0f }, 2.0f);
}

LeaderBoardScreen::~LeaderBoardScreen() {
    UnloadTexture(backgroundTexture);
    UnloadTexture(leaderboardLogo);
	delete returnButton;
}

void LeaderBoardScreen::draw() {
    DrawTexture(backgroundTexture, (GetScreenWidth() - backgroundTexture.width) / 2.0f,
        (GetScreenHeight() - backgroundTexture.height) / 2.0f, WHITE);
    DrawTexture(leaderboardLogo, (GetScreenWidth() - leaderboardLogo.width) / 2.0f, 120, WHITE);
    int numCols = 7;
    float tableWidth = GetScreenWidth() * 0.85f;
    float tableStartX = (GetScreenWidth() - tableWidth) / 2.0f;
    float tableStartY = 220.0f;
    float colWidth = tableWidth / numCols;
    const char* headers[] = { "RANK", "DATE", "TIME", "LIVE", "YOSHICOIN", "COIN", "POINT" };

    // Vẽ header
    for (int i = 0; i < numCols; ++i) {
        float x = tableStartX + i * colWidth;
        DrawTextEx(font, headers[i], { x, tableStartY }, fontSize, 2.0f, BLACK);
    }

    int maxRow = 5;
    int rowCount = std::min(maxRow, (int)leaderboardDataAsStrings.size() / 6);
    float tableHeight = GetScreenHeight() * 0.45f;
    float rowSpacing = tableHeight / maxRow;
    for (int row = 0; row < rowCount; ++row) {
        float y = tableStartY + 40.0f + row * rowSpacing;
        std::string rawDate = leaderboardDataAsStrings[row * 6 + 5];
        std::string shortDate = rawDate;
        size_t pos = rawDate.find(' ');
        if (pos != std::string::npos && rawDate.length() >= pos + 10) {
            shortDate = rawDate.substr(pos + 1, 10);
        }
        DrawTextEx(font, std::to_string(row + 1).c_str(), { tableStartX + 0 * colWidth, y }, fontSize, 2.0f, BLACK); // Rank
        DrawTextEx(font, shortDate.c_str(), { tableStartX + 1 * colWidth, y }, fontSize, 2.0f, BLACK); // Date
        DrawTextEx(font, leaderboardDataAsStrings[row * 6 + 4].c_str(), { tableStartX + 2 * colWidth, y }, fontSize, 2.0f, BLACK); // Time
        DrawTextEx(font, leaderboardDataAsStrings[row * 6 + 1].c_str(), { tableStartX + 3 * colWidth, y }, fontSize, 2.0f, BLACK); // Live
        DrawTextEx(font, leaderboardDataAsStrings[row * 6 + 3].c_str(), { tableStartX + 4 * colWidth, y }, fontSize, 2.0f, BLACK); // YoshiCoin
        DrawTextEx(font, leaderboardDataAsStrings[row * 6 + 2].c_str(), { tableStartX + 5 * colWidth, y }, fontSize, 2.0f, BLACK); // Coin
        DrawTextEx(font, leaderboardDataAsStrings[row * 6 + 0].c_str(), { tableStartX + 6 * colWidth, y }, fontSize, 2.0f, BLACK); // Point
    }
    returnButton->draw();
}

bool LeaderBoardScreen::leaderBoardShouldClose() const {
    Vector2 mousePos = GetMousePosition();
    return (!CheckCollisionPointRec(mousePos, border)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void LeaderBoardScreen::setLeaderboardDataAsStrings(const std::vector<std::string>& data) {
    //if (isLatestDataLoaded){
    //    return;
    //}
    leaderboardDataAsStrings.clear();
    for (const auto& entry : data) {
        leaderboardDataAsStrings.push_back(entry);
    }
    isLatestDataLoaded = true;
}

void LeaderBoardScreen::setLatestDataLoaded(bool isLoaded) {
    isLatestDataLoaded = isLoaded;
}

Button* LeaderBoardScreen::getReturnButton() const {
    return returnButton;
}
