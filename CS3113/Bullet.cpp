#include "Bullet.h"

float defaultPattern(float elapsedTime) {
    return 5.0f;
}

Bullet::Bullet(Vector2 position, Vector2 scale, const char* textureFile, EntityType entityType)
    : Entity(position, scale, textureFile, entityType), mPattern(defaultPattern) {
}

Bullet::Bullet(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType, float (*pattern)(float))
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType),
      mPattern(pattern) {
}

void Bullet::update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                    int collisionCheckCount) {
    if (!isActive()) {
        return;
    }
    mElapsedTime += deltaTime;
    mSpeed = mPattern(mElapsedTime);
    mPosition.y += std::cos(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;
    mPosition.x += std::sin(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;
    if (mElapsedTime >= mTimeAlive) {
        deactivate();
    }
}

void Bullet::setPattern(float (*newPattern)(float elapsedTime)) {
    mPattern = newPattern;
}
