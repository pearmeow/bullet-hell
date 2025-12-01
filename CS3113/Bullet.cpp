#include "Bullet.h"

#include "Enemy.h"

float defaultPattern(Entity* player, Bullet* bullet) {
    return 5.0f;
}

Bullet::Bullet(Vector2 position, Vector2 scale, const char* textureFile, EntityType entityType)
    : Entity(position, scale, textureFile, entityType), mPattern(defaultPattern) {
}

Bullet::Bullet(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType, float (*pattern)(Entity* player, Bullet* bullet))
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType),
      mPattern(pattern) {
}

// for enemy bullets
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
    mSpeed = mPattern(player, this);
    mPosition.y += std::cos(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;
    mPosition.x += std::sin(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;
    if (mElapsedTime >= mTimeAlive) {
        deactivate();
    }
}

// for player bullets
void Bullet::update(float deltaTime, std::vector<Enemy*>& enemies, Map* map, Entity* collidableEntities,
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
    mSpeed = mPattern(nullptr, this);
    mPosition.y += std::cos(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;
    mPosition.x += std::sin(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;
    for (Enemy* enemy : enemies) {
        if (isCollidingEnemy(enemy)) {
            // enemy loses health
            enemy->setHealth(enemy->getHealth() - 1);
            deactivate();
        }
    }
    if (mElapsedTime >= mTimeAlive) {
        deactivate();
    }
}

bool Bullet::isCollidingEnemy(Enemy* other) const {
    if (!other->isActive()) return false;

    float xDistance = fabs(mPosition.x - other->getPosition().x);
    float yDistance = fabs(mPosition.y - other->getPosition().y);

    float distance = xDistance * xDistance + yDistance * yDistance;
    float colliderDistance = mColliderRadius + other->getColliderRadius();

    if (distance < colliderDistance * colliderDistance) return true;

    return false;
}

void Bullet::setPattern(float (*newPattern)(Entity* player, Bullet* bullet)) {
    mPattern = newPattern;
}

void Bullet::setDelay(float delay) {
    mDelay = delay;
}

float Bullet::getDelay() {
    return mDelay;
}

float Bullet::getElapsedTime() {
    return mElapsedTime;
}

void Bullet::setTimeAlive(float timeAlive) {
    mTimeAlive = timeAlive;
}

void Bullet::setElapsedTime(float elapsedTime) {
    mElapsedTime = elapsedTime;
}
