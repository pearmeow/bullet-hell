#ifndef ENEMY_H
#define ENEMY_H

#include "Bullet.h"
#include "Entity.h"

class Enemy : public Entity {
private:
    std::vector<Bullet*> mBullets;

public:
    Enemy(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
          Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
          EntityType entityType);
    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                int collisionCheckCount) override;
};

#endif
