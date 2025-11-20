#include "Enemy.h"

#include <cmath>

#include "Entity.h"
#include "cs3113.h"

float straightPattern(float elapsedTime, float& angle);
float fastPattern(float elapsedTime, float& angle);

Enemy::Enemy(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
             Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
             EntityType entityType)
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType) {
    mAngle = 180.0f;
}

Enemy::~Enemy() {
    while (!mInactiveBullets.empty()) {
        mBullets.push_back(mInactiveBullets.front());
        mInactiveBullets.pop();
    }
    for (Bullet* activeBullet : mBullets) {
        delete activeBullet;
    }
    mBullets.clear();
}

void Enemy::update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                   int collisionCheckCount) {
    if (mEntityStatus == INACTIVE) {
        return;
    }
    if (GetLength(mMovement) > 1) {
        normaliseMovement();
    }
    mElapsedTime += deltaTime;
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
        // conditional based on deltaTime to switch up attacks
        mAttackDelay = 0.0f;
        if (mElapsedTime >= 10) {
            // one attack
            float rotation = 0.0f;
            float step = 360.0f / mAttacks;
            for (int count = 0; count < mAttacks; ++count) {
                float nextRotation = rotation + count * step;
                Bullet* nextBullet = mInactiveBullets.front();
                nextBullet->setPosition(
                    {mPosition.x + (5.0f + mScale.x) * std::sin(nextRotation * 3.14f / 180.0f),
                     mPosition.y + (5.0f + mScale.y) * std::cos(nextRotation * 3.14f / 180.0f)});
                nextBullet->setAngle(nextRotation);
                nextBullet->setPattern(straightPattern);
                mBullets.push_back(nextBullet);
                mInactiveBullets.pop();
            }
        } else {
            // another attack
            float rotation = 0.0f;
            float step = 360.0f / mAttacks;
            for (int count = 0; count < mAttacks * 1.5; ++count) {
                float nextRotation = rotation + count * step;
                Bullet* nextBullet = mInactiveBullets.front();
                nextBullet->setPosition(
                    {mPosition.x + (5.0f + mScale.x) * std::sin(nextRotation * 3.14f / 180.0f),
                     mPosition.y + (5.0f + mScale.y) * std::cos(nextRotation * 3.14f / 180.0f)});
                nextBullet->setAngle(nextRotation);
                nextBullet->setPattern(fastPattern);
                mBullets.push_back(nextBullet);
                mInactiveBullets.pop();
            }
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

void Enemy::addBullet(Bullet* bullet) {
    mInactiveBullets.push(bullet);
}

float fastPattern(float elapsedTime, float& angle) {
    return 90.0f;
}

float straightPattern(float elapsedTime, float& angle) {
    if (std::cos(elapsedTime * 75 * 3.14 / 180.0f) > 0) {
        angle += 0.5f;
    } else {
        angle -= 0.5f;
    }
    return 30.0f;
}
