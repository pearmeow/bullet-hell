#include "LevelA.h"

#include "Bullet.h"

LevelA::LevelA() : Scene{{0.0f}, nullptr} {
}
LevelA::LevelA(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelA::~LevelA() {
    shutdown();
}

void LevelA::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.playerHitSound = LoadSound("./assets/synthetic_explosion_1.mp3");
    SetSoundVolume(mGameState.playerHitSound, 0.6f);

    mGameState.playerDeathSound = LoadSound("./assets/rumble.mp3");
    SetSoundVolume(mGameState.playerDeathSound, 0.8f);

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

        for (int i = 0; i < 1000; ++i) {
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

        mGameState.player->setBombs(999);

    } else {
        mGameState.player->setBombs(999);
        mGameState.player->activate();
        mGameState.player->setPosition({mOrigin.x, mOrigin.y + 400.0f});
        mGameState.player->setHealth(3);
        mGameState.player->clearBullets();
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

    if (!mGameState.player->isActive()) {
        mGameState.player->updateBullets(deltaTime, mGameState.enemies, nullptr, nullptr, 0);
    }
}

void LevelA::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("Touhou Clone", mOrigin.x - 100.0f, mOrigin.y - 300.0f, 30, WHITE);
    DrawText("WASD to move", mOrigin.x - 100.0f, mOrigin.y + 180.0f, 30, WHITE);
    DrawText("Q to bomb", mOrigin.x - 80.0f, mOrigin.y + 210.0f, 30, WHITE);
    DrawText("Shift to half speed", mOrigin.x - 140.0f, mOrigin.y + 240.0f, 30, WHITE);
    DrawText("Press ENTER to start", mOrigin.x - 170.0f, mOrigin.y + 270.0f, 30, WHITE);
    DrawText("The blue circle is your hitbox", mOrigin.x - 200.0f, mOrigin.y + 300.0f, 30, WHITE);
    mGameState.player->render();
    mGameState.player->renderBullets();
}

void LevelA::shutdown() {
    if (mGameState.player == nullptr) {
        return;
    }
    delete mGameState.player;
    delete mGameState.map;
    for (Enemy* enemy : mGameState.enemies) {
        delete enemy;
    }
    mGameState.enemies.clear();
}
