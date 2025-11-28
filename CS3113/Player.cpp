#include "Player.h"

#include "Bullet.h"
#include "Enemy.h"
#include "Entity.h"
#include "cs3113.h"

float playerBulletPattern(Entity* player, Bullet* bullet) {
    return 500.0f;
}

Player::Player(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType)
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType) {
    mHitbox = new Entity(position, {scale.x / 3.0f, scale.y / 3.0f}, "./assets/hitbox.png", NPC);
    mHitbox->setColliderRadius(mHitbox->getScale().x / 2.0f);
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

    if (mShifted) {
        mVelocity.x /= 2.0f;
        mVelocity.y /= 2.0f;
    }

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(enemies);
    Entity::checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(enemies);
    Entity::checkCollisionX(map);

    mHitbox->setPosition(mPosition);

    mIframes -= deltaTime;
    if (mIframes <= 0.0f) {
        mIframes = 0.0f;
    } else {
        mDirection = INVINCIBLE;
    }

    mAttackDelay += deltaTime;
    if (mAttackDelay >= mAttackSpeed) {
        // conditional based on mElapsedTime to switch up attacks
        mAttackDelay = 0.0f;
        attack();
    }

    for (auto bullet = mBullets.begin(); bullet != mBullets.end(); ++bullet) {
        (*bullet)->update(deltaTime, enemies, nullptr, nullptr, 0);
        if (!(*bullet)->isActive() && (*bullet)->getDelay() <= 0.0f) {
            // bullet goes back to inactive
            mInactiveBullets.push(*bullet);
            // erase and go to the next one: erase returns the next iterator
            bullet = mBullets.erase(bullet);
            // go back one because iterator gets incremented at the end of the for loop
            --bullet;
        }
    }

    if (mTextureType == ATLAS) animate(deltaTime);
    resetMovement();
    mShifted = false;
}

void Player::checkCollisionY(std::vector<Enemy*>& entities) {
    for (Enemy* collidableEntity : entities) {
        for (Bullet* bullet : collidableEntity->getBullets()) {
            if (mIframes > 0.0f) {
                break;
            }
            if (isColliding(bullet)) {
                --mHealth;
                mIframes = 2.0f;
                break;
            }
        }
        if (Entity::isColliding(collidableEntity)) {
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
                --mHealth;
                mIframes = 2.0f;
                break;
            }
        }
        if (Entity::isColliding(collidableEntity)) {
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

// circular collision
bool Player::isColliding(Bullet* other) const {
    if (!other->isActive()) return false;

    float xDistance = fabs(mPosition.x - other->getPosition().x);
    float yDistance = fabs(mPosition.y - other->getPosition().y);

    float distance = xDistance * xDistance + yDistance * yDistance;
    float colliderDistance = mColliderRadius + other->getColliderRadius();

    if (distance < colliderDistance * colliderDistance) return true;

    return false;
}

void Player::render() {
    Entity::render();
    mHitbox->render();
}

int Player::getHealth() const {
    return mHealth;
}

Player::~Player() {
    delete mHitbox;
}

void Player::setShifted(bool isShifted) {
    mShifted = isShifted;
}

void Player::renderBullets() {
    for (Bullet* bullet : mBullets) {
        bullet->render();
    }
}

void Player::attack() {
    float angle = 180.0f;
    Bullet* nextBullet = mInactiveBullets.front();
    nextBullet->setPosition({mPosition.x, mPosition.y - mColliderRadius});
    nextBullet->setAngle(angle);
    nextBullet->setPattern(playerBulletPattern);
    nextBullet->deactivate();
    nextBullet->setTimeAlive(2.0f);
    mBullets.push_back(nextBullet);
    mInactiveBullets.pop();
}

void Player::addBullet(Bullet* bullet) {
    mInactiveBullets.push(bullet);
}
