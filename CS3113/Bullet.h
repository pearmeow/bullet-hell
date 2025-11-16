#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity {
private:
    Vector2 (*mPattern)(float elapsedTime, Vector2 currVelocity);
    float mElapsedTime = 0.0f;
    float mTimeAlive = 10.0f;

public:
    // default pattern
    Bullet(Vector2 position, Vector2 scale, const char* textureFile, EntityType entityType);
    // provid a pattern
    Bullet(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
           Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
           EntityType entityType, Vector2 (*pattern)(float deltaTime, Vector2 currVelocity));
    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                int collisionCheckCount) override;
    void setPattern(Vector2 (*newPattern)(float deltaTime, Vector2 currVelocity));
};

#endif
