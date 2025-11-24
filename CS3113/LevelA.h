#ifndef LEVEL_A
#define LEVEL_A

#include "Scene.h"

// Main menu scene with instructions and press enter to start
class LevelA : public Scene {
public:
    LevelA();
    LevelA(Vector2 origin, const char* bgHexCode);
    ~LevelA();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
