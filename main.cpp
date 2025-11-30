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
#include "CS3113/ShaderProgram.h"
#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 1000, FPS = 120, NUMBER_OF_LEVELS = 6;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0};

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f, gTimeAccumulator = 0.0f;
float gInvertTicks = 0.5f;

std::vector<Scene*> gLevels;
Scene* gCurrentScene;
size_t gSceneIndex = 0;
LevelA* gLevelA;
LevelB* gLevelB;
LevelC* gLevelC;
LevelD* gLevelD;

ShaderProgram gShader;
int gInvert = 0;
Effects* gEffects = nullptr;

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

    gShader.load("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    gEffects = new Effects(ORIGIN, SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT * 1.5f);

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
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        if (gCurrentScene->getState().player != nullptr) {
            gCurrentScene->getState().player->setShifted(true);
        }
    }

    if (IsKeyDown(KEY_A)) {
        if (gCurrentScene->getState().player != nullptr) {
            gCurrentScene->getState().player->moveLeft();
        }
    }
    if (IsKeyDown(KEY_D)) {
        if (gCurrentScene->getState().player != nullptr) {
            gCurrentScene->getState().player->moveRight();
        }
    }
    if (IsKeyDown(KEY_W)) {
        if (gCurrentScene->getState().player != nullptr) {
            gCurrentScene->getState().player->moveUp();
        }
    }
    if (IsKeyDown(KEY_S)) {
        if (gCurrentScene->getState().player != nullptr) {
            gCurrentScene->getState().player->moveDown();
        }
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

    if (IsKeyPressed(KEY_Q) && gCurrentScene->getState().player) {
        if (gCurrentScene->getState().player->bomb()) {
            gInvert = 1;
        }
    } else {
        gInvert = 0;
    }

    if (WindowShouldClose()) gAppStatus = TERMINATED;
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
        if (gInvert || gInvertTicks != 0.5f) {
            gInvert = 1;
            gInvertTicks -= FIXED_TIMESTEP;
        }
        if (gInvertTicks <= 0) {
            gInvert = 0;
            gInvertTicks = 0.5f;
        }
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render() {
    BeginDrawing();
    BeginMode2D(gCurrentScene->getState().camera);

    gShader.begin();
    // do some shader stuff
    gShader.setInt("invert", gInvert);
    gCurrentScene->render();
    gShader.end();

    EndMode2D();
    EndDrawing();
}

void shutdown() {
    for (size_t i = 0; i < gLevels.size(); ++i) {
        delete gLevels[i];
    }
    delete gEffects;
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
