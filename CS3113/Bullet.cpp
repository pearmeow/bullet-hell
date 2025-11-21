#include "Bullet.h"

float defaultPattern(float elapsedTime, float& angle) {
    return 5.0f;
}

Bullet::Bullet(Vector2 position, Vector2 scale, const char* textureFile, EntityType entityType)
    : Entity(position, scale, textureFile, entityType), mPattern(defaultPattern) {
}

Bullet::Bullet(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType, float (*pattern)(float elapsedTime, float& angle))
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType),
      mPattern(pattern) {
}

void Bullet::update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                    int collisionCheckCount) {
    mDelay -= deltaTime;
    if (mDelay <= 0.0f) {
        activate();
        mDelay = 0;
    } else {
        deactivate();
    }
    if (!isActive()) {
        return;
    }
    mElapsedTime += deltaTime;
    mSpeed = mPattern(mElapsedTime, mAngle);
    mPosition.y += std::cos(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;
    mPosition.x += std::sin(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;
    if (mElapsedTime >= mTimeAlive) {
        deactivate();
    }
}

void Bullet::setPattern(float (*newPattern)(float elapsedTime, float& angle)) {
    mPattern = newPattern;
}

void Bullet::setDelay(float delay) {
    mDelay = delay;
}

float Bullet::getDelay() {
    return mDelay;
}
