#ifndef ENEMY_H
#define ENEMY_H

#include <list>
#include <queue>

#include "Bullet.h"
#include "Entity.h"

class Enemy : public Entity {
private:
    std::list<Bullet*> mBullets;
    std::queue<Bullet*> mInactiveBullets;
    float mAttackDelay = 0.0f;
    float mAttackSpeed = 1.0f;
    int mAttacks = 1;

public:
    Enemy(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
          Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
          EntityType entityType);
    ~Enemy();
    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                int collisionCheckCount) override;
    void render();
    std::list<Bullet*>& getBullets();
    void addBullet(Bullet* bullet);
};

#endif
