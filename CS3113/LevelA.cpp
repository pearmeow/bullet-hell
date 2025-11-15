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

    mGameState.bgm = LoadMusicStream("");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    /*
       ----------- MAP -----------
    */
    mGameState.map = nullptr;
    // mGameState.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT,  // map grid cols & rows
    //                          (unsigned int*)mLevelData,  // grid data
    //                          "",                         // texture filepath
    //                          TILE_DIMENSION,             // tile size
    //                          4, 1,                       // texture cols & rows
    //                          mOrigin                     // in-game origin
    // );

    /*
       ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> playerAnimationAtlas = {
        {DOWN, {6, 7, 8}},
        {LEFT, {6, 7, 8}},
        {UP, {6, 7, 8}},
        {RIGHT, {6, 7, 8}},
    };

    float sizeRatio = 48.0f / 64.0f;

    mGameState.player = new Player({mOrigin.x - 300.0f, mOrigin.y - 200.0f},   // position
                                                                               //
                                   {250.0f * sizeRatio, 250.0f},               // scale
                                   "./assets/tiny-spaceships/tinyShip16.png",  // texture file address
                                   ATLAS,                                      // single image or atlas?
                                   {4, 3},                                     // atlas dimensions
                                   playerAnimationAtlas,                       // actual atlas
                                   PLAYER                                      // entity type
    );

    // mGameState.player->setColliderDimensions(
    //     {mGameState.player->getScale().x / 3.5f, mGameState.player->getScale().y / 3.0f});
    mGameState.player->setAcceleration({0.0f, 0.0f});

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
    if (mGameState.map) {
        mGameState.map->render();
    }
}

void LevelA::shutdown() {
    delete mGameState.player;

    UnloadMusicStream(mGameState.bgm);
}
