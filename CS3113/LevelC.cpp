#include "LevelC.h"

LevelC::LevelC() : Scene{{0.0f}, nullptr} {
}
LevelC::LevelC(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelC::~LevelC() {
    shutdown();
}

void LevelC::initialise() {
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

void LevelC::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);
}

void LevelC::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("GGEZ", mOrigin.x - 100.0f, mOrigin.y - 100.0f, 30, WHITE);
    DrawText("Press ENTER to play again", mOrigin.x - 170.0f, mOrigin.y + 400.0f, 30, WHITE);
}

void LevelC::shutdown() {
    UnloadMusicStream(mGameState.bgm);
}
