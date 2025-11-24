#include "LevelD.h"

LevelD::LevelD() : Scene{{0.0f}, nullptr} {
}
LevelD::LevelD(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelD::~LevelD() {
    shutdown();
}

void LevelD::initialise() {
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

void LevelD::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);
}

void LevelD::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
}

void LevelD::shutdown() {
    UnloadMusicStream(mGameState.bgm);
}
