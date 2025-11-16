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
    float mNextAttack = 0.0f;

public:
    Enemy(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
          Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
          EntityType entityType);
    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                int collisionCheckCount) override;
    void render();
    std::list<Bullet*>& getBullets();
};

#endif
