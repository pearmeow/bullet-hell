#include "LevelA.h"

LevelA::LevelA() : Scene{{0.0f}, nullptr} {
}
LevelA::LevelA(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelA::~LevelA() {
    shutdown();
}

void LevelA::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
    SetMusicVolume(mGameState.bgm, 0.33f);
    // PlayMusicStream(gState.bgm);

    /*
       ----------- MAP -----------
    */
    mGameState.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT,  // map grid cols & rows
                             (unsigned int*)mLevelData,  // grid data
                             "assets/game/tileset.png",  // texture filepath
                             TILE_DIMENSION,             // tile size
                             4, 1,                       // texture cols & rows
                             mOrigin                     // in-game origin
    );

    /*
       ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> playerAnimationAtlas = {
        {DOWN, {0, 1, 2, 3, 4, 5, 6, 7}},
        {LEFT, {8, 9, 10, 11, 12, 13, 14, 15}},
        {UP, {24, 25, 26, 27, 28, 29, 30, 31}},
        {RIGHT, {40, 41, 42, 43, 44, 45, 46, 47}},
    };

    float sizeRatio = 48.0f / 64.0f;

    // Assets from @see https://sscary.itch.io/the-adventurer-female
    mGameState.player = new Entity({mOrigin.x - 300.0f, mOrigin.y - 200.0f},  // position
                                   {250.0f * sizeRatio, 250.0f},              // scale
                                   "assets/game/walk.png",                    // texture file address
                                   ATLAS,                                     // single image or atlas?
                                   {6, 8},                                    // atlas dimensions
                                   playerAnimationAtlas,                      // actual atlas
                                   PLAYER                                     // entity type
    );

    mGameState.player->setJumpingPower(550.0f);
    mGameState.player->setColliderDimensions(
        {mGameState.player->getScale().x / 3.5f, mGameState.player->getScale().y / 3.0f});
    mGameState.player->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

    /*
       ----------- CAMERA -----------
    */
    mGameState.camera = {0};                                      // zero initialize
    mGameState.camera.target = mGameState.player->getPosition();  // camera follows player
    mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
    mGameState.camera.rotation = 0.0f;                            // no rotation
    mGameState.camera.zoom = 1.0f;                                // default zoom
}

void LevelA::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);

    mGameState.player->update(deltaTime,       // delta time / fixed timestep
                              nullptr,         // player
                              mGameState.map,  // map
                              nullptr,         // collidable entities
                              0                // col. entity count
    );

    Vector2 currentPlayerPosition = {mGameState.player->getPosition().x, mOrigin.y};

    if (mGameState.player->getPosition().y > 800.0f) mGameState.nextSceneID = 1;

    panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelA::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.player->render();
    mGameState.map->render();
}

void LevelA::shutdown() {
    delete mGameState.player;
    delete mGameState.map;

    UnloadMusicStream(mGameState.bgm);
}
