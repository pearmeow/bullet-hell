#ifndef ENEMY_H
#define ENEMY_H

#include <list>
#include <queue>

class Bullet;

#include "Entity.h"

class Enemy : public Entity {
private:
    std::list<Bullet*> mBullets;
    std::queue<Bullet*> mInactiveBullets;
    float (*mMovePattern)(Entity* player, Enemy* enemy);
    float mAttackDelay = 0.0f;
    float mAttackSpeed = 2.0f;
    float mElapsedTime = 0.0f;
    int mHealth = 100;
    int mAttacks = 25;

public:
    Enemy(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
          Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
          EntityType entityType);
    ~Enemy();
    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                int collisionCheckCount) override;
    void render();
    void splitAttack(float initAngle, int attacks, float (*pattern)(Entity* player, Bullet* bullet));
    void delayedAttack(float initAngle, int attacks, float delay,
                       float (*pattern)(Entity* player, Bullet* bullet));
    std::list<Bullet*>& getBullets();
    void addBullet(Bullet* bullet);
    void setHealth(int newHealth);
    int getHealth() const;
    void updateBullets(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                       int collisionCheckCount);
    void clearBullets();
    void setElapsedTime(float newElapsedTime);
    float getElapsedTime() const;
    void setAttackDelay(float newAttackDelay);
    void setMovePattern(float (*pattern)(Entity* player, Enemy* enemy));
};

#endif
