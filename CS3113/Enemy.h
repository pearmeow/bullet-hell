#ifndef ENEMY_H
#define ENEMY_H

#include "Bullet.h"
#include "Entity.h"

class Enemy : public Entity {
private:
    std::vector<Bullet*> mBullets;

public:
    Enemy();
};

#endif
