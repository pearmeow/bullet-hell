#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity {
private:
    float (*mPattern)(float elapsedTime, float& angle);
    float mElapsedTime = 0.0f;
    float mTimeAlive = 50.0f;
    float mDelay = 0.0f;

public:
    // default pattern
    Bullet(Vector2 position, Vector2 scale, const char* textureFile, EntityType entityType);
    // provid a pattern
    Bullet(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
           Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
           EntityType entityType, float (*pattern)(float elapsedTime, float& angle));
    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                int collisionCheckCount) override;
    void setPattern(float (*mPattern)(float elapsedTime, float& angle));
    void setDelay(float delay);
    float getDelay();
};

#endif
