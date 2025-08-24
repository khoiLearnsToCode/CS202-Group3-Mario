#include "GameWorld.h"
#include "Item.h"
#include "utils.h"
#include "TitleScreen.h"
#include "MenuScreen.h"
#include "MapEditorScreen1.h"
#include "MapEditorScreen2.h"
#include"LoadGame.h"

GameState GameWorld::state = GAME_STATE_TITLE_SCREEN;
float GameWorld::gravity = 1200;

GameWorld::GameWorld() :
    player( 
        {0,0},          // position
        {32, 40},       // dimention
        {0, 0},         // velocity
        {0, 0, 0, 255}, // color
        260,            // speedX
        360,            // maxSpeedX
        -600,           // jumpSpeed
        false           
    ),

    map(player, 1, true, this),
    camera(nullptr),
    settingBoardIsOpen(false),
    helpingBoardIsOpen(false),
    stateBeforePause(GAME_STATE_TITLE_SCREEN),
    remainingTimePointCount(0),
    totalPlayedTime(0),
    pauseMusic(false),
    pausePlayer(false),
    outroFinished(false),
    outroTime(1.0f),
    outroAcum(0.0f),
    settingButton(nullptr),
    helpButton(nullptr),
    pauseButtonsCooldownAcum(0.0f),
    pauseButtonsCooldownTime(0.5f),
    maxDistForCollisionCheck(1000.0f),
    titleScreen(nullptr),
    menuScreen(nullptr),
    mapEditorScreen1(nullptr),
    mapEditorScreen2(nullptr),
    selectCharacterScreen(nullptr),
    settingScreen(nullptr),
    helpingScreen(nullptr),
    guardScreen(nullptr),
	lastCheckpointData(1,5,0,0,PLAYER_TYPE_SMALL,false),
    leaderBoardScreen(nullptr)
    {
        player.setGameWorld(this);
        player.setMap(&map);
    }

GameWorld::~GameWorld() {
    if (titleScreen != nullptr) {
        delete titleScreen;
        titleScreen = nullptr;
    }

    if (menuScreen != nullptr) {
        delete menuScreen;
        menuScreen = nullptr;
    }

    if (mapEditorScreen1 != nullptr) {
        delete mapEditorScreen1;
        mapEditorScreen1 = nullptr;
    }
    
    if (mapEditorScreen2 != nullptr) {
        delete mapEditorScreen2;
        mapEditorScreen2 = nullptr;
    }

    if (selectCharacterScreen != nullptr) {
        delete selectCharacterScreen;
        selectCharacterScreen = nullptr;
    }

    if (settingScreen != nullptr) {
        delete settingScreen;
        settingScreen = nullptr;
    }

    if (helpingScreen != nullptr) {
        delete helpingScreen;
        helpingScreen = nullptr;
    }

    if (guardScreen != nullptr) {
        delete guardScreen;
        guardScreen = nullptr;
    }

    if (leaderBoardScreen != nullptr) {
        delete leaderBoardScreen;
        leaderBoardScreen = nullptr;
    }

    if (settingButton != nullptr) {
        delete settingButton;
        settingButton = nullptr;
    }

    if (helpButton != nullptr) {
        delete helpButton;
        helpButton = nullptr;
    }
    
    delete careTaker;
}
savedData* GameWorld::dataFromGameWorldToLoad()
{
    return &lastCheckpointData;
}
void GameWorld::dataFromLoadToGameWorld(savedData* data)
{
    player.setType(data->pT);
    player.setLives(data->lives);
    player.setCoins(data->coins);
    player.setPoints(data->score);
    player.setMaxTime(400.0f);
    if (data->isLuigi) player.toLuigi();
    if (data->mapID == 1) map.first();
    else if(data->mapID==2) map.second();
	else if (data->mapID == 3) map.third();
    player.reset(false, true);

    map.reset();
    state = GAME_STATE_PLAYING;
}

Memento* GameWorld::dataFromGameWorldToLeaderboard() {
    Data data(
        map.getId(),
        player.getPoints(),
        player.getLives(),
        player.getCoins(),
        player.getYoshiCoins(),
        player.getType()
    );
    return new ConcreteMemento(data);
}

void GameWorld::initScreensAndButtons() {
    if (titleScreen == nullptr) {
        titleScreen = new TitleScreen();
    }

    if (menuScreen == nullptr) {
        menuScreen = new MenuScreen();
    }

    if (leaderBoardScreen == nullptr) {
        leaderBoardScreen = new LeaderBoardScreen();
		//leaderBoardScreen->initData(); // Initialize leaderboard data
    }

    if (mapEditorScreen1 == nullptr) {
        mapEditorScreen1 = new MapEditorScreen1();
    }
    
    if (mapEditorScreen2 == nullptr) {
        mapEditorScreen2 = new MapEditorScreen2(mapEditorScreen1);
    }

    if (selectCharacterScreen == nullptr) {
        selectCharacterScreen = new SelectCharacterScreen();
    }

    if (settingScreen == nullptr) {
        settingScreen = new SettingScreen(this);
    }

    if (helpingScreen == nullptr) {
        helpingScreen = new HelpingScreen();
    }

    if (guardScreen == nullptr) {
        guardScreen = new GuardScreen();
    }

    if (settingButton == nullptr) {
        settingButton = new ButtonTextTexture("settingButton", { GetScreenWidth() - 80.0f, 20.0f }, 2.0f);
    }

    if (helpButton == nullptr) {
        helpButton = new ButtonTextTexture("helpButton", { GetScreenWidth() - 150.0f, 20.0f }, 2.0f);
    }
}

void GameWorld::inputAndUpdate() {

    map.loadFromJsonFile(); 

    // OPTIMIZATION: Cache resource references to avoid repeated lookups
    static std::map<std::string, Sound>& sounds = ResourceManager::getInstance().getSounds();
    static std::map<std::string, Music>& musics = ResourceManager::getInstance().getMusics();

    // OPTIMIZATION: Cache vector references
    std::vector<Tile*>& Tiles = map.getTiles();
    std::vector<Block*>& Blocks = map.getBlocks();
    std::vector<Baddie*>& Baddies = map.getBaddies();
    std::vector<Item*>& Items = map.getItems();
    std::vector<Item*>& StaticItems = map.getStaticItems();


    // std::cerr << "Player state: " << player.getState() << std::endl;
    // std::cerr << "GameWorld state: " << state << std::endl;
    // std::cerr << "gravity: " << gravity << std::endl;
    if ( player.getState() != SPRITE_STATE_DYING && 
         player.getState() != SPRITE_STATE_VICTORY &&
         player.getState() != SPRITE_STATE_WAITING_TO_NEXT_MAP &&
         state != GAME_STATE_TITLE_SCREEN &&
         state != GAME_STATE_MENU_SCREEN &&
         state != GAME_STATE_MAP_EDITOR_SCREEN1 &&
         state != GAME_STATE_MAP_EDITOR_SCREEN2 &&
         state != GAME_STATE_SELECT_CHARACTER_SCREEN &&
         state != GAME_STATE_CREDITS_SCREEN &&
         state != GAME_STATE_LEADERBOARD_SCREEN &&
         state != GAME_STATE_LOADGAME_SCREEN &&
         state != GAME_STATE_FINISHED &&
         state != GAME_STATE_GUARD_SCREEN && 
         !pauseMusic ) {
        map.playMusic();
    }

    if ( state != GAME_STATE_TITLE_SCREEN &&
         state != GAME_STATE_MENU_SCREEN &&
         state != GAME_STATE_SELECT_CHARACTER_SCREEN &&
         state != GAME_STATE_CREDITS_SCREEN &&
         state != GAME_STATE_MAP_EDITOR_SCREEN1 &&
         state != GAME_STATE_MAP_EDITOR_SCREEN2 &&
         state != GAME_STATE_FINISHED &&
         state != GAME_STATE_SETTINGS_SCREEN &&
         state != GAME_STATE_LOADGAME_SCREEN &&
         state != GAME_STATE_LEADERBOARD_SCREEN &&
         state != GAME_STATE_HELPING_SCREEN &&
         state != GAME_STATE_GUARD_SCREEN ) {
        player.setActivationWidth( GetScreenWidth() * 2 );
        player.update();
    } else if ( !pausePlayer && state != GAME_STATE_TITLE_SCREEN && 
        state != GAME_STATE_MENU_SCREEN &&
        state != GAME_STATE_MAP_EDITOR_SCREEN1 &&
        state != GAME_STATE_MAP_EDITOR_SCREEN2 &&
        state != GAME_STATE_SELECT_CHARACTER_SCREEN &&
        state != GAME_STATE_CREDITS_SCREEN &&
        state != GAME_STATE_LEADERBOARD_SCREEN &&
        state != GAME_STATE_LOADGAME_SCREEN ) {

        player.update();
    }

    if ( player.getState() != SPRITE_STATE_DYING && 
         player.getState() != SPRITE_STATE_VICTORY &&
         player.getState() != SPRITE_STATE_WAITING_TO_NEXT_MAP &&
         state != GAME_STATE_TITLE_SCREEN &&
         state != GAME_STATE_MENU_SCREEN &&
         state != GAME_STATE_MAP_EDITOR_SCREEN1 &&
         state != GAME_STATE_MAP_EDITOR_SCREEN2 &&
         state != GAME_STATE_SELECT_CHARACTER_SCREEN &&
         state != GAME_STATE_CREDITS_SCREEN &&
         state != GAME_STATE_FINISHED &&
         state != GAME_STATE_SETTINGS_SCREEN &&
         state != GAME_STATE_LEADERBOARD_SCREEN &&
         state != GAME_STATE_LOADGAME_SCREEN &&
         state != GAME_STATE_HELPING_SCREEN &&
         state != GAME_STATE_GUARD_SCREEN ) {

            std::vector<int> collectedIndexes;

        // Update setting button cooldown
        if (pauseButtonsCooldownAcum > 0.0f) {
            pauseButtonsCooldownAcum -= GetFrameTime();
            if (pauseButtonsCooldownAcum < 0.0f) {
                pauseButtonsCooldownAcum = 0.0f;
            }
        }

        if ( (IsKeyPressed( KEY_P) || settingButton->isReleased()) && pauseButtonsCooldownAcum <= 0.0f ) {
            pauseGame( true, true, true, true, false );
            settingScreen->setSettingBoardIsOpenInMenuScreen(false);
        }

        if ( (IsKeyPressed( KEY_H) || helpButton->isReleased()) && pauseButtonsCooldownAcum <= 0.0f) {
            pauseGame( true, true, true, false, true );
        }
        

        // OPTIMIZATION: Update collision probes once per entity per frame, not multiple times
        for ( const auto& block : Blocks ) {
            block->update();
        }

        for ( const auto& item : Items ) {
            item->update();
        }

        for ( const auto& staticItem : StaticItems ) {
            staticItem->update();
        }

        for ( const auto& baddie : Baddies ) {
            baddie->update();
            baddie->followTheLeader( &player );
            // OPTIMIZATION: Update collision probes once per baddie
            baddie->updateCollisionProbes();
        }

        // OPTIMIZATION: Update item collision probes once
        for ( const auto& item : Items ) {
            item->updateCollisionProbes();
        }

        // tiles collision resolution
        player.updateCollisionProbes();
        for ( const auto& tile : Tiles ) {

            // player x tiles - Distance-based collision optimization
            if (shouldCheckCollision(player.getPos(), player.getDim(), tile->getPos(), tile->getDim(), maxDistForCollisionCheck)) {
                CollisionType col = player.checkCollision( tile );

                if ( tile->getType() == TILE_TYPE_SOLID ) {
                    switch ( col ) {
                        case COLLISION_TYPE_NORTH:
                            player.setY( tile->getY() + tile->getHeight() );
                            player.setVelY( 0 );
                            player.updateCollisionProbes();
                            break;
                        case COLLISION_TYPE_SOUTH:
                            player.setY( tile->getY() - player.getHeight() );
                            player.setVelY( 0 );
                            player.setState( SPRITE_STATE_ON_GROUND );
                            player.updateCollisionProbes();
                            break;
                        case COLLISION_TYPE_EAST:
                            player.setX( tile->getX() - player.getWidth() );
                            player.setVelX( 0 );
                            player.updateCollisionProbes();
                            break;
                        case COLLISION_TYPE_WEST:
                            player.setX( tile->getX() + tile->getWidth() );
                            player.setVelX( 0 );
                            player.updateCollisionProbes();
                            break;
                        default:
                            break;
                    }
                } else if ( tile->getType() == TILE_TYPE_SOLID_FROM_ABOVE ) {
                    if ( col == COLLISION_TYPE_SOUTH && player.getState() == SPRITE_STATE_FALLING ) {
                        player.setY( tile->getY() - player.getHeight() );
                        player.setVelY( 0 );
                        player.setState( SPRITE_STATE_ON_GROUND );
                        player.updateCollisionProbes();
                    }
                }
            }

            // baddies x tiles - OPTIMIZATION: Skip redundant updateCollisionProbes + Distance check
            for ( const auto& baddie : Baddies ) {

                if ( baddie->getState() != SPRITE_STATE_DYING && 
                     baddie->getState() != SPRITE_STATE_TO_BE_REMOVED &&
                     ( tile->getType() == TILE_TYPE_SOLID || 
                       tile->getType() == TILE_TYPE_SOLID_ONLY_FOR_BADDIES ||
                       tile->getType() == TILE_TYPE_SOLID_FROM_ABOVE ) ) {
                    
                    // Distance-based collision optimization: Only check collision if objects are nearby
                    if (shouldCheckCollision(baddie->getPos(), baddie->getDim(), tile->getPos(), tile->getDim(), maxDistForCollisionCheck)) {
                        switch ( baddie->checkCollision( tile ) ) {
                            case COLLISION_TYPE_NORTH:
                                baddie->setY( tile->getY() + tile->getHeight() );
                                baddie->setVelY( 0 );
                                baddie->onNorthCollision();
                                baddie->updateCollisionProbes();
                                break;
                            case COLLISION_TYPE_SOUTH:
                                baddie->setY( tile->getY() - baddie->getHeight() );
                                baddie->setVelY( 0 );
                                baddie->onSouthCollision();
                                baddie->updateCollisionProbes();
                                break;
                            case COLLISION_TYPE_EAST:
                                baddie->setX( tile->getX() - baddie->getWidth() );
                                baddie->setVelX( -baddie->getVelX() );
                                baddie->updateCollisionProbes();
                                break;
                            case COLLISION_TYPE_WEST:
                                baddie->setX( tile->getX() + tile->getWidth() );
                                baddie->setVelX( -baddie->getVelX() );
                                baddie->updateCollisionProbes();
                                break;
                            default:
                                break;
                        }
                    }
                }

            }

            // items x tiles - OPTIMIZATION: Skip redundant updateCollisionProbes + Distance check
            for ( const auto& item : Items ) {

                if ( tile->getType() == TILE_TYPE_SOLID || tile->getType() == TILE_TYPE_SOLID_FROM_ABOVE ) {

                    // Distance-based collision optimization: Only check collision if objects are nearby
                    if (shouldCheckCollision(item->getPos(), item->getDim(), tile->getPos(), tile->getDim(), maxDistForCollisionCheck)) {
                        switch ( item->checkCollision( tile ) ) {
                            case COLLISION_TYPE_NORTH:
                                item->setY( tile->getY() + tile->getHeight() );
                                item->setVelY( 0 );
                                item->updateCollisionProbes();
                                break;
                            case COLLISION_TYPE_SOUTH:
                                item->setY( tile->getY() - item->getHeight() );
                                item->setVelY( 0 );
                                item->onSouthCollision( player );
                                item->updateCollisionProbes();
                                break;
                            case COLLISION_TYPE_EAST:
                                item->setX( tile->getX() - item->getWidth() );
                                item->setVelX( -item->getVelX() );
                                item->updateCollisionProbes();
                                break;
                            case COLLISION_TYPE_WEST:
                                item->setX( tile->getX() + tile->getWidth() );
                                item->setVelX( -item->getVelX() );
                                item->updateCollisionProbes();
                                break;
                            default:
                                break;
                        }
                    }

                }

            }

        }

        // blocks collision resolution
        player.updateCollisionProbes();

        for ( const auto& block : Blocks ) {

            // player x blocks - Distance-based collision optimization
            if (shouldCheckCollision(player.getPos(), player.getDim(), block->getPos(), block->getDim(), maxDistForCollisionCheck)) {
                switch ( player.checkCollision( block ) ) {
                    case COLLISION_TYPE_NORTH:
                        player.setY( block->getY() + block->getHeight() );
                        player.setVelY( 0 );
                        player.updateCollisionProbes();
                        block->doHit( player, &map );
                        break;
                    case COLLISION_TYPE_SOUTH:
                        player.setY( block->getY() - player.getHeight() );
                        player.setVelY( 0 );
                        player.setState( SPRITE_STATE_ON_GROUND );
                        player.updateCollisionProbes();
                        break;
                    case COLLISION_TYPE_EAST:
                        player.setX( block->getX() - player.getWidth() );
                        player.setVelX( 0 );
                        player.updateCollisionProbes();
                        break;
                    case COLLISION_TYPE_WEST:
                        player.setX( block->getX() + block->getWidth() );
                        player.setVelX( 0 );
                        player.updateCollisionProbes();
                        break;
                    default:
                        break;
                }
            }

            // baddies x blocks - OPTIMIZATION: Skip redundant updateCollisionProbes + Distance check
            for ( const auto& baddie : Baddies ) {

                if ( baddie->getState() != SPRITE_STATE_DYING && 
                     baddie->getState() != SPRITE_STATE_TO_BE_REMOVED ) {
                    
                    // Distance-based collision optimization: Only check collision if objects are nearby
                    if (shouldCheckCollision(baddie->getPos(), baddie->getDim(), block->getPos(), block->getDim(), maxDistForCollisionCheck)) {
                        switch ( baddie->checkCollision( block ) ) {
                            case COLLISION_TYPE_NORTH:
                                baddie->setY( block->getY() + block->getHeight() );
                                baddie->setVelY( 0 );
                                baddie->onNorthCollision();
                                baddie->updateCollisionProbes();
                                break;
                            case COLLISION_TYPE_SOUTH:
                                baddie->setY( block->getY() - baddie->getHeight() );
                                baddie->setVelY( 0 );
                                baddie->onSouthCollision();
                                baddie->updateCollisionProbes();
                                break;
                            case COLLISION_TYPE_EAST:
                                baddie->setX( block->getX() - baddie->getWidth() );
                                baddie->setVelX( -baddie->getVelX() );
                                baddie->updateCollisionProbes();
                                break;
                            case COLLISION_TYPE_WEST:
                                baddie->setX( block->getX() + block->getWidth() );
                                baddie->setVelX( -baddie->getVelX() );
                                baddie->updateCollisionProbes();
                                break;
                            default:
                                break;
                        }
                    }
                }

            }

            // items x blocks - OPTIMIZATION: Skip redundant updateCollisionProbes + Distance check  
            for ( const auto& item : Items ) {

                // Distance-based collision optimization: Only check collision if objects are nearby
                if (shouldCheckCollision(item->getPos(), item->getDim(), block->getPos(), block->getDim(), maxDistForCollisionCheck)) {
                    switch ( item->checkCollision( block ) ) {
                        case COLLISION_TYPE_NORTH:
                            item->setY( block->getY() + block->getHeight() );
                            item->setVelY( 0 );
                            item->updateCollisionProbes();
                            break;
                        case COLLISION_TYPE_SOUTH:
                            item->setY( block->getY() - item->getHeight() );
                            item->setVelY( 0 );
                            item->onSouthCollision( player );
                            item->updateCollisionProbes();
                            break;
                        case COLLISION_TYPE_EAST:
                            item->setX( block->getX() - item->getWidth() );
                            item->setVelX( -item->getVelX() );
                            item->updateCollisionProbes();
                            break;
                        case COLLISION_TYPE_WEST:
                            item->setX( block->getX() + block->getWidth() );
                            item->setVelX( -item->getVelX() );
                            item->updateCollisionProbes();
                            break;
                        default:
                            break;
                    }
                }

            }

        }

        // player x items collision resolution and offscreen items removal with distance optimization
        for ( size_t i = 0; i < Items.size(); i++ ) {

            Item* item = Items[i];

            if ( item->getState() != SPRITE_STATE_HIT &&
                 item->getState() != SPRITE_STATE_TO_BE_REMOVED ) {
                
                // Distance-based collision optimization: Only check collision if Player is close to the item
                if (shouldCheckCollision(player.getPos(), player.getDim(), item->getPos(), item->getDim(), maxDistForCollisionCheck)) {
                    if ( item->checkCollision( &player ) != COLLISION_TYPE_NONE ) {
                        if ( !player.isTransitioning() ) {
                            item->setState( SPRITE_STATE_HIT );
                            item->playCollisionSound();
                            if ( item->isPauseGameOnHit() ) {
                                pauseGame( false, false, false, false, false );
                            }
                            item->updatePlayer( player );
                        }
                    }
                } else if ( item->getY() > map.getMaxHeight() ) {
                    item->setState( SPRITE_STATE_TO_BE_REMOVED );
                }
            }

            if ( item->getState() == SPRITE_STATE_TO_BE_REMOVED ) {
                collectedIndexes.push_back( static_cast<int>( i ) );
            }

        }

        for ( int i = collectedIndexes.size() - 1; i >= 0; i-- ) {
            delete Items[collectedIndexes[i]];
            Items.erase( Items.begin() + collectedIndexes[i] );
        }
        
        // player x static items collision resolution with distance optimization
        collectedIndexes.clear();
        for ( size_t i = 0; i < StaticItems.size(); i++ ) {

            Item* item = StaticItems[i];

            if ( item->getState() != SPRITE_STATE_HIT &&
                 item->getState() != SPRITE_STATE_TO_BE_REMOVED ) {
                
                // Distance-based collision optimization: Only check collision if Player is close to the static item
                if (shouldCheckCollision(player.getPos(), player.getDim(), item->getPos(), item->getDim(), maxDistForCollisionCheck)) {
                    if ( item->checkCollision( &player ) != COLLISION_TYPE_NONE ) {
                        item->setState( SPRITE_STATE_HIT );
                        item->playCollisionSound();

                        // item->updateMario( player );

                        item->updatePlayer( player );

                    }
                } else if ( item->getY() > map.getMaxHeight() ) {
                    item->setState( SPRITE_STATE_TO_BE_REMOVED );
                }
            }

            if ( item->getState() == SPRITE_STATE_TO_BE_REMOVED ) {
                collectedIndexes.push_back( static_cast<int>( i ) );
            }

        }

        for ( int i = collectedIndexes.size() - 1; i >= 0; i-- ) {
            delete StaticItems[collectedIndexes[i]];
            StaticItems.erase( StaticItems.begin() + collectedIndexes[i] );
        }

        // baddies activation and player and fireballs x baddies collision resolution and offscreen baddies removal
        collectedIndexes.clear();
        if ( player.getState() != SPRITE_STATE_DYING && 
             player.getState() != SPRITE_STATE_VICTORY &&
             player.getState() != SPRITE_STATE_WAITING_TO_NEXT_MAP ) {

            player.updateCollisionProbes();

            for ( size_t i = 0; i < Baddies.size(); i++ ) {

                Baddie* baddie = Baddies[i];

                // baddies activation
                if ( baddie->getState() == SPRITE_STATE_IDLE ) {
                    baddie->activateWithPlayerProximity( player );
                }

                if ( baddie->getState() != SPRITE_STATE_DYING && 
                     baddie->getState() != SPRITE_STATE_TO_BE_REMOVED ) {

                    // Distance-based collision optimization: Only check collision if Player is close to the baddie
                    if (shouldCheckCollision(player.getPos(), player.getDim(), baddie->getPos(), baddie->getDim(), maxDistForCollisionCheck)) {
                        const CollisionType col = player.checkCollisionBaddie( baddie );

                        if ( player.isInvincible() && col ) {
                            baddie->onHit();
                            PlaySound( sounds["stomp"] );
                            player.addPoints( baddie->getEarnedPoints() );
                        } else {

                            if ( baddie->getAuxiliaryState() != SPRITE_STATE_INVULNERABLE ) {

                                // player and fireballs x baddies collision resolution and offscreen baddies removal
                                switch ( col ) {
                                    case COLLISION_TYPE_NORTH:
                                    case COLLISION_TYPE_EAST:
                                    case COLLISION_TYPE_WEST:
                                        if ( !player.isImmortal() && !player.isInvulnerable() ) {
                                            switch ( player.getType() ) {
                                                case PLAYER_TYPE_SMALL:
                                                    player.setState( SPRITE_STATE_DYING );
                                                    player.playPlayerDownMusicStream();
                                                    player.removeLives( 1 );
                                                    break;
                                                case PLAYER_TYPE_SUPER:
                                                    PlaySound( sounds["pipe"] );
                                                    player.setLastStateBeforeTransition( player.getState() );
                                                    player.setState( SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL );
                                                player.setInvulnerable( true );
                                                break;
                                            case PLAYER_TYPE_FLOWER:
                                                PlaySound( sounds["pipe"] );
                                                player.setLastStateBeforeTransition( player.getState() );
                                                player.setState( SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL );
                                                player.setInvulnerable( true );
                                                break;
                                        }
                                    }
                                    break;
                                case COLLISION_TYPE_SOUTH:
                                    if ( player.getState() == SPRITE_STATE_FALLING && 
                                         baddie->getState() != SPRITE_STATE_DYING && 
                                         baddie->getState() != SPRITE_STATE_TO_BE_REMOVED ) {
                                        player.setY( baddie->getY() - player.getHeight() );
                                        if ( ( IsKeyDown( KEY_LEFT_CONTROL ) ||
                                               IsGamepadButtonDown( 0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT ) ) ) {
                                            player.setVelY( -400 );
                                        } else {
                                            player.setVelY( -200 );
                                        }
                                        player.setState( SPRITE_STATE_JUMPING );
                                        baddie->onHit();
                                        PlaySound( sounds["stomp"] );
                                        player.addPoints( baddie->getEarnedPoints() );
                                    } else {
                                        if ( !player.isImmortal() && !player.isInvulnerable() ) {
                                            switch ( player.getType() ) {
                                                case PLAYER_TYPE_SMALL:
                                                    player.setState( SPRITE_STATE_DYING );
                                                    player.playPlayerDownMusicStream();
                                                    player.removeLives( 1 );
                                                    break;
                                                case PLAYER_TYPE_SUPER:
                                                    PlaySound( sounds["pipe"] );
                                                    player.setLastStateBeforeTransition( player.getState() );
                                                    player.setState( SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL );
                                                    player.setInvulnerable( true );
                                                    break;
                                                case PLAYER_TYPE_FLOWER:
                                                    PlaySound( sounds["pipe"] );
                                                    player.setLastStateBeforeTransition( player.getState() );
                                                    player.setState( SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL );
                                                    player.setInvulnerable( true );
                                                    break;
                                            }
                                        }
                                    }
                                    break;
                                case COLLISION_TYPE_FIREBALL:
                                    baddie->onHit();
                                    PlaySound( sounds["stomp"] );
                                    player.addPoints( baddie->getEarnedPoints() );
                                    break;
                                default:
                                    break;

                            }

                        } else {

                            if ( col ) {
                                if ( col == COLLISION_TYPE_FIREBALL ) {
                                    baddie->onHit();
                                    PlaySound( sounds["stomp"] );
                                    player.addPoints( baddie->getEarnedPoints() );
                                } else {
                                    if ( !player.isImmortal() && !player.isInvulnerable() ) {
                                        switch ( player.getType() ) {
                                            case PLAYER_TYPE_SMALL:
                                                player.setState( SPRITE_STATE_DYING );
                                                player.playPlayerDownMusicStream();
                                                player.removeLives( 1 );
                                                break;
                                            case PLAYER_TYPE_SUPER:
                                                PlaySound( sounds["pipe"] );
                                                player.setLastStateBeforeTransition( player.getState() );
                                                player.setState( SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL );
                                                player.setInvulnerable( true );
                                                break;
                                            case PLAYER_TYPE_FLOWER:
                                                PlaySound( sounds["pipe"] );
                                                player.setLastStateBeforeTransition( player.getState() );
                                                player.setState( SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL );
                                                player.setInvulnerable( true );
                                                break;
                                        }
                                    }
                                }
                            }

                        }

                    }
                    
                    } // Close distance check bracket

                    if ( baddie->getX() + baddie->getWidth() < 0 ||
                         baddie->getX() > map.getMaxWidth() ||
                         baddie->getY() > map.getMaxHeight() ) {
                        baddie->setState( SPRITE_STATE_TO_BE_REMOVED );
                    }

                } else {
                    if ( baddie->getX() + baddie->getWidth() < 0 ||
                         baddie->getX() > map.getMaxWidth() ||
                         baddie->getY() > map.getMaxHeight() ) {
                        baddie->setState( SPRITE_STATE_TO_BE_REMOVED );
                    }
                }

                if ( baddie->getState() == SPRITE_STATE_TO_BE_REMOVED ) {
                    collectedIndexes.push_back( static_cast<int>( i ) );
                }

                player.updateCollisionProbes();

            }

        }

        for ( int i = collectedIndexes.size() - 1; i >= 0; i-- ) {

            Baddie *baddie = Baddies[collectedIndexes[i]];
            delete baddie;
            Baddies.erase( Baddies.begin() + collectedIndexes[i] );

            // the map draws baddies in two layers
            // the baddies are stored in three vectors:
            //     one for baddies management (all baddies)
            //     one for drawing in front of the scenario
            //     one for drawing in back of the scenario
            map.eraseBaddieFromDrawingVectors( baddie );

        }
    }

    else if (player.getState() == SPRITE_STATE_DYING) {
            if ( !player.isPlayerDownMusicStreamPlaying() && !player.isGameOverMusicStreamPlaying() ) {
            
            if ( player.getLives() > 0 ) {
                resetMap();
            } else if ( player.getLives() < 0 ) {
                resetGame();
            } else {
                player.playGameOverMusicStream();
                state = GAME_STATE_GAME_OVER;
                player.setLives( -1 );
            }
        }
    }

    else if ( player.getState() == SPRITE_STATE_VICTORY ) {
        remainingTimePointCount = player.getRemainingTime();
        state = GAME_STATE_COUNTING_POINTS;
        map.setDrawBlackScreen( true );
        player.setState( SPRITE_STATE_WAITING_TO_NEXT_MAP );
    }

    else if (state == GAME_STATE_COUNTING_POINTS) {
        if ( !IsMusicStreamPlaying( musics["courseClear"] ) ) {
            PlayMusicStream( musics["courseClear"] );
        } else {
            UpdateMusicStream( musics["courseClear"] );
        }

        remainingTimePointCount--;
        player.addPoints( 50 );

        if ( remainingTimePointCount % 3 == 0 ) {
            PlaySound( sounds["coin"] );
        }

        if ( remainingTimePointCount == 0 ) {
            state = GAME_STATE_OUTRO;
        }
    }

    // pre-outro (music control)
    else if (state == GAME_STATE_OUTRO) {
        if ( !IsMusicStreamPlaying( musics["courseClear"] ) ) {
            StopMusicStream( musics["courseClear"] );
            PlaySound( sounds["goalIrisOut"] );
            state = GAME_STATE_GO_TO_NEXT_MAP;
            outroAcum = 0;
        } else {
            UpdateMusicStream( musics["courseClear"] );
            if ( static_cast<int>(GetMusicTimeLength( musics["courseClear"] )) == static_cast<int>(GetMusicTimePlayed( musics["courseClear"] )) ) {
                StopMusicStream( musics["courseClear"] );
            }
        }
    }

    // real outro (time control and call next map)
    else if (state == GAME_STATE_GO_TO_NEXT_MAP) {
        outroAcum += GetFrameTime();
        if ( outroAcum >= outroTime ) {
            outroFinished = true;
        }

        if ( outroFinished ) {
            outroAcum = 0;
            outroFinished = false;
            nextMap();
        }
    }

    else if ( state == GAME_STATE_SETTINGS_SCREEN ) {
        if ((IsKeyPressed(KEY_P) || settingScreen->settingBoardShouldClose()) && pauseButtonsCooldownAcum <= 0.0f) {
            unpauseGame();
        }
    }


    else if ( state == GAME_STATE_HELPING_SCREEN ) {
        if ((IsKeyPressed(KEY_H) || helpingScreen->helpingBoardShouldClose()) && pauseButtonsCooldownAcum <= 0.0f) {
            unpauseGame();
        }
    }

    else if ( state == GAME_STATE_GUARD_SCREEN ) {
        if (guardScreen->getCancelButton()->isReleased()) {
            // Cancel action, return to settings screen
            state = GAME_STATE_SETTINGS_SCREEN;
            settingBoardIsOpen = true;
            pauseButtonsCooldownAcum = pauseButtonsCooldownTime;
            // Ensure we maintain the correct pause state
            pauseMusic = true;
            pausePlayer = true;
        }
        else if (guardScreen->getAcceptButton()->isReleased()) {
            // Execute the guarded action
            if (guardScreen->getCurrentAction() == GUARD_ACTION_HOME) {
                resetGame();
                state = GAME_STATE_TITLE_SCREEN;
                settingBoardIsOpen = false;
                pauseMusic = false;
                pausePlayer = false;
            }
            else if (guardScreen->getCurrentAction() == GUARD_ACTION_RESET) {
                resetMap();
                settingBoardIsOpen = false;
            }
        }
    }


    if (settingBoardIsOpen){
        settingScreen->update();
        settingScreen->updateVolume(); 

        if (settingScreen->settingBoardShouldClose()) {
            settingBoardIsOpen = false;
        }
    }

    // Update setting button cooldown even when not in game
    if (pauseButtonsCooldownAcum > 0.0f) {
        pauseButtonsCooldownAcum -= GetFrameTime();
        if (pauseButtonsCooldownAcum < 0.0f) {
            pauseButtonsCooldownAcum = 0.0f;
        }
    }

    if ( player.getState() != SPRITE_STATE_DYING && 
         player.getY() > map.getMaxHeight() ) {

        player.setState( SPRITE_STATE_DYING );
        player.playPlayerDownMusicStream();
        player.removeLives( 1 );
    }

    const float xc = GetScreenWidth() / 2.0;
    const float yc = GetScreenHeight() / 2.0;
    const float pxc = player.getX() + player.getWidth() / 2.0;
    const float pyc = player.getY() + player.getHeight() / 2.0;
    
    camera->offset.x = xc;

    if ( pxc < xc ) {
        camera->target.x = xc + Map::TILE_WIDTH;
        map.setPlayerOffset( 0 );         // x parallax
    } else if ( pxc >= map.getMaxWidth() - xc - Map::TILE_WIDTH ) {
        camera->target.x = map.getMaxWidth() - GetScreenWidth();
        camera->offset.x = 0;
    } else {
        camera->target.x = pxc + Map::TILE_WIDTH;
        map.setPlayerOffset( pxc - xc );  // x parallax
    }

    camera->offset.y = yc;

    if ( pyc < yc ) {
        camera->target.y = yc + Map::TILE_WIDTH;
    } else if ( pyc >= map.getMaxHeight() - yc - Map::TILE_WIDTH ) {
        camera->target.y = map.getMaxHeight() - GetScreenHeight();
        camera->offset.y = 0;
    } else {
        camera->target.y = pyc + Map::TILE_WIDTH;
    }

    if (state == GAME_STATE_TITLE_SCREEN || 
        state == GAME_STATE_MENU_SCREEN || 
        state == GAME_STATE_MAP_EDITOR_SCREEN1 ||
        state == GAME_STATE_MAP_EDITOR_SCREEN2 ||
        state == GAME_STATE_SELECT_CHARACTER_SCREEN ||
        state == GAME_STATE_LEADERBOARD_SCREEN ||
        state == GAME_STATE_LOADGAME_SCREEN ||
        state == GAME_STATE_CREDITS_SCREEN) {

        if (!IsMusicStreamPlaying(musics["title"])) {
            PlayMusicStream(musics["title"]);
        } else {
            UpdateMusicStream(musics["title"]);
        }

        if (state == GAME_STATE_TITLE_SCREEN) {

            if (titleScreen->getStartButton().isReleased()) {
                state = GAME_STATE_MENU_SCREEN;
            }

            else if (titleScreen->getCreditButton().isReleased()) {
                state = GAME_STATE_CREDITS_SCREEN;
            }
        }

        else if (state == GAME_STATE_CREDITS_SCREEN) {
            Texture2D& creditTexture = ResourceManager::getInstance().getTexture("credit");
            Rectangle creditRect = { 
                (GetScreenWidth() - creditTexture.width) / 2.0f, 
                (GetScreenHeight() - creditTexture.height) / 2.0f, 
                1.0f * (creditTexture.width), 
                1.0f * (creditTexture.height) 
            };
            if (!CheckCollisionPointRec(GetMousePosition(), creditRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                state = GAME_STATE_TITLE_SCREEN;
            }
        }

        else if (state == GAME_STATE_MENU_SCREEN) {

            if (settingBoardIsOpen){
                return;
            }

            if (menuScreen->getButton("NEW GAME")->isReleased()) {
                map.first();
                map.setLoadFromUserDesignedMap(false);
                state = GAME_STATE_SELECT_CHARACTER_SCREEN;
            }

            else if (menuScreen->getButton("LOAD GAME")->isReleased()) {
                loadGame->load();
            }

            else if (menuScreen->getButton("DESIGN MAP")->isReleased()) {
                state = GAME_STATE_MAP_EDITOR_SCREEN1;
            }

            else if (menuScreen->getButton("SETTINGS")->isReleased()) {
                settingBoardIsOpen = true;
                settingScreen->setSettingBoardIsOpenInMenuScreen(true);
            }

            else if (menuScreen->getButton("LEADERBOARD")->isReleased()) {
                state = GAME_STATE_LEADERBOARD_SCREEN;
                careTaker->releaseLeaderBoardData();
            }

            else if (menuScreen->getButton("EXIT")->isReleased()) {
                state = GAME_STATE_TITLE_SCREEN;
            }
        }

        else if (state == GAME_STATE_SELECT_CHARACTER_SCREEN) {
            if (selectCharacterScreen->getMarioButton().isReleased()) {
                // Mario selected - start the game
                player.resetAll();
				player.toMario(); // Switch to Mario
                map.loadFromJsonFile(); // Load the map immediately
                player.setMaxTime(400.0f); // Set the time limit (400 seconds = typical Mario game time)
                
                if (IsMusicStreamPlaying(musics["title"])) {
                    StopMusicStream(musics["title"]);
                }            
                state = GAME_STATE_PLAYING;
            }

            else if (selectCharacterScreen->getLuigiButton().isReleased()) {
                // Luigi selected - implement later
                // std::cout << "Luigi button pressed. Luigi functionality will be implemented later." << std::endl;

                player.resetAll();
				player.toLuigi(); // Switch to Luigi
                lastCheckpointData.isLuigi = true;
                map.loadFromJsonFile(); // Load the map immediately
                player.setMaxTime(400.0f); // Set the time limit (400 seconds = typical Mario game time)

                if (IsMusicStreamPlaying(musics["title"])) {
                    StopMusicStream(musics["title"]);
                }
                state = GAME_STATE_PLAYING;
            }

            else if (selectCharacterScreen->getHomeButton().isReleased()) {
                // Go back to menu
                state = GAME_STATE_MENU_SCREEN;
            }
        }

        else if (state == GAME_STATE_MAP_EDITOR_SCREEN1) {
            if (mapEditorScreen1 != nullptr) {
                mapEditorScreen1->update();
                
                // Only check other buttons if dialog is not open
                if (!mapEditorScreen1->isDialogOpen()) {
                    Button* newMapButton = mapEditorScreen1->getButton("NEW MAP");
                    if (newMapButton != nullptr && newMapButton->isReleased()) {
                        // Create new map and navigate to map editor screen 2
                        mapEditorScreen1->createNewMap();
                    }

                    Button* backButton = mapEditorScreen1->getButton("BACK TO MENU");
                    if (backButton != nullptr && backButton->isReleased()) {
                        state = GAME_STATE_MENU_SCREEN;
                    }
                }
            }
        }

        else if (state == GAME_STATE_MAP_EDITOR_SCREEN2) {
            if (mapEditorScreen2 != nullptr) {
                mapEditorScreen2->update();
                if (mapEditorScreen2->getPlayButton()->isReleased()) {
                    // Start the game with the current map
                    state = GAME_STATE_SELECT_CHARACTER_SCREEN;
                    map.setCurrentData(mapEditorScreen2->getCurrentMapData());
                    map.setLoadFromUserDesignedMap(true);
                }
                else if (mapEditorScreen2->getReturnButton()->isReleased()) {
                    // Return to the previous screen
                    state = GAME_STATE_MAP_EDITOR_SCREEN1;
                }
            }
        }

        else if (state == GAME_STATE_LEADERBOARD_SCREEN) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                state = GAME_STATE_MENU_SCREEN;
            }
            else if (leaderBoardScreen && leaderBoardScreen->getReturnButton()->isReleased()) {
            state = GAME_STATE_MENU_SCREEN;
            }
        }
    
    }
    
    else if ( state == GAME_STATE_GAME_OVER ) {
        player.playGameOverMusicStream();
        careTaker -> saveToCareTakerLeaderBoard();
        careTaker->releaseLeaderBoardData();
        leaderBoardScreen->setLatestDataLoaded(true);
		state = GAME_STATE_LEADERBOARD_SCREEN;
    }

//     else if (state == GAME_STATE_LEADERBOARD_SCREEN) {
//         if (IsKeyPressed(KEY_ESCAPE)) {
//             state = GAME_STATE_MENU_SCREEN;
//         }
//         else if (leaderBoardScreen && leaderBoardScreen->getReturnButton()->isReleased()) {
//         state = GAME_STATE_MENU_SCREEN;
//         }
//     }

// 	// Added additional fireball collision checks
//     for (auto& fireball : player.fireballs) {

// 		// Check fireball collisions with baddies
//         /*for (auto& baddie : Baddies) {
//             if (baddie->getState() != SPRITE_STATE_DYING && baddie->getState() != SPRITE_STATE_TO_BE_REMOVED) {
//                 if (shouldCheckCollision(fireball.getPos(), fireball.getDim(), baddie->getPos(), baddie->getDim(), maxDistForCollisionCheck)) {
//                     if (fireball.checkCollision(baddie) == COLLISION_TYPE_COLLIDED) {
//                         fireball.setState(SPRITE_STATE_TO_BE_REMOVED);
//                         baddie->onHit();
//                         PlaySound(sounds["stomp"]);
//                         player.addPoints(baddie->getEarnedPoints());
//                     }
//                 }
//             }
//         }*/

// 		// Check fireball collisions with tiles
//         for (auto& tile : Tiles) {
//             if (shouldCheckCollision(fireball.getPos(), fireball.getDim(), tile->getPos(), tile->getDim(), maxDistForCollisionCheck)) {
//                 CollisionType col = fireball.checkCollision(tile);
//                 switch (col) {
//                     case COLLISION_TYPE_NORTH:
//                         fireball.setVelY(-fireball.getVelY());
//                         break;
//                     case COLLISION_TYPE_SOUTH:
//                         fireball.setVelY(-300);
//                         break;
//                     case COLLISION_TYPE_EAST:
//                     case COLLISION_TYPE_WEST:
//                         fireball.setState(SPRITE_STATE_TO_BE_REMOVED);
//                         break;
//                     default:
//                         break;
//                 }
//             }
//         }

// 		// Check fireball collisions with blocks
//         for (auto& block : Blocks) {
//             if (shouldCheckCollision(fireball.getPos(), fireball.getDim(), block->getPos(), block->getDim(), maxDistForCollisionCheck)) {
//                 CollisionType col = fireball.checkCollision(block);
//                 switch (col) {
//                     case COLLISION_TYPE_NORTH:
//                         fireball.setVelY(-fireball.getVelY());
//                         break;
//                     case COLLISION_TYPE_SOUTH:
//                         fireball.setVelY(-300);
//                         break;
//                     case COLLISION_TYPE_EAST:
//                     case COLLISION_TYPE_WEST:
//                         fireball.setState(SPRITE_STATE_TO_BE_REMOVED);
//                         break;
//                     default:
//                         break;
//                 }
//             }
//         }
//     }
}

void GameWorld::draw() {
    BeginDrawing();
    ClearBackground(WHITE);
    std::map<std::string, Texture2D>& textures = ResourceManager::getInstance().getTextures();

    // std:: cerr << "Total played time: " << totalPlayedTime << " seconds" << std::endl;

    if (state == GAME_STATE_TITLE_SCREEN) {
        titleScreen->draw();
    }

    else if (state == GAME_STATE_CREDITS_SCREEN) {
        titleScreen->draw();

        // Draw a faded background
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.3f));

        DrawTexture(textures["credit"], (GetScreenWidth() - textures["credit"].width) / 2, 
                                        (GetScreenHeight() - textures["credit"].height) / 2, WHITE);
    }

    else if (state == GAME_STATE_MENU_SCREEN) {
        menuScreen->draw();
    }

    else if (state == GAME_STATE_MAP_EDITOR_SCREEN1) {
        if (mapEditorScreen1 != nullptr) {
            mapEditorScreen1->draw();
        }
    }

    else if (state == GAME_STATE_MAP_EDITOR_SCREEN2) {
        if (mapEditorScreen2 != nullptr) {
            mapEditorScreen2->draw();
        }
    }

    else if (state == GAME_STATE_SELECT_CHARACTER_SCREEN) {
        selectCharacterScreen->draw();
    }

    else if (state == GAME_STATE_LEADERBOARD_SCREEN) {
        if(leaderBoardScreen)
            leaderBoardScreen->draw();
    }

    else if (state == GAME_STATE_GAME_OVER){
        DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), BLACK );
        Texture2D* t = &textures["guiGameOver"];
        DrawTexture( *t, GetScreenWidth() / 2 - t->width / 2, GetScreenHeight() / 2 - t->height / 2, WHITE );
    }

    // Draw the map
    else {
        BeginMode2D(*camera);
        map.draw();
        EndMode2D();
        player.drawHud();
        settingButton->draw();
        helpButton->draw();

        if ( state == GAME_STATE_TIME_UP ) {

            DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), BLACK );
            Texture2D* t = &textures["guiTimeUp"];
            DrawTexture( *t, GetScreenWidth() / 2 - t->width / 2, GetScreenHeight() / 2 - t->height / 2, WHITE );

        } 
        
        else if ( state == GAME_STATE_COUNTING_POINTS || state == GAME_STATE_OUTRO || state == GAME_STATE_GO_TO_NEXT_MAP ) {

            Vector2 sc{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
            DrawTexture( textures["guiMario"], sc.x - textures["guiMario"].width / 2, sc.y - 120, WHITE);

            std::string message1 = "course clear!";
            drawString( message1, sc.x - getDrawStringWidth( message1 ) / 2, sc.y - 80 );

            int clockWidth = textures["guiClock"].width;
            int remainingTimeWidth = getSmallNumberWidth( player.getRemainingTime() );
            int pointsPerSecondWidth = getSmallNumberWidth( 50 );
            int timesWidth = textures["guiX"].width;
            int equalSignWidth = getDrawStringWidth( "=" );
            int totalTimePoints = player.getRemainingTime() * 50;
            int totalTimePointsWidth = getSmallNumberWidth( totalTimePoints );
            int completeMessageWidth = clockWidth + remainingTimeWidth + pointsPerSecondWidth + timesWidth + equalSignWidth + totalTimePointsWidth;
            int completeMessageStart = sc.x - (completeMessageWidth/2);
            int completeMessageY = sc.y - 40;

            DrawTexture( textures["guiClock"], completeMessageStart, completeMessageY, WHITE );
            drawWhiteSmallNumber( player.getRemainingTime(), completeMessageStart + clockWidth, completeMessageY );
            DrawTexture( textures["guiX"], completeMessageStart + clockWidth + remainingTimeWidth, completeMessageY, WHITE );
            drawWhiteSmallNumber( 50, completeMessageStart + clockWidth + remainingTimeWidth + timesWidth, completeMessageY );
            drawString( "=", completeMessageStart + clockWidth + remainingTimeWidth + timesWidth + pointsPerSecondWidth, completeMessageY - 4 );
            drawWhiteSmallNumber( totalTimePoints, completeMessageStart + clockWidth + remainingTimeWidth + timesWidth + pointsPerSecondWidth + equalSignWidth, completeMessageY );

            Vector2 centerFunnel = GetWorldToScreen2D( player.getCenter(), *camera );
            DrawRing( centerFunnel, 
                      sqrt( GetScreenWidth() * GetScreenWidth() + GetScreenHeight() * GetScreenHeight() ) * ( 1 -  outroAcum / outroTime ),
                      GetScreenWidth() * 2, 
                      0, 360, 100, BLACK );

        } 
        
        else if ( state == GAME_STATE_FINISHED ) {

            std::map<std::string, Music>& musics = ResourceManager::getInstance().getMusics();

            if ( !IsMusicStreamPlaying( musics["ending"] ) ) {
                PlayMusicStream( musics["ending"] );
            } else {
                UpdateMusicStream( musics["ending"] );
            }

            if ( GetKeyPressed() ) {
                StopMusicStream( musics["ending"] );
                careTaker->releaseLeaderBoardData();
                state = GAME_STATE_LEADERBOARD_SCREEN;
                resetGame();
            }

            DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), Fade( RAYWHITE, 0.9 ) );

            std::string message1 = "Thank you for playing!!!";
            std::string message2 = "Press any key to restart!";

            drawString( message1, GetScreenWidth() / 2 - getDrawStringWidth( message1 ) / 2, 40 );
            drawString( message2, GetScreenWidth() / 2 - getDrawStringWidth( message2 ) / 2, 65 );

        } 
        
    }
    

    if (settingBoardIsOpen) {
        settingScreen->draw();
    }

    if (helpingBoardIsOpen) {
        helpingScreen->draw();
    }

    if (state == GAME_STATE_GUARD_SCREEN) {
        guardScreen->draw();
    }

    EndDrawing();
}

void GameWorld::loadResources() {
    ResourceManager::getInstance().loadResources();
}

void GameWorld::unloadResources() {
    ResourceManager::getInstance().unloadResources();
}

void GameWorld::setCamera(Camera2D* camera) {
    this->camera = camera;
    this->map.setCamera(camera);
}

Camera2D* GameWorld::getCamera() const {
    return this->camera;
}

int GameWorld::getTotalPlayedTime() const {
    return totalPlayedTime;
}

void GameWorld::addToTotalPlayedTime(float timeToAdd) {
    totalPlayedTime += static_cast<int>(timeToAdd);
}

void GameWorld::stopAllMusic() {
    std::map<std::string, Music>& musics = ResourceManager::getInstance().getMusics();
    
    // Stop all possible music streams
    for (auto& musicPair : musics) {
        if (IsMusicStreamPlaying(musicPair.second)) {
            StopMusicStream(musicPair.second);
        }
    }
}

void GameWorld::resetMap() {
    // Stop all music streams before resetting
    stopAllMusic();
    player.reset(true);
    map.reset();
    map.loadFromJsonFile(); 
    pauseMusic = false;  // Ensure music isn't paused after reset
    state = GAME_STATE_PLAYING;
}

void GameWorld::resetGame() {
    // Stop all music streams before resetting
    stopAllMusic();
    player.resetAll();
    map.first();
    map.reset();
    totalPlayedTime = 0; // Reset total played time when starting a new game
    pauseMusic = false;  // Ensure music isn't paused after reset
    state = GAME_STATE_TITLE_SCREEN;
}

void GameWorld::nextMap() {
    if (map.hasNext()) {
        state = GAME_STATE_PLAYING;

        // Add Player's elapsed time to total played time when map is completed
        totalPlayedTime += static_cast<int>(player.getEllapsedTime());
        player.setPointsFromPreviousMap(player.getPoints());
        player.setCoinsFromPreviousMap(player.getCoins());
	    // careTaker -> saveToCareTakerLeaderBoard();
        lastCheckpointData = savedData(
            map.getId(),
            player.getLives(),
            player.getPoints(),
            player.getCoins(),
			player.getType(),
            lastCheckpointData.isLuigi
            );
        player.reset(false, false);
    } else {
        state = GAME_STATE_FINISHED;
        careTaker->saveToCareTakerLeaderBoard();
		careTaker->releaseLeaderBoardData();
		leaderBoardScreen->setLatestDataLoaded(true);
    }
}

void GameWorld::pauseGame(bool playPauseSFX, bool pauseMusic, bool pausePlayer, bool showSettingBoard, bool showHelpingBoard) {
    if (playPauseSFX) {
        PlaySound(ResourceManager::getInstance().getSounds()["pause"]);
    }
    this->pauseMusic = pauseMusic;
    this->pausePlayer = pausePlayer;
    settingBoardIsOpen = showSettingBoard;
    helpingBoardIsOpen = showHelpingBoard;
    this->stateBeforePause = state;

    state = settingBoardIsOpen ? GAME_STATE_SETTINGS_SCREEN : GAME_STATE_HELPING_SCREEN; 
}

void GameWorld::unpauseGame() {
    state = stateBeforePause;
    pauseMusic = false;
    pausePlayer = false;
    
    // Start cooldown timer if settings screen was open
    if (settingBoardIsOpen || helpingBoardIsOpen) {
        pauseButtonsCooldownAcum = pauseButtonsCooldownTime;
    }
    
    settingBoardIsOpen = false;
    helpingBoardIsOpen = false;
}

void GameWorld::showGuardScreen(GuardAction action) {
    if (guardScreen != nullptr) {
        // Only update stateBeforePause if we're not already in a paused state
        if (state != GAME_STATE_SETTINGS_SCREEN && state != GAME_STATE_HELPING_SCREEN && state != GAME_STATE_GUARD_SCREEN) {
            stateBeforePause = state;
        }
        state = GAME_STATE_GUARD_SCREEN;
        guardScreen->setAction(action);
        pauseMusic = true;
        pausePlayer = true;
        settingBoardIsOpen = false;
        helpingBoardIsOpen = false;
    }
}
// Distance threshold getter and setter
float GameWorld::getMaxDistForCollisionCheck() const {
    return maxDistForCollisionCheck;
}


void GameWorld::setCaretaker(CareTaker* caretaker) {
    this->careTaker = caretaker;
}
void GameWorld::setLoadGame( LoadGame* loadgame) {
    this->loadGame = loadgame;
}

