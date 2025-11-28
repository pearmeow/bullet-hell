#include "LevelB.h"

#include "Bullet.h"
#include "Enemy.h"
#include "Entity.h"
#include "Player.h"

LevelB::LevelB() : Scene{{0.0f}, nullptr} {
}
LevelB::LevelB(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelB::~LevelB() {
    shutdown();
}

void LevelB::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.bgm = LoadMusicStream("");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    /*
       ----------- MAP -----------
    */
    if (!mGameState.map) {
        mGameState.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT,  // map grid cols & rows
                                 (unsigned int*)mLevelData,  // grid data
                                 "./assets/bounds.png",      // texture filepath
                                 TILE_DIMENSION,             // tile size
                                 1, 1,                       // texture cols & rows
                                 mOrigin                     // in-game origin
        );
    }

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

    if (!mGameState.player) {
        mGameState.player = new Player({mOrigin.x, mOrigin.y + 400.0f},            // position
                                                                                   //
                                       {50.0f, 50.0f},                             // scale
                                       "./assets/tiny-spaceships/tinyShip16.png",  // texture file address
                                       ATLAS,                                      // single image or atlas?
                                       {3, 4},                                     // atlas dimensions
                                       playerAnimationAtlas,                       // actual atlas
                                       PLAYER                                      // entity type
        );
        mGameState.player->setAcceleration({0.0f, 0.0f});
        mGameState.player->setSpeed(140.0f);
        mGameState.player->setColliderDimensions(
            {mGameState.player->getScale().x / 3.0f, mGameState.player->getScale().y / 3.0f});
        mGameState.player->setColliderRadius(mGameState.player->getColliderDimensions().x / 2.0f);
        mGameState.player->setFrameSpeed(6.0f);

        for (int i = 0; i < 2500; ++i) {
            Bullet* newBullet = new Bullet({mOrigin.x, mOrigin.y - 450.0f},  // position
                                           {20.0f, 20.0f},                   // scale
                                           "./assets/playerBullet.png",      // texture file address
                                           NPC                               // entity type)
            );
            newBullet->setColliderDimensions(
                {newBullet->getColliderDimensions().x - 8.0f, newBullet->getColliderDimensions().y - 8.0f});
            newBullet->setColliderRadius(newBullet->getColliderDimensions().x / 2.0f);
            mGameState.player->addBullet(newBullet);
        }

    } else {
        mGameState.player->setPosition({mOrigin.x, mOrigin.y + 400.0f});
        mGameState.player->clearBullets();
    }

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

    if (mGameState.enemies.empty()) {
        mGameState.enemies.push_back(new Enemy({mOrigin.x, mOrigin.y - 400.0f},            // position
                                               {50.0f, 50.0f},                             // scale
                                               "./assets/tiny-spaceships/tinyShip16.png",  // texture file address
                                               ATLAS,                // single image or atlas?
                                               {3, 4},               // atlas dimensions
                                               enemyAnimationAtlas,  // actual atlas
                                               NPC                   // entity type));
                                               ));
        for (Enemy* enemy : mGameState.enemies) {
            enemy->setColliderRadius(25.0f);
            for (int i = 0; i < 2500; ++i) {
                Bullet* newBullet = new Bullet({mOrigin.x, mOrigin.y - 450.0f},  // position
                                               {50.0f, 50.0f},                   // scale
                                               "./assets/bullet.png",            // texture file address
                                               NPC                               // entity type)
                );
                newBullet->setColliderDimensions(
                    {newBullet->getColliderDimensions().x - 8.0f, newBullet->getColliderDimensions().y - 8.0f});
                newBullet->setColliderRadius(newBullet->getColliderDimensions().x / 2.0f);
                enemy->addBullet(newBullet);
            }
        }
    } else {
        for (Enemy* enemy : mGameState.enemies) {
            enemy->setElapsedTime(0.0f);
            enemy->setAttackDelay(0.0f);
            enemy->setHealth(100);
            enemy->activate();
            enemy->clearBullets();
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

void LevelB::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);

    mGameState.player->update(deltaTime,          // delta time / fixed timestep
                              nullptr,            // player
                              mGameState.map,     // map
                              mGameState.enemies  // enemies (vector)
    );

    if (mGameState.player->getHealth() <= 0) {
        mGameState.nextSceneID = 3;
    }

    bool enemiesAlive = false;
    for (Enemy* enemy : mGameState.enemies) {
        enemy->update(deltaTime, mGameState.player, nullptr, nullptr, 0);
        if (!enemy->isActive()) {
            enemy->updateBullets(deltaTime, mGameState.player, nullptr, nullptr, 0);
        } else {
            enemiesAlive = true;
        }
    }
    if (enemiesAlive == false) {
        mGameState.nextSceneID = 2;
    }
}

void LevelB::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.player->render();
    mGameState.player->renderBullets();
    for (Enemy* enemy : mGameState.enemies) {
        enemy->render();
    }
}

void LevelB::shutdown() {
    delete mGameState.player;
    for (Enemy* enemy : mGameState.enemies) {
        delete enemy;
    }

    UnloadMusicStream(mGameState.bgm);
}
