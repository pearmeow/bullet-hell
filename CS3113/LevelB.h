#ifndef LEVEL_B
#define LEVEL_B

#include "Scene.h"

class LevelB : public Scene {
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

    LevelB();
    LevelB(Vector2 origin, const char* bgHexCode);
    ~LevelB();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
