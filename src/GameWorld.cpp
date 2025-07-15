#include "GameWorld.h"
#include "Button.h"
#include "Item.h"

GameState GameWorld::state = GAME_STATE_TITLE_SCREEN;
float GameWorld::gravity = 20;

GameWorld::GameWorld() :
    mario( 
        {0,0},          // position
        {32, 40},       // dimention
        {0, 0},         // velocity
        {0, 0, 0, 255}, // color
        260,            // speedX
        360,            // maxSpeedX
        -600,           // jumpSpeed
        false           
    ),
    map(mario, 1, true, this),
    camera(nullptr),
    settingBoardIsOpen(false),
    helpingBoardIsOpen(false),
    stateBeforePause(GAME_STATE_TITLE_SCREEN),
    remainingTimePointCount(400),
    pauseMusic(false),
    pauseMario(false),
    outroFinished(false),
    outroTime(1.0f),
    outroAcum(0.0f),
    titleScreen(nullptr),
    menuScreen(nullptr),
    settingScreen(nullptr)
    {
        mario.setGameWorld(this);
        mario.setMap(&map);
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

    if (settingScreen != nullptr) {
        delete settingScreen;
        settingScreen = nullptr;
    }
}

Memento* GameWorld::dataFromGameWorldToSave() const {
    Data data(map.getId(), remainingTimePointCount, mario.getPoints(), mario.getLives());
    return new ConcreteMemento(data);
}

void GameWorld::restoreDataFromMemento(const Memento* memento) const {
    // Implement later
    // Data data = memento->getData();
    // map.loadFromJsonFile(data.mapID, true);
    // map.setRemainingTime(data.remainingTime);
    // map.setScore(data.score);
    // map.setLives(data.lives);
}

void GameWorld::initScreens() {
    if (titleScreen == nullptr) {
        titleScreen = new TitleScreen();
    }

    if (menuScreen == nullptr) {
        menuScreen = new MenuScreen();
    }

    if (settingScreen == nullptr) {
        settingScreen = new SettingScreen();
    }
}

void GameWorld::inputAndUpdate() {

    map.loadFromJsonFile();

    std::map<std::string, Sound>& sounds = ResourceManager::getInstance().getSounds();
    std::map<std::string, Music>& musics = ResourceManager::getInstance().getMusics();

    std::vector<Tile*>& Tiles = map.getTiles();
    std::vector<Block*>& Blocks = map.getBlocks();
    std::vector<Baddie*>& Baddies = map.getBaddies();
    std::vector<Item*>& Items = map.getItems();
    std::vector<Item*>& StaticItems = map.getStaticItems();


    // std::cerr << "Mario state: " << mario.getState() << std::endl;
    // std::cerr << "GameWorld state: " << state << std::endl;
    if ( mario.getState() != SPRITE_STATE_DYING && 
         mario.getState() != SPRITE_STATE_VICTORY &&
         mario.getState() != SPRITE_STATE_WAITING_TO_NEXT_MAP &&
         state != GAME_STATE_TITLE_SCREEN &&
         state != GAME_STATE_MENU_SCREEN &&
         state != GAME_STATE_CREDITS_SCREEN &&
         state != GAME_STATE_FINISHED && 
         !pauseMusic ) {
        map.playMusic();
    }

    if ( state != GAME_STATE_TITLE_SCREEN &&
         state != GAME_STATE_MENU_SCREEN &&
         state != GAME_STATE_CREDITS_SCREEN &&
         state != GAME_STATE_FINISHED &&
         state != GAME_STATE_SETTINGS_SCREEN &&
         state != GAME_STATE_HELPING_SCREEN ) {
        mario.setActivationWidth( GetScreenWidth() * 2 );
        mario.update();
    } else if ( !pauseMario && state != GAME_STATE_TITLE_SCREEN && 
        state != GAME_STATE_MENU_SCREEN && 
        state != GAME_STATE_CREDITS_SCREEN) {

        mario.update();
    }

    if ( mario.getState() != SPRITE_STATE_DYING && 
         mario.getState() != SPRITE_STATE_VICTORY &&
         mario.getState() != SPRITE_STATE_WAITING_TO_NEXT_MAP &&
         state != GAME_STATE_TITLE_SCREEN &&
         state != GAME_STATE_MENU_SCREEN &&
         state != GAME_STATE_CREDITS_SCREEN &&
         state != GAME_STATE_FINISHED &&
         state != GAME_STATE_SETTINGS_SCREEN &&
         state != GAME_STATE_HELPING_SCREEN ) {

            std::vector<int> collectedIndexes;

        if ( IsKeyPressed( KEY_P ) ) {
            pauseGame( true, true, true, true, false );
        }

        if ( IsKeyPressed( KEY_H ) ) {
            pauseGame( true, true, true, false, true );
        }
        

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
            baddie->followTheLeader( &mario );
        }

        // tiles collision resolution
        mario.updateCollisionProbes();
        for ( const auto& tile : Tiles ) {

            // mario x tiles
            CollisionType col = mario.checkCollision( tile );

            if ( tile->getType() == TILE_TYPE_SOLID ) {
                switch ( col ) {
                    case COLLISION_TYPE_NORTH:
                        mario.setY( tile->getY() + tile->getHeight() );
                        mario.setVelY( 0 );
                        mario.updateCollisionProbes();
                        break;
                    case COLLISION_TYPE_SOUTH:
                        mario.setY( tile->getY() - mario.getHeight() );
                        mario.setVelY( 0 );
                        mario.setState( SPRITE_STATE_ON_GROUND );
                        mario.updateCollisionProbes();
                        break;
                    case COLLISION_TYPE_EAST:
                        mario.setX( tile->getX() - mario.getWidth() );
                        mario.setVelX( 0 );
                        mario.updateCollisionProbes();
                        break;
                    case COLLISION_TYPE_WEST:
                        mario.setX( tile->getX() + tile->getWidth() );
                        mario.setVelX( 0 );
                        mario.updateCollisionProbes();
                        break;
                    default:
                        break;
                }
            } else if ( tile->getType() == TILE_TYPE_SOLID_FROM_ABOVE ) {
                if ( col == COLLISION_TYPE_SOUTH && mario.getState() == SPRITE_STATE_FALLING ) {
                    mario.setY( tile->getY() - mario.getHeight() );
                    mario.setVelY( 0 );
                    mario.setState( SPRITE_STATE_ON_GROUND );
                    mario.updateCollisionProbes();
                }
            }

            // baddies x tiles
            for ( const auto& baddie : Baddies ) {

                baddie->updateCollisionProbes();

                if ( baddie->getState() != SPRITE_STATE_DYING && 
                     baddie->getState() != SPRITE_STATE_TO_BE_REMOVED &&
                     ( tile->getType() == TILE_TYPE_SOLID || 
                       tile->getType() == TILE_TYPE_SOLID_ONLY_FOR_BADDIES ||
                       tile->getType() == TILE_TYPE_SOLID_FROM_ABOVE ) ) {
                    switch ( baddie->checkCollision( tile ) ) {
                        case COLLISION_TYPE_NORTH:
                            baddie->setY( tile->getY() + tile->getHeight() );
                            baddie->setVelY( 0 );
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

            // items x tiles
            for ( const auto& item : Items ) {

                if ( tile->getType() == TILE_TYPE_SOLID || tile->getType() == TILE_TYPE_SOLID_FROM_ABOVE ) {

                    item->updateCollisionProbes();

                    switch ( item->checkCollision( tile ) ) {
                        case COLLISION_TYPE_NORTH:
                            item->setY( tile->getY() + tile->getHeight() );
                            item->setVelY( 0 );
                            item->updateCollisionProbes();
                            break;
                        case COLLISION_TYPE_SOUTH:
                            item->setY( tile->getY() - item->getHeight() );
                            item->setVelY( 0 );
                            item->onSouthCollision( mario );
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

        // blocks collision resolution
        mario.updateCollisionProbes();

        for ( const auto& block : Blocks ) {

            // mario x blocks
            switch ( mario.checkCollision( block ) ) {
                case COLLISION_TYPE_NORTH:
                    mario.setY( block->getY() + block->getHeight() );
                    mario.setVelY( 0 );
                    mario.updateCollisionProbes();
                    block->doHit( mario, &map );
                    break;
                case COLLISION_TYPE_SOUTH:
                    mario.setY( block->getY() - mario.getHeight() );
                    mario.setVelY( 0 );
                    mario.setState( SPRITE_STATE_ON_GROUND );
                    mario.updateCollisionProbes();
                    break;
                case COLLISION_TYPE_EAST:
                    mario.setX( block->getX() - mario.getWidth() );
                    mario.setVelX( 0 );
                    mario.updateCollisionProbes();
                    break;
                case COLLISION_TYPE_WEST:
                    mario.setX( block->getX() + block->getWidth() );
                    mario.setVelX( 0 );
                    mario.updateCollisionProbes();
                    break;
                default:
                    break;
            }

            // baddies x blocks
            for ( const auto& baddie : Baddies ) {

                baddie->updateCollisionProbes();

                if ( baddie->getState() != SPRITE_STATE_DYING && 
                     baddie->getState() != SPRITE_STATE_TO_BE_REMOVED ) {
                    switch ( baddie->checkCollision( block ) ) {
                        case COLLISION_TYPE_NORTH:
                            baddie->setY( block->getY() + block->getHeight() );
                            baddie->setVelY( 0 );
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

            // items x blocks
            for ( const auto& item : Items ) {

                item->updateCollisionProbes();

                switch ( item->checkCollision( block ) ) {
                    case COLLISION_TYPE_NORTH:
                        item->setY( block->getY() + block->getHeight() );
                        item->setVelY( 0 );
                        item->updateCollisionProbes();
                        break;
                    case COLLISION_TYPE_SOUTH:
                        item->setY( block->getY() - item->getHeight() );
                        item->setVelY( 0 );
                        item->onSouthCollision( mario );
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

        // mario x items collision resolution and offscreen items removal
        for ( size_t i = 0; i < Items.size(); i++ ) {

            Item* item = Items[i];

            if ( item->getState() != SPRITE_STATE_HIT &&
                 item->getState() != SPRITE_STATE_TO_BE_REMOVED ) {
                if ( item->checkCollision( &mario ) != COLLISION_TYPE_NONE ) {
                    if ( !mario.isTransitioning() ) {
                        item->setState( SPRITE_STATE_HIT );
                        item->playCollisionSound();
                        if ( item->isPauseGameOnHit() ) {
                            pauseGame( false, false, false, false, false );
                        }
                        item->updateMario( mario );
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
        
        // mario x static items collision resolution
        collectedIndexes.clear();
        for ( size_t i = 0; i < StaticItems.size(); i++ ) {

            Item* item = StaticItems[i];

            if ( item->getState() != SPRITE_STATE_HIT &&
                 item->getState() != SPRITE_STATE_TO_BE_REMOVED ) {
                if ( item->checkCollision( &mario ) != COLLISION_TYPE_NONE ) {
                    item->setState( SPRITE_STATE_HIT );
                    item->playCollisionSound();
                    item->updateMario( mario );
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

        // baddies activation and mario and fireballs x baddies collision resolution and offscreen baddies removal
        collectedIndexes.clear();
        if ( mario.getState() != SPRITE_STATE_DYING && 
             mario.getState() != SPRITE_STATE_VICTORY &&
             mario.getState() != SPRITE_STATE_WAITING_TO_NEXT_MAP ) {

            mario.updateCollisionProbes();

            for ( size_t i = 0; i < Baddies.size(); i++ ) {

                Baddie* baddie = Baddies[i];

                // baddies activation
                if ( baddie->getState() == SPRITE_STATE_IDLE ) {
                    baddie->activateWithMarioProximity( mario );
                }

                if ( baddie->getState() != SPRITE_STATE_DYING && 
                     baddie->getState() != SPRITE_STATE_TO_BE_REMOVED ) {

                    const CollisionType col = mario.checkCollisionBaddie( baddie );

                    if ( mario.isInvincible() && col ) {
                        baddie->onHit();
                        PlaySound( sounds["stomp"] );
                        mario.addPoints( baddie->getEarnedPoints() );
                    } else {

                        if ( baddie->getAuxiliaryState() != SPRITE_STATE_INVULNERABLE ) {

                            // mario and fireballs x baddies collision resolution and offscreen baddies removal
                            switch ( col ) {
                                case COLLISION_TYPE_NORTH:
                                case COLLISION_TYPE_EAST:
                                case COLLISION_TYPE_WEST:
                                    if ( !mario.isImmortal() && !mario.isInvulnerable() ) {
                                        switch ( mario.getType() ) {
                                            case MARIO_TYPE_SMALL:
                                                mario.setState( SPRITE_STATE_DYING );
                                                mario.playPlayerDownMusicStream();
                                                mario.removeLives( 1 );
                                                break;
                                            case MARIO_TYPE_SUPER:
                                                PlaySound( sounds["pipe"] );
                                                mario.setLastStateBeforeTransition( mario.getState() );
                                                mario.setState( SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL );
                                                mario.setInvulnerable( true );
                                                break;
                                            case MARIO_TYPE_FLOWER:
                                                PlaySound( sounds["pipe"] );
                                                mario.setLastStateBeforeTransition( mario.getState() );
                                                mario.setState( SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL );
                                                mario.setInvulnerable( true );
                                                break;
                                        }
                                    }
                                    break;
                                case COLLISION_TYPE_SOUTH:
                                    if ( mario.getState() == SPRITE_STATE_FALLING && 
                                         baddie->getState() != SPRITE_STATE_DYING && 
                                         baddie->getState() != SPRITE_STATE_TO_BE_REMOVED ) {
                                        mario.setY( baddie->getY() - mario.getHeight() );
                                        if ( ( IsKeyDown( KEY_LEFT_CONTROL ) ||
                                               IsGamepadButtonDown( 0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT ) ) ) {
                                            mario.setVelY( -400 );
                                        } else {
                                            mario.setVelY( -200 );
                                        }
                                        mario.setState( SPRITE_STATE_JUMPING );
                                        baddie->onHit();
                                        PlaySound( sounds["stomp"] );
                                        mario.addPoints( baddie->getEarnedPoints() );
                                    } else {
                                        if ( !mario.isImmortal() && !mario.isInvulnerable() ) {
                                            switch ( mario.getType() ) {
                                                case MARIO_TYPE_SMALL:
                                                    mario.setState( SPRITE_STATE_DYING );
                                                    mario.playPlayerDownMusicStream();
                                                    mario.removeLives( 1 );
                                                    break;
                                                case MARIO_TYPE_SUPER:
                                                    PlaySound( sounds["pipe"] );
                                                    mario.setLastStateBeforeTransition( mario.getState() );
                                                    mario.setState( SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL );
                                                    mario.setInvulnerable( true );
                                                    break;
                                                case MARIO_TYPE_FLOWER:
                                                    PlaySound( sounds["pipe"] );
                                                    mario.setLastStateBeforeTransition( mario.getState() );
                                                    mario.setState( SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL );
                                                    mario.setInvulnerable( true );
                                                    break;
                                            }
                                        }
                                    }
                                    break;
                                case COLLISION_TYPE_FIREBALL:
                                    baddie->onHit();
                                    PlaySound( sounds["stomp"] );
                                    mario.addPoints( baddie->getEarnedPoints() );
                                    break;
                                default:
                                    break;

                            }

                        } else {

                            if ( col ) {
                                if ( col == COLLISION_TYPE_FIREBALL ) {
                                    baddie->onHit();
                                    PlaySound( sounds["stomp"] );
                                    mario.addPoints( baddie->getEarnedPoints() );
                                } else {
                                    if ( !mario.isImmortal() && !mario.isInvulnerable() ) {
                                        switch ( mario.getType() ) {
                                            case MARIO_TYPE_SMALL:
                                                mario.setState( SPRITE_STATE_DYING );
                                                mario.playPlayerDownMusicStream();
                                                mario.removeLives( 1 );
                                                break;
                                            case MARIO_TYPE_SUPER:
                                                PlaySound( sounds["pipe"] );
                                                mario.setLastStateBeforeTransition( mario.getState() );
                                                mario.setState( SPRITE_STATE_TRANSITIONING_SUPER_TO_SMALL );
                                                mario.setInvulnerable( true );
                                                break;
                                            case MARIO_TYPE_FLOWER:
                                                PlaySound( sounds["pipe"] );
                                                mario.setLastStateBeforeTransition( mario.getState() );
                                                mario.setState( SPRITE_STATE_TRANSITIONING_FLOWER_TO_SMALL );
                                                mario.setInvulnerable( true );
                                                break;
                                        }
                                    }
                                }
                            }

                        }

                    }

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

                mario.updateCollisionProbes();

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

    else if (mario.getState() == SPRITE_STATE_DYING) {
            if ( !mario.isPlayerDownMusicStreamPlaying() && !mario.isGameOverMusicStreamPlaying() ) {
            
            if ( mario.getLives() > 0 ) {
                resetMap();
            } else if ( mario.getLives() < 0 ) {
                resetGame();
            } else {
                mario.playGameOverMusicStream();
                state = GAME_STATE_GAME_OVER;
                mario.setLives( -1 );
            }
        }
    }

    else if ( mario.getState() == SPRITE_STATE_VICTORY ) {
        remainingTimePointCount = mario.getRemainingTime();
        state = GAME_STATE_COUNTING_POINTS;
        map.setDrawBlackScreen( true );
        mario.setState( SPRITE_STATE_WAITING_TO_NEXT_MAP );
    }

    else if (state == GAME_STATE_COUNTING_POINTS) {
        if ( !IsMusicStreamPlaying( musics["courseClear"] ) ) {
            PlayMusicStream( musics["courseClear"] );
        } else {
            UpdateMusicStream( musics["courseClear"] );
        }

        remainingTimePointCount--;
        mario.addPoints( 50 );

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
        if (IsKeyPressed(KEY_P) || settingScreen->settingBoardShouldClose()) {
            unpauseGame();
        }
    }

    else if ( state == GAME_STATE_HELPING_SCREEN ) {
        if (IsKeyPressed(KEY_H)) {
            unpauseGame();
        }
    }


    if (settingBoardIsOpen){
        settingScreen->update();
        settingScreen->updateVolume(); 

        if (settingScreen->settingBoardShouldClose()) {
        settingBoardIsOpen = false;
        }
    }

    if ( mario.getState() != SPRITE_STATE_DYING && 
         mario.getY() > map.getMaxHeight() ) {

        mario.setState( SPRITE_STATE_DYING );
        mario.playPlayerDownMusicStream();
        mario.removeLives( 1 );
    }

    const float xc = GetScreenWidth() / 2.0;
    const float yc = GetScreenHeight() / 2.0;
    const float pxc = mario.getX() + mario.getWidth() / 2.0;
    const float pyc = mario.getY() + mario.getHeight() / 2.0;
    
    camera->offset.x = xc;

    if ( pxc < xc ) {
        camera->target.x = xc + Map::TILE_WIDTH;
        map.setMarioOffset( 0 );         // x parallax
    } else if ( pxc >= map.getMaxWidth() - xc - Map::TILE_WIDTH ) {
        camera->target.x = map.getMaxWidth() - GetScreenWidth();
        camera->offset.x = 0;
    } else {
        camera->target.x = pxc + Map::TILE_WIDTH;
        map.setMarioOffset( pxc - xc );  // x parallax
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
                mario.resetAll();
                map.first();
                map.reset();
                map.loadFromJsonFile(); // Load the map immediately

                // Set the time limit (400 seconds = typical Mario game time)
                mario.setMaxTime(400.0f);

                if (IsMusicStreamPlaying(musics["title"])) {
                    StopMusicStream(musics["title"]);
                }            
                state = GAME_STATE_PLAYING;
            }

            else if (menuScreen->getButton("LOAD GAME")->isReleased()) {
                // implement later
                std::cout << "Load Game button pressed. Implement load game functionality later." << std::endl;
            }

            else if (menuScreen->getButton("SETTINGS")->isReleased()) {
                settingBoardIsOpen = true;
            }

            else if (menuScreen->getButton("EXIT")->isReleased()) {
                state = GAME_STATE_TITLE_SCREEN;
            }
        }
    }

    else if ( state == GAME_STATE_GAME_OVER ) {
        mario.playGameOverMusicStream();
    }
      
}

void GameWorld::draw() {
    BeginDrawing();
    ClearBackground(WHITE);
    std::map<std::string, Texture2D>& textures = ResourceManager::getInstance().getTextures();

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
        mario.drawHud();

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
            int remainingTimeWidth = getSmallNumberWidth( mario.getRemainingTime() );
            int pointsPerSecondWidth = getSmallNumberWidth( 50 );
            int timesWidth = textures["guiX"].width;
            int equalSignWidth = getDrawStringWidth( "=" );
            int totalTimePoints = mario.getRemainingTime() * 50;
            int totalTimePointsWidth = getSmallNumberWidth( totalTimePoints );
            int completeMessageWidth = clockWidth + remainingTimeWidth + pointsPerSecondWidth + timesWidth + equalSignWidth + totalTimePointsWidth;
            int completeMessageStart = sc.x - (completeMessageWidth/2);
            int completeMessageY = sc.y - 40;

            DrawTexture( textures["guiClock"], completeMessageStart, completeMessageY, WHITE );
            drawWhiteSmallNumber( mario.getRemainingTime(), completeMessageStart + clockWidth, completeMessageY );
            DrawTexture( textures["guiX"], completeMessageStart + clockWidth + remainingTimeWidth, completeMessageY, WHITE );
            drawWhiteSmallNumber( 50, completeMessageStart + clockWidth + remainingTimeWidth + timesWidth, completeMessageY );
            drawString( "=", completeMessageStart + clockWidth + remainingTimeWidth + timesWidth + pointsPerSecondWidth, completeMessageY - 4 );
            drawWhiteSmallNumber( totalTimePoints, completeMessageStart + clockWidth + remainingTimeWidth + timesWidth + pointsPerSecondWidth + equalSignWidth, completeMessageY );

            Vector2 centerFunnel = GetWorldToScreen2D( mario.getCenter(), *camera );
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
                resetGame();
            }

            DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), Fade( RAYWHITE, 0.9 ) );
            Texture2D* t = &textures["guiCredits"];
            DrawTexture( *t, GetScreenWidth() / 2 - t->width / 2, 20, WHITE );

            std::string message1 = "Thank you for playing!!!";
            std::string message2 = "Press any key to restart!";

            drawString( message1, GetScreenWidth() / 2 - getDrawStringWidth( message1 ) / 2, t->height + 40 );
            drawString( message2, GetScreenWidth() / 2 - getDrawStringWidth( message2 ) / 2, t->height + 65 );

        } 
        
        // else if ( state == GAME_STATE_PAUSED ) {
        //     if ( showOverlayOnPause ) {
        //         DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), Fade( BLACK, 0.3 ) );
        //     }
        // }
    }

    if (settingBoardIsOpen) {
        settingScreen->draw();
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

void GameWorld::resetMap() {
    mario.reset(true);
    map.reset();
    map.loadFromJsonFile(); 
    state = GAME_STATE_PLAYING;
}

void GameWorld::resetGame() {
    mario.resetAll();
    map.first();
    map.reset();
    state = GAME_STATE_TITLE_SCREEN;
}

void GameWorld::nextMap() {
    if (map.hasNext()) {
        state = GAME_STATE_PLAYING;
        // reset state but keep the power-up
        mario.reset(false);
    } else {
        state = GAME_STATE_FINISHED;
    }
}

void GameWorld::pauseGame(bool playPauseSFX, bool pauseMusic, bool pauseMario, bool showSettingBoard, bool showHelpingBoard) {
    if (playPauseSFX) {
        PlaySound(ResourceManager::getInstance().getSounds()["pause"]);
    }
    this->pauseMusic = pauseMusic;
    this->pauseMario = pauseMario;
    settingBoardIsOpen = showSettingBoard;
    helpingBoardIsOpen = showHelpingBoard;
    this->stateBeforePause = state;

    state = settingBoardIsOpen ? GAME_STATE_SETTINGS_SCREEN : GAME_STATE_HELPING_SCREEN; 
}

void GameWorld::unpauseGame() {
    state = stateBeforePause;
    pauseMusic = false;
    pauseMario = false;
    settingBoardIsOpen = false;
    helpingBoardIsOpen = false;
}

