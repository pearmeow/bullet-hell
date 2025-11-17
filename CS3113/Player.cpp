#include "Player.h"

#include "Enemy.h"
#include "Entity.h"
#include "cs3113.h"

Player::Player(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType)
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType) {
}

// unused, just makes linker happy because of virtual
void Player::update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                    int collisionCheckCount) {
    printf("Bruh you're not supposed to use this one.\n");
}

void Player::update(float deltaTime, Entity* player, Map* map, std::vector<Enemy*>& enemies) {
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
    if (mIframes == 0.0f) {
        checkCollisionY(enemies);
    }
    Entity::checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    if (mIframes == 0.0f) {
        checkCollisionX(enemies);
    }
    Entity::checkCollisionX(map);

    mIframes -= deltaTime;
    if (mIframes < 0.0f) {
        mIframes = 0.0f;
    }

    if (mTextureType == ATLAS) animate(deltaTime);
    resetMovement();
}

void Player::checkCollisionY(std::vector<Enemy*>& entities) {
    for (Enemy* collidableEntity : entities) {
        for (Bullet* bullet : collidableEntity->getBullets()) {
            if (mIframes > 0.0f) {
                break;
            }
            if (isColliding(bullet)) {
                mIframes = 2.0f;
                break;
            }
        }
        if (isColliding(collidableEntity)) {
            // STEP 2: Calculate the distance between its centre and our centre
            //         and use that to calculate the amount of overlap between
            //         both bodies.
            float yDistance = fabs(mPosition.y - collidableEntity->getPosition().y);
            float yOverlap = fabs(yDistance - (mColliderDimensions.y / 2.0f) -
                                  (collidableEntity->getColliderDimensions().y / 2.0f));

            // STEP 3: "Unclip" ourselves from the other entity, and zero our
            //         vertical velocity.
            if (mVelocity.y > 0) {
                mPosition.y -= yOverlap;
                mVelocity.y = 0;
                mIsCollidingBottom = true;
            } else if (mVelocity.y < 0) {
                mPosition.y += yOverlap;
                mVelocity.y = 0;
                mIsCollidingTop = true;
            }
        }
    }
}

void Player::checkCollisionX(std::vector<Enemy*>& entities) {
    for (Enemy* collidableEntity : entities) {
        for (Bullet* bullet : collidableEntity->getBullets()) {
            if (mIframes > 0.0f) {
                break;
            }
            if (isColliding(bullet)) {
                mIframes = 10.0f;
                break;
            }
        }
        if (isColliding(collidableEntity)) {
            // When standing on a platform, we're always slightly overlapping
            // it vertically due to gravity, which causes false horizontal
            // collision detections. So the solution I found is only resolve X
            // collisions if there's significant Y overlap, preventing the
            // platform we're standing on from acting like a wall.
            float yDistance = fabs(mPosition.y - collidableEntity->getPosition().y);
            float yOverlap = fabs(yDistance - (mColliderDimensions.y / 2.0f) -
                                  (collidableEntity->getColliderDimensions().y / 2.0f));

            // Skip if barely touching vertically (standing on platform)
            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(mPosition.x - collidableEntity->getPosition().x);
            float xOverlap = fabs(xDistance - (mColliderDimensions.x / 2.0f) -
                                  (collidableEntity->getColliderDimensions().x / 2.0f));

            if (mVelocity.x > 0) {
                mPosition.x -= xOverlap;
                mVelocity.x = 0;

                // Collision!
                mIsCollidingRight = true;
            } else if (mVelocity.x < 0) {
                mPosition.x += xOverlap;
                mVelocity.x = 0;

                // Collision!
                mIsCollidingLeft = true;
            }
        }
    }
}
