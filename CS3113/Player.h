#ifndef PLAYER_H
#define PLAYER_H

#include "Enemy.h"
#include "Entity.h"

class Player : public Entity {
private:
    int mHealth = 3;
    float mIframes = 0.0f;
    bool mShifted = false;
    float mAttackDelay = 0.0f;
    float mAttackSpeed = 0.2f;
    Entity* mHitbox = nullptr;
    std::list<Bullet*> mBullets;
    std::queue<Bullet*> mInactiveBullets;

public:
    Player(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
           Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
           EntityType entityType);
    ~Player() override;
    void render();
    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities,
                int collisionCheckCount) override;
    void update(float deltaTime, Entity* player, Map* map, std::vector<Enemy*>& enemies);
    void checkCollisionY(std::vector<Enemy*>& entities);
    void checkCollisionX(std::vector<Enemy*>& entities);
    void setShifted(bool isShifted);
    void renderBullets();
    void addBullet(Bullet* bullet);
    void attack();
    void clearBullets();
    bool isColliding(Bullet* other) const;
    int getHealth() const;
    void setHealth(int newHealth);
};

#endif
