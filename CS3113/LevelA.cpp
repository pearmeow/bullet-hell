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
}

void LevelA::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("Touhou Clone", mOrigin.x - 100.0f, mOrigin.y - 300.0f, 30, WHITE);
    DrawText("WASD to move", mOrigin.x - 100.0f, mOrigin.y + 300.0f, 30, WHITE);
    DrawText("Press ENTER to start", mOrigin.x - 170.0f, mOrigin.y + 400.0f, 30, WHITE);

    // TODO: Make image in middle possibly if time allows
}

void LevelA::shutdown() {
    UnloadMusicStream(mGameState.bgm);
}
