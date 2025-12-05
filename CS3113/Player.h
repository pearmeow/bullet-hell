#ifndef PLAYER_H
#define PLAYER_H

#include "Enemy.h"
#include "Entity.h"

class Player : public Entity {
private:
    int mHealth = 3;
    int mBombs = 2;
    bool mIsBombing = false;
    float mIframes = 0.0f;
    bool mShifted = false;
    float mAttackDelay = 0.0f;
    float mAttackSpeed = 0.2f;
    Entity* mHitbox = nullptr;
    std::list<Bullet*> mBullets;
    std::queue<Bullet*> mInactiveBullets;
    bool pause = false;

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
    bool bomb();
    void setBombs(int bombs);
    bool isBombing() const;
    void setBombing(bool isBombing);
    bool isColliding(Bullet* other) const;
    bool isInvincible() const;
    void updateBullets(float deltaTime, std::vector<Enemy*> enemies, Map* map, Entity* collidableEntities,
                       int collisionCheckCount);
    int getHealth() const;
    void setHealth(int newHealth);
    void togglePause();
    bool isPaused();
};

#endif
