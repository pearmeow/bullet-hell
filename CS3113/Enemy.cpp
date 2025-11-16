#include "Enemy.h"

#include "Entity.h"
#include "cs3113.h"

Vector2 straightPattern(float deltaTime, Vector2 currVelocity);

Enemy::Enemy(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
             Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
             EntityType entityType)
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType) {
    mAngle = 180.0f;
}

void Enemy::update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                   int collisionCheckCount) {
    if (mEntityStatus == INACTIVE) {
        return;
    }
    if (GetLength(mMovement) > 1) {
        normaliseMovement();
    }
    resetColliderFlags();
    mVelocity.x = mMovement.x * mSpeed;
    mVelocity.y = mMovement.y * mSpeed;

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    checkCollisionX(map);

    if (mTextureType == ATLAS) animate(deltaTime);
    resetMovement();
    for (auto bullet = mBullets.begin(); bullet != mBullets.end(); ++bullet) {
        (*bullet)->update(deltaTime, nullptr, nullptr, nullptr, 0);
        if (!(*bullet)->isActive()) {
            // bullet goes back to inactive
            mInactiveBullets.push(*bullet);
            // erase and go to the next one: erase returns the next iterator
            bullet = mBullets.erase(bullet);
            // go back one because iterator gets incremented at the end of the for loop
            --bullet;
        }
    }
    mAttackDelay += deltaTime;
    if (mAttackDelay >= mAttackSpeed) {
        mAttackDelay = 0.0f;
        float rotation = 30.0f;
        for (int count = 0; count < mAttacks; ++count) {
            Bullet* nextBullet = mInactiveBullets.front();
            nextBullet->setPosition({mPosition.x, mPosition.y + mScale.y + 5.0f});
            nextBullet->setAngle(rotation);
            nextBullet->setPattern(straightPattern);
            rotation -= 10.0f;
            mBullets.push_back(nextBullet);
            mInactiveBullets.pop();
        }
    }
}

void Enemy::render() {
    Entity::render();
    for (Bullet* bullet : mBullets) {
        bullet->render();
    }
}

std::list<Bullet*>& Enemy::getBullets() {
    return mBullets;
}

Vector2 straightPattern(float elapsedTime, Vector2 currVelocity) {
    if (elapsedTime >= 10.0f) {
        return {0.0f, 30.0f};
    } else {
        return {0.0f, 10.0f};
    }
}
