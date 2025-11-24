#ifndef LEVEL_D
#define LEVEL_D

#include "Scene.h"

// Lose screen
class LevelD : public Scene {
public:
    LevelD();
    LevelD(Vector2 origin, const char* bgHexCode);
    ~LevelD();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
