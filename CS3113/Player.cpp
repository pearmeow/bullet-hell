#include "Player.h"

#include "Entity.h"
#include "cs3113.h"

Player::Player(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType)
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType) {
}

void Player::update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                    int collisionCheckCount) {
    if (mEntityStatus == INACTIVE) {
        return;
    }
    mVelocity.x = mMovement.x * mSpeed;
    mVelocity.y = mMovement.y * mSpeed;

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    checkCollisionX(map);

    if (mTextureType == ATLAS && GetLength(mMovement) != 0) animate(deltaTime);
}
