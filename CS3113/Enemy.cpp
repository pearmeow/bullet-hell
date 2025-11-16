#include "Enemy.h"

#include "Entity.h"
#include "cs3113.h"

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
    for (Bullet* bullet : mBullets) {
        bullet->update(deltaTime, nullptr, nullptr, nullptr, 0);
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
