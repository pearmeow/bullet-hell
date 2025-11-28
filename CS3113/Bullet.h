#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity {
private:
    float (*mPattern)(Entity* player, Bullet* bullet);
    float mElapsedTime = 0.0f;
    float mTimeAlive = 35.0f;
    float mDelay = 0.0f;

public:
    // default pattern
    Bullet(Vector2 position, Vector2 scale, const char* textureFile, EntityType entityType);
    // provid a pattern
    Bullet(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
           Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
           EntityType entityType, float (*pattern)(Entity* player, Bullet* bullet));
    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                int collisionCheckCount) override;
    void setPattern(float (*mPattern)(Entity* player, Bullet* bullet));
    void setDelay(float delay);
    float getDelay();
    float getElapsedTime();
};

#endif
