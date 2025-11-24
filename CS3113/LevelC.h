#ifndef LEVEL_A
#define LEVEL_A

#include "Scene.h"

class LevelC : public Scene {
private:
    static constexpr int LEVEL_WIDTH = 6;
    static constexpr int LEVEL_HEIGHT = 7;
    // clang-format off
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1
    };
    // clang-format on

public:
    static constexpr float TILE_DIMENSION = 200.0f;

    LevelC();
    LevelC(Vector2 origin, const char* bgHexCode);
    ~LevelC();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
