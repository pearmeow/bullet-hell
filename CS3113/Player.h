#ifndef PLAYER_H
#define PLAYER_H

#include "Enemy.h"
#include "Entity.h"

class Player : public Entity {
private:
    float mIframes = 0.0f;

public:
    Player(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
           Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
           EntityType entityType);
    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                int collisionCheckCount) override;
    void update(float deltaTime, Entity* player, Map* map, std::vector<Enemy*>& enemies);
    void checkCollisionY(std::vector<Enemy*>& entities);
    void checkCollisionX(std::vector<Enemy*>& entities);
};

#endif
