#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
    Player(Vector2 position, Vector2 scale, const char* textureFile, EntityType entityType);
};

#endif
