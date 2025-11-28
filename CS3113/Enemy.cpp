#include "Enemy.h"

#include <cmath>

#include "Entity.h"
#include "cs3113.h"

float wavyPattern(Entity* player, Bullet* bullet);
float fastPattern(Entity* player, Bullet* bullet);
float homingPattern(Entity* player, Bullet* bullet);

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
        if (!(*bullet)->isActive() && (*bullet)->getDelay() <= 0.0f) {
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
        // conditional based on mElapsedTime to switch up attacks
        mAttackDelay = 0.0f;
        if (std::sin(mElapsedTime * 100 * 3.14 / 180.0f) >= 0) {
            // one attack
            splitAttack(0, 10, fastPattern);
            delayedAttack(-90, 40, 0.2f, fastPattern);
        } else {
            // another attack
            splitAttack(0, 10, fastPattern);
            splitAttack(mElapsedTime * 100, 10, wavyPattern);
        }
    }
}

void Enemy::delayedAttack(float initAngle, int attacks, float delay,
                          float (*pattern)(Entity* player, Bullet* bullet)) {
    float angle = initAngle;
    float step = 360.0f / attacks;
    for (int count = 0; count < attacks; ++count) {
        float nextAngle = angle + count * step;
        Bullet* nextBullet = mInactiveBullets.front();
        nextBullet->setPosition({mPosition.x + (5.0f + mScale.x) * std::sin(nextAngle * 3.14f / 180.0f),
                                 mPosition.y + (5.0f + mScale.y) * std::cos(nextAngle * 3.14f / 180.0f)});
        nextBullet->setAngle(nextAngle);
        nextBullet->deactivate();
        nextBullet->setDelay(delay * count);
        nextBullet->setPattern(pattern);
        mBullets.push_back(nextBullet);
        mInactiveBullets.pop();
    }
}

void Enemy::splitAttack(float initAngle, int attacks, float (*pattern)(Entity* player, Bullet* bullet)) {
    float angle = initAngle;
    float step = 360.0f / attacks;
    for (int count = 0; count < attacks; ++count) {
        float nextAngle = angle + count * step;
        Bullet* nextBullet = mInactiveBullets.front();
        nextBullet->setPosition({mPosition.x + (5.0f + mScale.x) * std::sin(nextAngle * 3.14f / 180.0f),
                                 mPosition.y + (5.0f + mScale.y) * std::cos(nextAngle * 3.14f / 180.0f)});
        nextBullet->setAngle(nextAngle);
        nextBullet->setPattern(pattern);
        mBullets.push_back(nextBullet);
        mInactiveBullets.pop();
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

float fastPattern(Entity* player, Bullet* bullet) {
    return 90.0f;
}

float wavyPattern(Entity* player, Bullet* bullet) {
    if (std::cos(bullet->getElapsedTime() * 75 * 3.14 / 180.0f) > 0) {
        bullet->setAngle(bullet->getAngle() + 0.5f);
    } else {
        bullet->setAngle(bullet->getAngle() - 0.5f);
    }
    return 30.0f;
}
