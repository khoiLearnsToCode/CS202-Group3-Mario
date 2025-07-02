#include "ResourceManager.h"
#include <iostream>

// Initialize static instance pointer
ResourceManager* ResourceManager::instance = nullptr;

// Get singleton instance
ResourceManager& ResourceManager::getInstance() {
    if (instance == nullptr) {
        instance = new ResourceManager();
        std::cout << "ResourceManager instance created." << std::endl;

        // Load resources when the instance is created
        instance->loadResources();
    }
    return *instance;
}

// Cleanup singleton
void ResourceManager::destroyInstance() {
    if (instance != nullptr) {
        instance->unloadResources();
        delete instance;
        instance = nullptr;
    }
}

// Load all resources
void ResourceManager::loadResources() {
    loadTextures();
    loadSounds();
    loadMusics();
}

// Unload all resources
void ResourceManager::unloadResources() {
    unloadTextures();
    unloadSounds();
    unloadMusics();
}
// Implementation of loading textures
void ResourceManager::loadTextures() {

    if (textures.empty()) {

        // load textures...

        //// small mario
        //textures["smallMario0R"] = LoadTexture("resources/images/sprites/mario/SmallMario_0.png");
        //textures["smallMario1R"] = LoadTexture("resources/images/sprites/mario/SmallMario_1.png");
        //textures["smallMario0L"] = texture2DFlipHorizontal(textures["smallMario0R"]);
        //textures["smallMario1L"] = texture2DFlipHorizontal(textures["smallMario1R"]);

        //textures["smallMario0RuR"] = LoadTexture("resources/images/sprites/mario/SmallMarioRunning_0.png");
        //textures["smallMario1RuR"] = LoadTexture("resources/images/sprites/mario/SmallMarioRunning_1.png");
        //textures["smallMario0RuL"] = texture2DFlipHorizontal(textures["smallMario0RuR"]);
        //textures["smallMario1RuL"] = texture2DFlipHorizontal(textures["smallMario1RuR"]);

        //textures["smallMario0JuR"] = LoadTexture("resources/images/sprites/mario/SmallMarioJumping_0.png");
        //textures["smallMario0JuL"] = texture2DFlipHorizontal(textures["smallMario0JuR"]);

        //textures["smallMario0JuRuR"] = LoadTexture("resources/images/sprites/mario/SmallMarioJumpingAndRunning_0.png");
        //textures["smallMario0JuRuL"] = texture2DFlipHorizontal(textures["smallMario0JuRuR"]);

        //textures["smallMario0FaR"] = LoadTexture("resources/images/sprites/mario/SmallMarioFalling_0.png");
        //textures["smallMario0FaL"] = texture2DFlipHorizontal(textures["smallMario0FaR"]);

        //textures["smallMario0LuR"] = LoadTexture("resources/images/sprites/mario/SmallMarioLookingUp_0.png");
        //textures["smallMario0LuL"] = texture2DFlipHorizontal(textures["smallMario0LuR"]);

        //textures["smallMario0DuR"] = LoadTexture("resources/images/sprites/mario/SmallMarioDucking_0.png");
        //textures["smallMario0DuL"] = texture2DFlipHorizontal(textures["smallMario0DuR"]);

        //textures["smallMario0Vic"] = LoadTexture("resources/images/sprites/mario/SmallMarioVictory_0.png");

        //textures["smallMario0Dy"] = LoadTexture("resources/images/sprites/mario/SmallMarioDying_0.png");
        //textures["smallMario1Dy"] = texture2DFlipHorizontal(textures["smallMario0Dy"]);

        //// transitioning mario
        //textures["transitioningMarioSS0R"] = LoadTexture("resources/images/sprites/mario/TransitioningMario_0.png");
        //textures["transitioningMarioSS1R"] = LoadTexture("resources/images/sprites/mario/TransitioningMario_1.png");
        //textures["transitioningMarioSS2R"] = LoadTexture("resources/images/sprites/mario/TransitioningMario_2.png");
        //textures["transitioningMarioSS0L"] = texture2DFlipHorizontal(textures["transitioningMarioSS0R"]);
        //textures["transitioningMarioSS1L"] = texture2DFlipHorizontal(textures["transitioningMarioSS1R"]);
        //textures["transitioningMarioSS2L"] = texture2DFlipHorizontal(textures["transitioningMarioSS2R"]);

        //textures["transitioningMarioSF0R"] = textureColorReplace(textures["transitioningMarioSS0R"], flowerMarioReplacePallete);
        //textures["transitioningMarioSF1R"] = textureColorReplace(textures["transitioningMarioSS1R"], flowerMarioReplacePallete);
        //textures["transitioningMarioSF2R"] = textureColorReplace(textures["transitioningMarioSS2R"], flowerMarioReplacePallete);
        //textures["transitioningMarioSF0L"] = texture2DFlipHorizontal(textures["transitioningMarioSF0R"]);
        //textures["transitioningMarioSF1L"] = texture2DFlipHorizontal(textures["transitioningMarioSF1R"]);
        //textures["transitioningMarioSF2L"] = texture2DFlipHorizontal(textures["transitioningMarioSF2R"]);

        //// super mario
        //textures["superMario0R"] = LoadTexture("resources/images/sprites/mario/SuperMario_0.png");
        //textures["superMario1R"] = LoadTexture("resources/images/sprites/mario/SuperMario_1.png");
        //textures["superMario2R"] = LoadTexture("resources/images/sprites/mario/SuperMario_2.png");
        //textures["superMario0L"] = texture2DFlipHorizontal(textures["superMario0R"]);
        //textures["superMario1L"] = texture2DFlipHorizontal(textures["superMario1R"]);
        //textures["superMario2L"] = texture2DFlipHorizontal(textures["superMario2R"]);

        //textures["superMario0RuR"] = LoadTexture("resources/images/sprites/mario/SuperMarioRunning_0.png");
        //textures["superMario1RuR"] = LoadTexture("resources/images/sprites/mario/SuperMarioRunning_1.png");
        //textures["superMario2RuR"] = LoadTexture("resources/images/sprites/mario/SuperMarioRunning_2.png");
        //textures["superMario0RuL"] = texture2DFlipHorizontal(textures["superMario0RuR"]);
        //textures["superMario1RuL"] = texture2DFlipHorizontal(textures["superMario1RuR"]);
        //textures["superMario2RuL"] = texture2DFlipHorizontal(textures["superMario2RuR"]);

        //textures["superMario0JuR"] = LoadTexture("resources/images/sprites/mario/SuperMarioJumping_0.png");
        //textures["superMario0JuL"] = texture2DFlipHorizontal(textures["superMario0JuR"]);

        //textures["superMario0JuRuR"] = LoadTexture("resources/images/sprites/mario/SuperMarioJumpingAndRunning_0.png");
        //textures["superMario0JuRuL"] = texture2DFlipHorizontal(textures["superMario0JuRuR"]);

        //textures["superMario0FaR"] = LoadTexture("resources/images/sprites/mario/SuperMarioFalling_0.png");
        //textures["superMario0FaL"] = texture2DFlipHorizontal(textures["superMario0FaR"]);

        //textures["superMario0LuR"] = LoadTexture("resources/images/sprites/mario/SuperMarioLookingUp_0.png");
        //textures["superMario0LuL"] = texture2DFlipHorizontal(textures["superMario0LuR"]);

        //textures["superMario0DuR"] = LoadTexture("resources/images/sprites/mario/SuperMarioDucking_0.png");
        //textures["superMario0DuL"] = texture2DFlipHorizontal(textures["superMario0DuR"]);

        //textures["superMario0Vic"] = LoadTexture("resources/images/sprites/mario/SuperMarioVictory_0.png");

        //textures["superMario0TfR"] = LoadTexture("resources/images/sprites/mario/SuperMarioThrowingFireball_0.png");
        //textures["superMario0TfL"] = texture2DFlipHorizontal(textures["superMario0TfR"]);

        // flower mario
        //textures["flowerMario0R"] = textureColorReplace(textures["superMario0R"], flowerMarioReplacePallete);
        //textures["flowerMario1R"] = textureColorReplace(textures["superMario1R"], flowerMarioReplacePallete);
        //textures["flowerMario2R"] = textureColorReplace(textures["superMario2R"], flowerMarioReplacePallete);
        //textures["flowerMario0L"] = texture2DFlipHorizontal(textures["flowerMario0R"]);
        //textures["flowerMario1L"] = texture2DFlipHorizontal(textures["flowerMario1R"]);
        //textures["flowerMario2L"] = texture2DFlipHorizontal(textures["flowerMario2R"]);

        //textures["flowerMario0RuR"] = textureColorReplace(textures["superMario0RuR"], flowerMarioReplacePallete);
        //textures["flowerMario1RuR"] = textureColorReplace(textures["superMario1RuR"], flowerMarioReplacePallete);
        //textures["flowerMario2RuR"] = textureColorReplace(textures["superMario2RuR"], flowerMarioReplacePallete);
        //textures["flowerMario0RuL"] = texture2DFlipHorizontal(textures["flowerMario0RuR"]);
        //textures["flowerMario1RuL"] = texture2DFlipHorizontal(textures["flowerMario1RuR"]);
        //textures["flowerMario2RuL"] = texture2DFlipHorizontal(textures["flowerMario2RuR"]);

        //textures["flowerMario0JuR"] = textureColorReplace(textures["superMario0JuR"], flowerMarioReplacePallete);
        //textures["flowerMario0JuL"] = texture2DFlipHorizontal(textures["flowerMario0JuR"]);

        //textures["flowerMario0JuRuR"] = textureColorReplace(textures["superMario0JuRuR"], flowerMarioReplacePallete);
        //textures["flowerMario0JuRuL"] = texture2DFlipHorizontal(textures["flowerMario0JuRuR"]);

        //textures["flowerMario0FaR"] = textureColorReplace(textures["superMario0FaR"], flowerMarioReplacePallete);
        //textures["flowerMario0FaL"] = texture2DFlipHorizontal(textures["flowerMario0FaR"]);

        //textures["flowerMario0LuR"] = textureColorReplace(textures["superMario0LuR"], flowerMarioReplacePallete);
        //textures["flowerMario0LuL"] = texture2DFlipHorizontal(textures["flowerMario0LuR"]);

        //textures["flowerMario0DuR"] = textureColorReplace(textures["superMario0DuR"], flowerMarioReplacePallete);
        //textures["flowerMario0DuL"] = texture2DFlipHorizontal(textures["flowerMario0DuR"]);

        //textures["flowerMario0Vic"] = textureColorReplace(textures["superMario0Vic"], flowerMarioReplacePallete);

        //textures["flowerMario0TfR"] = textureColorReplace(textures["superMario0TfR"], flowerMarioReplacePallete);
        //textures["flowerMario0TfL"] = texture2DFlipHorizontal(textures["flowerMario0TfR"]);

        //// fireball
        //textures["fireball0R"] = LoadTexture("resources/images/sprites/mario/FlowerMarioFireball_0.png");
        //textures["fireball1R"] = LoadTexture("resources/images/sprites/mario/FlowerMarioFireball_1.png");
        //textures["fireball2R"] = LoadTexture("resources/images/sprites/mario/FlowerMarioFireball_2.png");
        //textures["fireball3R"] = LoadTexture("resources/images/sprites/mario/FlowerMarioFireball_3.png");
        //textures["fireball0L"] = texture2DFlipHorizontal(textures["fireball0R"]);
        //textures["fireball1L"] = texture2DFlipHorizontal(textures["fireball1R"]);
        //textures["fireball2L"] = texture2DFlipHorizontal(textures["fireball2R"]);
        //textures["fireball3L"] = texture2DFlipHorizontal(textures["fireball3R"]);

        // tiles
        for (int i = 1; i <= 87; i++) {
            textures[TextFormat("tile_%d", i)] = LoadTexture(TextFormat("../resource/graphic/tiles/tile_%d.png", i));
        }


        //textures["tileCourseClearPoleBackTop"] = LoadTexture("resources/graphics/tiles/scenario/tile_CourseClearPoleBackTop.png");
        //textures["tileCourseClearPoleBackBody"] = LoadTexture("resources/graphics/tiles/scenario/tile_CourseClearPoleBackBody.png");
        //textures["tileCourseClearPoleFrontTop"] = LoadTexture("resources/graphics/tiles/scenario/tile_CourseClearPoleFrontTop.png");
        //textures["tileCourseClearPoleFrontBody"] = LoadTexture("resources/graphics/tiles/scenario/tile_CourseClearPoleFrontBody.png");

        //// blocks
        //textures["blockCloud"] = LoadTexture("resources/images/sprites/blocks/Cloud_0.png");
        //textures["blockExclamation"] = LoadTexture("resources/images/sprites/blocks/Exclamation_0.png");
        //textures["blockEyesClosed"] = LoadTexture("resources/images/sprites/blocks/EyesClosed_0.png");
        //textures["blockEyesOpened0"] = LoadTexture("resources/images/sprites/blocks/EyesOpened_0.png");
        //textures["blockEyesOpened1"] = LoadTexture("resources/images/sprites/blocks/EyesOpened_1.png");
        //textures["blockEyesOpened2"] = LoadTexture("resources/images/sprites/blocks/EyesOpened_2.png");
        //textures["blockEyesOpened3"] = LoadTexture("resources/images/sprites/blocks/EyesOpened_3.png");
        //textures["blockGlass"] = LoadTexture("resources/images/sprites/blocks/Glass_0.png");
        //textures["blockMessage"] = LoadTexture("resources/images/sprites/blocks/Message_0.png");
        //textures["blockQuestion0"] = LoadTexture("resources/images/sprites/blocks/Question_0.png");
        //textures["blockQuestion1"] = LoadTexture("resources/images/sprites/blocks/Question_1.png");
        //textures["blockQuestion2"] = LoadTexture("resources/images/sprites/blocks/Question_2.png");
        //textures["blockQuestion3"] = LoadTexture("resources/images/sprites/blocks/Question_3.png");
        //textures["blockStone"] = LoadTexture("resources/images/sprites/blocks/Stone_0.png");
        //textures["blockWood"] = LoadTexture("resources/images/sprites/blocks/Wood_0.png");

        // backgrounds
        Image titleScreenImage = LoadImage("../resource/graphic/backgrounds/title_screen.png");
        ImageResize(&titleScreenImage, GetScreenWidth(), GetScreenHeight());
        textures["title_screen"] = LoadTextureFromImage(titleScreenImage);
        UnloadImage(titleScreenImage);
        textures["background1"] = LoadTexture("../resource/graphic/backgrounds/background1.png");
        //textures["background2"] = LoadTexture("resources/graphics/backgrounds/background2.png");
        //textures["background3"] = LoadTexture("resources/graphics/backgrounds/background3.png");
        //textures["background4"] = LoadTexture("resources/graphics/backgrounds/background4.png");
        //textures["background5"] = LoadTexture("resources/graphics/backgrounds/background5.png");
        //textures["background6"] = LoadTexture("resources/graphics/backgrounds/background6.png");
        //textures["background7"] = LoadTexture("resources/graphics/backgrounds/background7.png");
        //textures["background8"] = LoadTexture("resources/graphics/backgrounds/background8.png");
        //textures["background9"] = LoadTexture("resources/graphics/backgrounds/background9.png");
        //textures["background10"] = LoadTexture("resources/graphics/backgrounds/background10.png");

        // items
        //textures["coin0"] = LoadTexture("resources/images/sprites/items/Coin_0.png");
        //textures["coin1"] = LoadTexture("resources/images/sprites/items/Coin_1.png");
        //textures["coin2"] = LoadTexture("resources/images/sprites/items/Coin_2.png");
        //textures["coin3"] = LoadTexture("resources/images/sprites/items/Coin_3.png");
        //textures["1UpMushroom"] = LoadTexture("resources/images/sprites/items/1UpMushroom.png");
        //textures["3UpMoon"] = LoadTexture("resources/images/sprites/items/3UpMoon.png");
        //textures["courseClearToken"] = LoadTexture("resources/images/sprites/items/CourseClearToken.png");
        //textures["fireFlower0"] = LoadTexture("resources/images/sprites/items/FireFlower_0.png");
        //textures["fireFlower1"] = LoadTexture("resources/images/sprites/items/FireFlower_1.png");
        //textures["mushroom"] = LoadTexture("resources/images/sprites/items/Mushroom.png");
        //textures["star"] = LoadTexture("resources/images/sprites/items/Star.png");
        //textures["yoshiCoin0"] = LoadTexture("resources/images/sprites/items/YoshiCoin_0.png");
        //textures["yoshiCoin1"] = LoadTexture("resources/images/sprites/items/YoshiCoin_1.png");
        //textures["yoshiCoin2"] = LoadTexture("resources/images/sprites/items/YoshiCoin_2.png");
        //textures["yoshiCoin3"] = LoadTexture("resources/images/sprites/items/YoshiCoin_3.png");

        //// baddies
        //textures["blueKoopaTroopa0R"] = LoadTexture("resources/images/sprites/baddies/BlueKoopaTroopa_0.png");
        //textures["blueKoopaTroopa1R"] = LoadTexture("resources/images/sprites/baddies/BlueKoopaTroopa_1.png");
        //textures["blueKoopaTroopa0L"] = texture2DFlipHorizontal(textures["blueKoopaTroopa0R"]);
        //textures["blueKoopaTroopa1L"] = texture2DFlipHorizontal(textures["blueKoopaTroopa1R"]);

        //textures["bobOmb0R"] = LoadTexture("resources/images/sprites/baddies/BobOmb_0.png");
        //textures["bobOmb1R"] = LoadTexture("resources/images/sprites/baddies/BobOmb_1.png");
        //textures["bobOmb0L"] = texture2DFlipHorizontal(textures["bobOmb0R"]);
        //textures["bobOmb1L"] = texture2DFlipHorizontal(textures["bobOmb1R"]);

        //textures["bulletBill0R"] = LoadTexture("resources/images/sprites/baddies/BulletBill_0.png");
        //textures["bulletBill0L"] = texture2DFlipHorizontal(textures["bulletBill0R"]);

        //textures["buzzyBeetle0R"] = LoadTexture("resources/images/sprites/baddies/BuzzyBeetle_0.png");
        //textures["buzzyBeetle1R"] = LoadTexture("resources/images/sprites/baddies/BuzzyBeetle_1.png");
        //textures["buzzyBeetle0L"] = texture2DFlipHorizontal(textures["buzzyBeetle0R"]);
        //textures["buzzyBeetle1L"] = texture2DFlipHorizontal(textures["buzzyBeetle1R"]);

        //textures["flyingGoomba0R"] = LoadTexture("resources/images/sprites/baddies/FlyingGoomba_0.png");
        //textures["flyingGoomba1R"] = LoadTexture("resources/images/sprites/baddies/FlyingGoomba_1.png");
        //textures["flyingGoomba2R"] = LoadTexture("resources/images/sprites/baddies/FlyingGoomba_2.png");
        //textures["flyingGoomba3R"] = LoadTexture("resources/images/sprites/baddies/FlyingGoomba_3.png");
        //textures["flyingGoomba0L"] = texture2DFlipHorizontal(textures["flyingGoomba0R"]);
        //textures["flyingGoomba1L"] = texture2DFlipHorizontal(textures["flyingGoomba1R"]);
        //textures["flyingGoomba2L"] = texture2DFlipHorizontal(textures["flyingGoomba2R"]);
        //textures["flyingGoomba3L"] = texture2DFlipHorizontal(textures["flyingGoomba3R"]);

        //textures["goomba0R"] = LoadTexture("resources/images/sprites/baddies/Goomba_0.png");
        //textures["goomba1R"] = LoadTexture("resources/imagesdanss/sprites/baddies/Goomba_1.png");
        //textures["goomba0L"] = texture2DFlipHorizontal(textures["goomba0R"]);
        //textures["goomba1L"] = texture2DFlipHorizontal(textures["goomba1R"]);

        //textures["greenKoopaTroopa0R"] = LoadTexture("resources/images/sprites/baddies/GreenKoopaTroopa_0.png");
        //textures["greenKoopaTroopa1R"] = LoadTexture("resources/images/sprites/baddies/GreenKoopaTroopa_1.png");
        //textures["greenKoopaTroopa0L"] = texture2DFlipHorizontal(textures["greenKoopaTroopa0R"]);
        //textures["greenKoopaTroopa1L"] = texture2DFlipHorizontal(textures["greenKoopaTroopa1R"]);

        //textures["mummyBeetle0R"] = LoadTexture("resources/images/sprites/baddies/MummyBeetle_0.png");
        //textures["mummyBeetle1R"] = LoadTexture("resources/images/sprites/baddies/MummyBeetle_1.png");
        //textures["mummyBeetle0L"] = texture2DFlipHorizontal(textures["mummyBeetle0R"]);
        //textures["mummyBeetle1L"] = texture2DFlipHorizontal(textures["mummyBeetle1R"]);

        //textures["muncher0"] = LoadTexture("resources/images/sprites/baddies/Muncher_0.png");
        //textures["muncher1"] = LoadTexture("resources/images/sprites/baddies/Muncher_1.png");

        //textures["piranhaPlant0"] = LoadTexture("resources/images/sprites/baddies/PiranhaPlant_0.png");
        //textures["piranhaPlant1"] = LoadTexture("resources/images/sprites/baddies/PiranhaPlant_1.png");

        //textures["redKoopaTroopa0R"] = LoadTexture("resources/images/sprites/baddies/RedKoopaTroopa_0.png");
        //textures["redKoopaTroopa1R"] = LoadTexture("resources/images/sprites/baddies/RedKoopaTroopa_1.png");
        //textures["redKoopaTroopa0L"] = texture2DFlipHorizontal(textures["redKoopaTroopa0R"]);
        //textures["redKoopaTroopa1L"] = texture2DFlipHorizontal(textures["redKoopaTroopa1R"]);

        //textures["rex10R"] = LoadTexture("resources/images/sprites/baddies/Rex_1_0.png");
        //textures["rex11R"] = LoadTexture("resources/images/sprites/baddies/Rex_1_1.png");
        //textures["rex20R"] = LoadTexture("resources/images/sprites/baddies/Rex_2_0.png");
        //textures["rex21R"] = LoadTexture("resources/images/sprites/baddies/Rex_2_1.png");
        //textures["rex10L"] = texture2DFlipHorizontal(textures["rex10R"]);
        //textures["rex11L"] = texture2DFlipHorizontal(textures["rex11R"]);
        //textures["rex20L"] = texture2DFlipHorizontal(textures["rex20R"]);
        //textures["rex21L"] = texture2DFlipHorizontal(textures["rex21R"]);

        //textures["swooper0R"] = LoadTexture("resources/images/sprites/baddies/Swooper_1.png");
        //textures["swooper1R"] = LoadTexture("resources/images/sprites/baddies/Swooper_2.png");
        //textures["swooper0L"] = texture2DFlipHorizontal(textures["swooper0R"]);
        //textures["swooper1L"] = texture2DFlipHorizontal(textures["swooper1R"]);

        //textures["yellowKoopaTroopa0R"] = LoadTexture("resources/images/sprites/baddies/YellowKoopaTroopa_0.png");
        //textures["yellowKoopaTroopa1R"] = LoadTexture("resources/images/sprites/baddies/YellowKoopaTroopa_1.png");
        //textures["yellowKoopaTroopa0L"] = texture2DFlipHorizontal(textures["yellowKoopaTroopa0R"]);
        //textures["yellowKoopaTroopa1L"] = texture2DFlipHorizontal(textures["yellowKoopaTroopa1R"]);

        //textures["montyMole0R"] = LoadTexture("resources/images/sprites/baddies/MontyMole_0.png");
        //textures["montyMole1R"] = LoadTexture("resources/images/sprites/baddies/MontyMole_1.png");
        //textures["montyMole0L"] = texture2DFlipHorizontal(textures["montyMole0R"]);
        //textures["montyMole1L"] = texture2DFlipHorizontal(textures["montyMole1R"]);

        //textures["banzaiBill0R"] = LoadTexture("resources/images/sprites/baddies/BanzaiBill_0.png");
        //textures["banzaiBill0L"] = texture2DFlipHorizontal(textures["banzaiBill0R"]);

        //textures["jumpingPiranhaPlant0"] = LoadTexture("resources/images/sprites/baddies/JumpingPiranhaPlant_0.png");
        //textures["jumpingPiranhaPlant1"] = LoadTexture("resources/images/sprites/baddies/JumpingPiranhaPlant_1.png");
        //textures["jumpingPiranhaPlant2"] = LoadTexture("resources/images/sprites/baddies/JumpingPiranhaPlant_2.png");
        //textures["jumpingPiranhaPlant3"] = LoadTexture("resources/images/sprites/baddies/JumpingPiranhaPlant_3.png");

        //// effects
        //textures["puft0"] = LoadTexture("resources/images/sprites/effects/Puft_0.png");
        //textures["puft1"] = LoadTexture("resources/images/sprites/effects/Puft_1.png");
        //textures["puft2"] = LoadTexture("resources/images/sprites/effects/Puft_2.png");
        //textures["puft3"] = LoadTexture("resources/images/sprites/effects/Puft_3.png");

        // textures["stardust0"] = LoadTexture("resources/images/sprites/effects/Stardust_0.png");
        // textures["stardust1"] = LoadTexture("resources/images/sprites/effects/Stardust_1.png");
        // textures["stardust2"] = LoadTexture("resources/images/sprites/effects/Stardust_2.png");
        // textures["stardust3"] = LoadTexture("resources/images/sprites/effects/Stardust_3.png");

        // // gui
        // textures["guiAlfa"] = LoadTexture("resources/images/gui/guiAlfa.png");
        // textures["guiAlfaLowerUpper"] = LoadTexture("resources/images/gui/guiAlfaLowerUpper.png");
        // textures["guiClock"] = LoadTexture("resources/images/gui/guiClock.png");
        // textures["guiCoin"] = LoadTexture("resources/images/gui/guiCoin.png");
        // textures["guiCredits"] = LoadTexture("resources/images/gui/guiCredits.png");
        // textures["guiGameOver"] = LoadTexture("resources/images/gui/guiGameOver.png");
        // textures["guiLetters"] = LoadTexture("resources/images/gui/guiLetters.png");
        // textures["guiMario"] = LoadTexture("resources/images/gui/guiMario.png");
        // textures["guiMarioStart"] = LoadTexture("resources/images/gui/guiMarioStart.png");
        // textures["guiNextItem"] = LoadTexture("resources/images/gui/guiNextItem.png");
        // textures["guiNumbersBig"] = LoadTexture("resources/images/gui/guiNumbersBig.png");
        // textures["guiNumbersWhite"] = LoadTexture("resources/images/gui/guiNumbersWhite.png");
        // textures["guiNumbersYellow"] = LoadTexture("resources/images/gui/guiNumbersYellow.png");
        // textures["guiPunctuation"] = LoadTexture("resources/images/gui/guiPunctuation.png");
        textures["guiRayMarioLogo"] = LoadTexture("../resource/graphic/gui/guiRayMarioLogo.png");
        // textures["guiTime"] = LoadTexture("resources/images/gui/guiTime.png");
        // textures["guiTimeUp"] = LoadTexture("resources/images/gui/guiTimeUp.png");
        // textures["guiX"] = LoadTexture("resources/images/gui/guiX.png");
        // textures["guiPoints10"] = LoadTexture("resources/images/gui/guiTensPoints_10.png");
        // textures["guiPoints20"] = LoadTexture("resources/images/gui/guiTensPoints_20.png");
        // textures["guiPoints40"] = LoadTexture("resources/images/gui/guiTensPoints_40.png");
        // textures["guiPoints80"] = LoadTexture("resources/images/gui/guiTensPoints_80.png");
        // textures["guiPoints100"] = LoadTexture("resources/images/gui/guiHundredsPoints_100.png");
        // textures["guiPoints200"] = LoadTexture("resources/images/gui/guiHundredsPoints_200.png");
        // textures["guiPoints400"] = LoadTexture("resources/images/gui/guiHundredsPoints_400.png");
        // textures["guiPoints800"] = LoadTexture("resources/images/gui/guiHundredsPoints_800.png");
        // textures["guiPoints1000"] = LoadTexture("resources/images/gui/guiThousandsPoints_1000.png");
        // textures["guiPoints2000"] = LoadTexture("resources/images/gui/guiThousandsPoints_2000.png");
        // textures["guiPoints4000"] = LoadTexture("resources/images/gui/guiThousandsPoints_4000.png");
        // textures["guiPoints8000"] = LoadTexture("resources/images/gui/guiThousandsPoints_8000.png");
        // textures["gui1Up"] = LoadTexture("resources/images/gui/gui1Up.png");
        // textures["gui2Up"] = LoadTexture("resources/images/gui/gui2Up.png");
        // textures["gui3Up"] = LoadTexture("resources/images/gui/gui3Up.png");
        textures["StartButton"] = LoadTexture("../resource/graphic/gui/StartButton.png");
        textures["credit"] = LoadTexture("../resource/graphic/gui/credit.png");
        textures["longButtonRelease"] = LoadTexture("../resource/graphic/gui/longButtonRelease.png");
        textures["longButtonPress"] = LoadTexture("../resource/graphic/gui/longButtonPress.png");

        // UI elements
            // Mute buttons
        textures["muteButtonBlue"] = LoadTexture("../resource/graphic/ui/MuteButtonBlue.png");
        textures["muteButtonGreen"] = LoadTexture("../resource/graphic/ui/MuteButtonGreen.png");
        textures["muteButtonOrange"] = LoadTexture("../resource/graphic/ui/MuteButtonOrange.png");
        textures["muteButtonWhite"] = LoadTexture("../resource/graphic/ui/MuteButtonWhite.png");
        textures["muteButtonYellow"] = LoadTexture("../resource/graphic/ui/MuteButtonYellow.png");
        
            // Unmute buttons
        textures["unmuteButtonBlue"] = LoadTexture("../resource/graphic/ui/UnmuteButtonBlue.png");
        textures["unmuteButtonGreen"] = LoadTexture("../resource/graphic/ui/UnmuteButtonGreen.png");
        textures["unmuteButtonOrange"] = LoadTexture("../resource/graphic/ui/UnmuteButtonOrange.png");
        textures["unmuteButtonWhite"] = LoadTexture("../resource/graphic/ui/UnmuteButtonWhite.png");
        textures["unmuteButtonYellow"] = LoadTexture("../resource/graphic/ui/UnmuteButtonYellow.png");
        
            // Save buttons
        textures["saveButtonBlue"] = LoadTexture("../resource/graphic/ui/SaveButtonBlue.png");
        textures["saveButtonGreen"] = LoadTexture("../resource/graphic/ui/SaveButtonGreen.png");
        textures["saveButtonOrange"] = LoadTexture("../resource/graphic/ui/SaveButtonOrange.png");
        textures["saveButtonWhite"] = LoadTexture("../resource/graphic/ui/SaveButtonWhite.png");
        textures["saveButtonYellow"] = LoadTexture("../resource/graphic/ui/SaveButtonYellow.png");
        
            // Setting buttons
        textures["settingButtonBlue"] = LoadTexture("../resource/graphic/ui/SettingButtonBlue.png");
        textures["settingButtonGreen"] = LoadTexture("../resource/graphic/ui/SettingButtonGreen.png");
        textures["settingButtonOrange"] = LoadTexture("../resource/graphic/ui/SettingButtonOrange.png");
        textures["settingButtonWhite"] = LoadTexture("../resource/graphic/ui/SettingButtonWhite.png");
        textures["settingButtonYellow"] = LoadTexture("../resource/graphic/ui/SettingButtonYellow.png");
        
    }
}

// Implementation of loading sounds
void ResourceManager::loadSounds() {
    // Implement later
}

// Implementation of loading musics
void ResourceManager::loadMusics() {
    if (musics.empty()) {
        musics["courseClear"]  = LoadMusicStream( "../resource/audio/musics/courseClear.mp3" );
        musics["ending"] = LoadMusicStream( "../resource/audio/musics/ending.mp3" );
        musics["gameOver"] = LoadMusicStream( "../resource/audio/musics/gameOver.mp3" );
        musics["invincible"] = LoadMusicStream( "../resource/audio/musics/invincible.mp3" );
        musics["music1"] = LoadMusicStream( "../resource/audio/musics/music1.mp3" );
        musics["music2"] = LoadMusicStream( "../resource/audio/musics/music2.mp3" );
        musics["music3"] = LoadMusicStream( "../resource/audio/musics/music3.mp3" );
        musics["music4"] = LoadMusicStream( "../resource/audio/musics/music4.mp3" );
        musics["music5"] = LoadMusicStream( "../resource/audio/musics/music5.mp3" );
        musics["music6"] = LoadMusicStream( "../resource/audio/musics/music6.mp3" );
        musics["music7"] = LoadMusicStream( "../resource/audio/musics/music7.mp3" );
        musics["music8"] = LoadMusicStream( "../resource/audio/musics/music8.mp3" );
        musics["music9"] = LoadMusicStream( "../resource/audio/musics/music9.mp3" );
        musics["playerDown"] = LoadMusicStream( "../resource/audio/musics/playerDown.mp3" );
        musics["title"] = LoadMusicStream( "../resource/audio/musics/title.mp3" );
    }
}

void ResourceManager::unloadTextures() {
    for (auto& texture : textures) {
        UnloadTexture(texture.second);
    }
    textures.clear();
}

void ResourceManager::unloadSounds() {
    for (auto& sound : sounds) {
        UnloadSound(sound.second);
    }
    sounds.clear();
}

void ResourceManager::unloadMusics() {
    for (auto& music : musics) {
        UnloadMusicStream(music.second);
    }
    musics.clear();
}

// Get resource maps
std::map<std::string, Texture2D>& ResourceManager::getTextures() {
    return textures;
}

std::map<std::string, Sound>& ResourceManager::getSounds() {
    return sounds;
}

std::map<std::string, Music>& ResourceManager::getMusics() {
    return musics;
}

//Utility getters
Texture2D& ResourceManager::getTexture(const std::string& key) {
    if (textures.find(key) == textures.end()) {
        throw std::runtime_error("Texture not found: " + key);
    }
    return textures[key];
}

Sound& ResourceManager::getSound(const std::string& key) {
    if (sounds.find(key) == sounds.end()) {
        throw std::runtime_error("Sound not found: " + key);
    }
    return sounds[key];
}

Music& ResourceManager::getMusic(const std::string& key) {
    if (musics.find(key) == musics.end()) {
        throw std::runtime_error("Music not found: " + key);
    }
    return musics[key];
}