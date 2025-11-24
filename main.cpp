/**
 * Author: Perry Huang
 * Assignment: Bullet Hell
 * Date due: December 5th, 2:00pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include <raylib.h>

#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/LevelD.h"
#include "CS3113/Scene.h"
#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 1000, FPS = 120, NUMBER_OF_LEVELS = 6;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0};

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f, gTimeAccumulator = 0.0f;

std::vector<Scene*> gLevels;
Scene* gCurrentScene;
size_t gSceneIndex = 0;
LevelA* gLevelA;
LevelB* gLevelB;
LevelC* gLevelC;
LevelD* gLevelD;

// Function Declarations
void switchToScene(Scene* scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene* scene) {
    gCurrentScene = scene;
    gCurrentScene->initialise();
}

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();
    gLevelA = new LevelA(ORIGIN, "#000000");
    gLevelB = new LevelB(ORIGIN, "#000000");
    gLevelC = new LevelC(ORIGIN, "#000000");
    gLevelD = new LevelD(ORIGIN, "#000000");
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);
    gLevels.push_back(gLevelD);

    switchToScene(gLevels[gSceneIndex]);

    SetTargetFPS(FPS);
}

void processInput() {
    if (IsKeyDown(KEY_A)) {
        gCurrentScene->getState().player->moveLeft();
    }
    if (IsKeyDown(KEY_D)) {
        gCurrentScene->getState().player->moveRight();
    }
    if (IsKeyDown(KEY_W)) {
        gCurrentScene->getState().player->moveUp();
    }
    if (IsKeyDown(KEY_S)) {
        gCurrentScene->getState().player->moveDown();
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (gSceneIndex == 0 || gSceneIndex == 3) {
            gSceneIndex = (gSceneIndex + 1) % gLevels.size();
            switchToScene(gLevels[gSceneIndex]);
        } else if (gSceneIndex == 2) {
            gSceneIndex = 0;
            switchToScene(gLevels[gSceneIndex]);
        }
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP) {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render() {
    BeginDrawing();
    BeginMode2D(gCurrentScene->getState().camera);

    gCurrentScene->render();

    EndMode2D();
    EndDrawing();
}

void shutdown() {
    CloseAudioDevice();
    CloseWindow();
}

int main() {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID > 0) {
            gSceneIndex = gCurrentScene->getState().nextSceneID;
            switchToScene(gLevels[gSceneIndex]);
        }

        render();
    }

    shutdown();

    return 0;
}
