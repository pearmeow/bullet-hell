#include "Enemy.h"

#include <raylib.h>

#include <cmath>

#include "Bullet.h"
#include "Entity.h"
#include "cs3113.h"

float wavyPattern(Entity* player, Bullet* bullet);
float fastPattern(Entity* player, Bullet* bullet);
float homingPattern(Entity* player, Bullet* bullet);
float straightPattern(Entity* player, Bullet* bullet);
float trackingPattern(Entity* player, Bullet* bullet);
float rotatingPattern(Entity* player, Bullet* bullet);
float reverseRotatingPattern(Entity* player, Bullet* bullet);

float stillMovePattern(Entity* player, Enemy* enemy);

Enemy::Enemy(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
             Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
             EntityType entityType)
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType),
      mMovePattern(stillMovePattern) {
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
    if (mHealth <= 0) {
        deactivate();
    }
    if (mEntityStatus == INACTIVE) {
        return;
    }
    if (GetLength(mMovement) > 1) {
        normaliseMovement();
    }
    if (mElapsedTime == 0.0f) {
        mAttackSpeed = 2.0f;
    }
    mElapsedTime += deltaTime;
    resetColliderFlags();
    mSpeed = mMovePattern(player, this);
    mPosition.y += -1.0f * std::cos(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;
    mPosition.x += std::sin(mAngle * (3.1415f / 180.0f)) * mSpeed * deltaTime;

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    checkCollisionX(map);

    // just to make the animation play because it's the same for all directions
    if (mSpeed > 0) {
        mDirection = DOWN;
    }
    if (mTextureType == ATLAS) animate(deltaTime);
    resetMovement();
    updateBullets(deltaTime, player, map, collidableEntities, collisionCheckCount);
    mAttackDelay += deltaTime;
    if (mAttackDelay >= mAttackSpeed) {
        // conditional based on mElapsedTime to switch up attacks
        mAttackDelay = 0.0f;
        if (mElapsedTime <= 20.0f) {
            delayedAttack(-90, 40, 0.2f, 360.0f / 40, fastPattern);
            splitAttack(0, 15, wavyPattern);
            splitAttack(0, 10, homingPattern);
        } else if (mElapsedTime <= 30.0f) {
            mAttackSpeed = 1.3f;
            splitAttack(mElapsedTime * 5, 30, fastPattern);
            delayedAttack(0.0f, 1, 0, 360.0f / 1.0f, trackingPattern);
        } else if (mElapsedTime <= 40.0f) {
            mAttackSpeed = 1.0f;
            splitAttack(mElapsedTime * 3, 45, fastPattern);
            delayedAttack(-90.0f, 3, 0.3f, 360.0f / 5.0f, trackingPattern);
        } else if (mElapsedTime <= 50.0f) {
            delayedAttack(-87.5f, 35, 0.2f, 5.5f, fastPattern);
        } else if (mElapsedTime <= 58.0f) {
        } else if (mElapsedTime <= 70.0f) {
            delayedAttack(0, 10, 0.3f, 0, trackingPattern);
        } else if (mElapsedTime <= 80.0f) {
            delayedAttack(50.0f, 5, 0.2f, 5.0f, reverseRotatingPattern);
            delayedAttack(-50.0f, 5, -0.2f, -5.0f, rotatingPattern);
        } else if (mElapsedTime <= 95.0f) {
            splitAttack(0, 20, homingPattern);
        } else if (mElapsedTime <= 90.0f) {
            splitAttack(0.0f, 9, rotatingPattern);
        } else if (mElapsedTime <= 100.0f) {
            splitAttack(0.0f, 9, reverseRotatingPattern);
        } else if (mElapsedTime <= 120.0f) {
            splitAttack(0.0f, 9, rotatingPattern);
            splitAttack(0.0f, 9, reverseRotatingPattern);
            delayedAttack(-87.5f, 35, 0.2f, 5.5f, fastPattern);
        } else if (mElapsedTime <= 130.0f) {
            // better kill him quick
            mAttackSpeed = 0.2f;
            delayedAttack(-90, 40, 0.2f, 360.0f / 40, fastPattern);
            splitAttack(0, 15, wavyPattern);
            splitAttack(0, 10, homingPattern);
        }
    }
}

void Enemy::delayedAttack(float initAngle, int attacks, float delay, float step,
                          float (*pattern)(Entity* player, Bullet* bullet)) {
    float angle = initAngle;
    for (int count = 0; count < attacks; ++count) {
        float nextAngle = angle + count * step;
        Bullet* nextBullet = mInactiveBullets.front();
        nextBullet->setPosition({mPosition.x + (5.0f + mScale.x) * std::sin(nextAngle * 3.14f / 180.0f),
                                 mPosition.y + (5.0f + mScale.y) * std::cos(nextAngle * 3.14f / 180.0f)});
        nextBullet->setAngle(nextAngle);
        nextBullet->deactivate();
        nextBullet->setDelay(delay * count);
        nextBullet->setElapsedTime(0.0f);
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
        nextBullet->setDelay(0.0f);
        nextBullet->setElapsedTime(0.0f);
        nextBullet->activate();
        mBullets.push_back(nextBullet);
        mInactiveBullets.pop();
    }
}

void Enemy::render() {
    for (Bullet* bullet : mBullets) {
        bullet->render();
    }
    Entity::render();
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
    float angle = bullet->getAngle();
    float elapsedTime = bullet->getElapsedTime();
    if (std::cos(elapsedTime * 75 * 3.14 / 180.0f) > 0) {
        bullet->setAngle(angle + 0.5f);
    } else {
        bullet->setAngle(angle - 0.5f);
    }
    return 30.0f;
}

float homingPattern(Entity* player, Bullet* bullet) {
    if (bullet->getElapsedTime() > 5.0f) {
        return 90.0f;
    }
    float angle = 0.0f;
    Vector2 playerPos = player->getPosition();
    Vector2 bulletPos = bullet->getPosition();
    // do some angle calculations
    Vector2 difference = {playerPos.x - bulletPos.x, playerPos.y - bulletPos.y};
    angle = 180.0f / 3.1415 * atan2f(difference.y, difference.x) - 90.0f;
    angle *= -1.0f;
    bullet->setAngle(angle);
    return 30.0f;
}

void Enemy::setHealth(int newHealth) {
    mHealth = newHealth;
}
int Enemy::getHealth() const {
    return mHealth;
}

void Enemy::updateBullets(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                          int collisionCheckCount) {
    for (auto bullet = mBullets.begin(); bullet != mBullets.end(); ++bullet) {
        (*bullet)->update(deltaTime, player, nullptr, nullptr, 0);
        if (!(*bullet)->isActive() && (*bullet)->getDelay() <= 0.0f) {
            // bullet goes back to inactive
            mInactiveBullets.push(*bullet);
            // erase and go to the next one: erase returns the next iterator
            bullet = mBullets.erase(bullet);
            // go back one because iterator gets incremented at the end of the for loop
            --bullet;
        }
    }
}

void Enemy::setAttackDelay(float newAttackDelay) {
    mAttackDelay = newAttackDelay;
}

void Enemy::setElapsedTime(float newElapsedTime) {
    mElapsedTime = newElapsedTime;
}

void Enemy::clearBullets() {
    while (!mBullets.empty()) {
        Bullet* curr = mBullets.front();
        curr->deactivate();
        curr->setTimeAlive(35.0f);
        curr->setElapsedTime(0.0f);
        curr->setDelay(0.0f);
        mInactiveBullets.push(curr);
        mBullets.pop_front();
    }
}

float Enemy::getElapsedTime() const {
    return mElapsedTime;
}

void Enemy::setMovePattern(float (*pattern)(Entity* player, Enemy* enemy)) {
    mMovePattern = pattern;
}

float stillMovePattern(Entity* player, Enemy* enemy) {
    return 0.0f;
}

float straightPattern(Entity* player, Bullet* bullet) {
    return 90.0f;
}

float trackingPattern(Entity* player, Bullet* bullet) {
    if (bullet->getElapsedTime() > 0.1f) {
        return 190.0f;
    }
    float angle = 0.0f;
    Vector2 playerPos = player->getPosition();
    Vector2 bulletPos = bullet->getPosition();
    // do some angle calculations
    Vector2 difference = {playerPos.x - bulletPos.x, playerPos.y - bulletPos.y};
    angle = 180.0f / 3.1415 * atan2f(difference.y, difference.x) - 90.0f;
    angle *= -1.0f;
    bullet->setAngle(angle);
    return 160.0f;
}

float rotatingPattern(Entity* player, Bullet* bullet) {
    float angle = bullet->getAngle();
    bullet->setAngle(angle + 0.2f);
    return 90.0f;
}

float reverseRotatingPattern(Entity* player, Bullet* bullet) {
    float angle = bullet->getAngle();
    bullet->setAngle(angle - 0.2f);
    return 90.0f;
}
