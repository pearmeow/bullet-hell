#include "Enemy.h"
#include "Player.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState {
    Player* player = nullptr;
    Map* map = nullptr;
    Music bgm;
    Sound playerHitSound;
    Sound enemyDeathSound;
    Sound playerDeathSound;
    Texture2D enemyHealthBar;
    Texture2D enemyHealthBarBackground;

    std::vector<Enemy*> enemies;
    Camera2D camera;
    float timeUntilNextScene = 5.0f;

    int nextSceneID;
};

class Scene {
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char* mBGColourHexCode = "#000000";

public:
    Scene();
    Scene(Vector2 origin, const char* bgHexCode);
    virtual ~Scene();

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState getState() const {
        return mGameState;
    }
    Vector2 getOrigin() const {
        return mOrigin;
    }
    const char* getBGColourHexCode() const {
        return mBGColourHexCode;
    }
};

#endif
