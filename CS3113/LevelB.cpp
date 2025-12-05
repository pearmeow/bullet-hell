#include "LevelB.h"

#include <raylib.h>

#include "Bullet.h"
#include "Enemy.h"
#include "Entity.h"
#include "Player.h"

float eightMovePattern(Entity* player, Enemy* enemy);

LevelB::LevelB() : Scene{{0.0f}, nullptr} {
}
LevelB::LevelB(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelB::~LevelB() {
    shutdown();
}

void LevelB::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.bgm = LoadMusicStream("./assets/dova_Mauve Rhopalocera_master.mp3");
    SetMusicVolume(mGameState.bgm, 0.25f);
    PlayMusicStream(mGameState.bgm);

    mGameState.playerHitSound = LoadSound("./assets/synthetic_explosion_1.mp3");
    SetSoundVolume(mGameState.playerHitSound, 0.6f);

    mGameState.playerDeathSound = LoadSound("./assets/rumble.mp3");
    SetSoundVolume(mGameState.playerDeathSound, 0.8f);

    mGameState.enemyDeathSound = LoadSound("./assets/Multi Explosion.mp3");
    SetSoundVolume(mGameState.enemyDeathSound, 0.8f);

    mGameState.enemyHealthBar = LoadTexture("./assets/greenBox.png");
    mGameState.enemyHealthBarBackground = LoadTexture("./assets/redBox.png");

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

        for (int i = 0; i < 5000; ++i) {
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
        mGameState.player->setBombs(2);
        mGameState.player->activate();
        mGameState.player->setPosition({mOrigin.x, mOrigin.y + 400.0f});
        mGameState.player->setHealth(3);
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
        mGameState.enemies[0]->setMovePattern(eightMovePattern);
        mGameState.enemies[0]->setHealth(275);
    } else {
        for (Enemy* enemy : mGameState.enemies) {
            enemy->setElapsedTime(0.0f);
            enemy->setAttackDelay(0.0f);
            enemy->setHealth(275);
            enemy->activate();
            enemy->clearBullets();
        }
        mGameState.enemies[0]->setMovePattern(eightMovePattern);
        mGameState.enemies[0]->setPosition({mOrigin.x, mOrigin.y - 400.0f});
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
    if (mGameState.player->isPaused()) return;

    if (mGameState.player->isBombing()) {
        for (Enemy* enemy : mGameState.enemies) {
            enemy->clearBullets();
        }
        PlaySound(mGameState.playerDeathSound);
        mGameState.player->setBombing(false);
    }

    int playerHealth = mGameState.player->getHealth();

    mGameState.player->update(deltaTime,       // delta time / fixed timestep
                              nullptr,         // player
                              mGameState.map,  // map
                              mGameState.enemies);

    if (mGameState.player->getHealth() <= 0 && mGameState.player->isActive()) {
        // play death sound
        mGameState.player->deactivate();
        PlaySound(mGameState.playerDeathSound);

        // start delay, then switch later
    } else if (mGameState.player->getHealth() < playerHealth) {
        PlaySound(mGameState.playerHitSound);
    }

    bool enemiesAlive = false;
    for (Enemy* enemy : mGameState.enemies) {
        bool wasAlive = enemy->isActive();
        enemy->update(deltaTime, mGameState.player, nullptr, nullptr, 0);
        if (wasAlive && !enemy->isActive()) {
            PlaySound(mGameState.enemyDeathSound);
        }
        if (!enemy->isActive()) {
            enemy->updateBullets(deltaTime, mGameState.player, nullptr, nullptr, 0);
        } else {
            enemiesAlive = true;
        }
    }

    if (!mGameState.player->isActive()) {
        mGameState.player->updateBullets(deltaTime, mGameState.enemies, nullptr, nullptr, 0);
    }

    // detect win/loss
    if (enemiesAlive == false || !mGameState.player->isActive()) {
        mGameState.timeUntilNextScene -= deltaTime;
        if (mGameState.timeUntilNextScene <= 0) {
            mGameState.timeUntilNextScene = 5.0f;
            if (!mGameState.player->isActive()) {
                mGameState.nextSceneID = 3;
            } else if (mGameState.player->isActive()) {
                mGameState.nextSceneID = 2;
            }
        }
    }
}

void LevelB::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.player->render();
    mGameState.player->renderBullets();
    for (Enemy* enemy : mGameState.enemies) {
        enemy->render();
    }

    DrawTexturePro(mGameState.enemyHealthBarBackground,
                   {0.0f, 0.0f, (float)mGameState.enemyHealthBarBackground.width,
                    (float)mGameState.enemyHealthBarBackground.height},
                   {0.0f, 0.0f, 100.0f, 10.0f}, {0.0f, 0.0f}, 0.0f, WHITE);
    DrawTexturePro(mGameState.enemyHealthBar,
                   {0.0f, 0.0f, (float)mGameState.enemyHealthBar.width, (float)mGameState.enemyHealthBar.height},
                   {0.0f, 0.0f, mGameState.enemies[0]->getHealth() / 275.0f * 100.0f, 10.0f}, {0.0f, 0.0f}, 0.0f,
                   WHITE);
}

void LevelB::shutdown() {
    // no point in deleting uninitialized stuff
    if (mGameState.player == nullptr) {
        return;
    }
    delete mGameState.player;
    delete mGameState.map;
    for (Enemy* enemy : mGameState.enemies) {
        delete enemy;
    }
    mGameState.enemies.clear();
    UnloadMusicStream(mGameState.bgm);
}

float eightMovePattern(Entity* player, Enemy* enemy) {
    float elapsedTime = enemy->getElapsedTime();
    float angle = 180 * std::cos(5 * elapsedTime * 3.1415 / 180.0f);
    enemy->setAngle(angle);
    return 5.0f;
}
