#include "LevelA.h"

#include "Entity.h"

LevelA::LevelA() : Scene{{0.0f}, nullptr} {
}
LevelA::LevelA(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelA::~LevelA() {
    shutdown();
}

void LevelA::initialise() {
    printf("joever\n");
    mGameState.nextSceneID = 0;

    mGameState.bgm = LoadMusicStream("");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    /*
       ----------- MAP -----------
    */
    mGameState.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT,  // map grid cols & rows
                             (unsigned int*)mLevelData,  // grid data
                             "./assets/bounds.png",      // texture filepath
                             TILE_DIMENSION,             // tile size
                             1, 1,                       // texture cols & rows
                             mOrigin                     // in-game origin
    );

    /*
       ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> playerAnimationAtlas = {{DOWN, {5, 6, 7}},
                                                                  {LEFT, {5, 6, 7}},
                                                                  {UP, {5, 6, 7}},
                                                                  {RIGHT, {5, 6, 7}},
                                                                  {
                                                                      STILL,
                                                                      {4, 4, 4},
                                                                  },
                                                                  {
                                                                      INVINCIBLE,
                                                                      {6, 7, 2},
                                                                  }};

    mGameState.player = new Player({mOrigin.x, mOrigin.y + 400.0f},            // position
                                                                               //
                                   {50.0f, 50.0f},                             // scale
                                   "./assets/tiny-spaceships/tinyShip16.png",  // texture file address
                                   ATLAS,                                      // single image or atlas?
                                   {3, 4},                                     // atlas dimensions
                                   playerAnimationAtlas,                       // actual atlas
                                   PLAYER                                      // entity type
    );

    /*
       ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> enemyAnimationAtlas = {
        {DOWN, {5, 6, 7}},
        {LEFT, {5, 6, 7}},
        {UP, {5, 6, 7}},
        {RIGHT, {5, 6, 7}},
        {
            STILL,
            {4, 4, 4},
        },
    };

    // mGameState.player->setColliderDimensions(
    //     {mGameState.player->getScale().x / 3.5f, mGameState.player->getScale().y / 3.0f});
    mGameState.player->setAcceleration({0.0f, 0.0f});
    mGameState.player->setSpeed(140.0f);
    mGameState.player->setColliderDimensions({20.0f, 20.0f});
    mGameState.player->setFrameSpeed(6.0f);

    mGameState.enemies.clear();
    mGameState.enemies.push_back(new Enemy({mOrigin.x, mOrigin.y - 400.0f},            // position
                                           {50.0f, 50.0f},                             // scale
                                           "./assets/tiny-spaceships/tinyShip16.png",  // texture file address
                                           ATLAS,                                      // single image or atlas?
                                           {3, 4},                                     // atlas dimensions
                                           enemyAnimationAtlas,                        // actual atlas
                                           NPC                                         // entity type));
                                           ));

    for (Enemy* enemy : mGameState.enemies) {
        for (int i = 0; i < 400; ++i) {
            Bullet* newBullet = new Bullet({mOrigin.x, mOrigin.y - 450.0f},  // position
                                           {50.0f, 50.0f},                   // scale
                                           "./assets/bullet.png",            // texture file address
                                           NPC                               // entity type)
            );
            newBullet->setColliderDimensions(
                {newBullet->getColliderDimensions().x - 12.0f, newBullet->getColliderDimensions().y - 12.0f});
            enemy->addBullet(newBullet);
        }
    }

    /*
       ----------- CAMERA -----------
    */
    mGameState.camera = {0};             // zero initialize
    mGameState.camera.target = mOrigin;  // camera follows origin
    mGameState.camera.offset = mOrigin;  // camera offset to center of screen
    mGameState.camera.rotation = 0.0f;   // no rotation
    mGameState.camera.zoom = 1.0f;       // default zoom
}

void LevelA::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);

    mGameState.player->update(deltaTime,          // delta time / fixed timestep
                              nullptr,            // player
                              mGameState.map,     // map
                              mGameState.enemies  // enemies (vector)
    );

    for (Enemy* enemy : mGameState.enemies) {
        enemy->update(deltaTime, nullptr, nullptr, nullptr, 0);
    }
}

void LevelA::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    for (Enemy* enemy : mGameState.enemies) {
        enemy->render();
    }
    mGameState.player->render();
}

void LevelA::shutdown() {
    delete mGameState.player;
    for (Enemy* enemy : mGameState.enemies) {
        delete enemy;
    }

    UnloadMusicStream(mGameState.bgm);
}
