#include "Bullet.h"

Vector2 defaultPattern(float elapsedTime, Vector2 currVelocity) {
    return {0.0f, 5.0f};
}

Bullet::Bullet(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType)
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType),
      mPattern(defaultPattern) {
}

Bullet::Bullet(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType, Vector2 (*pattern)(float elapsedTime, Vector2 currVelocity))
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType),
      mPattern(pattern) {
}

void Bullet::update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                    int collisionCheckCount) {
    if (!isActive()) {
        return;
    }
    mElapsedTime += deltaTime;
    mVelocity = mPattern(mElapsedTime, mVelocity);
    mPosition.y += std::cos(mAngle * (3.1415f / 180.0f)) * mVelocity.y * deltaTime;
    mPosition.x += std::sin(mAngle * (3.1415f / 180.0f)) * mVelocity.x * deltaTime;
    if (mElapsedTime >= mTimeAlive) {
        deactivate();
    }
}

void Bullet::setPattern(Vector2 (*newPattern)(float deltaTime, Vector2 currVelocity)) {
    mPattern = newPattern;
}
