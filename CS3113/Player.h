#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
public:
    Player(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
           Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
           EntityType entityType);
};

#endif
