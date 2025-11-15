#include "Player.h"

#include "Entity.h"
#include "cs3113.h"

Player::Player(Vector2 position, Vector2 scale, const char* textureFile, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType)
    : Entity(position, scale, textureFile, textureType, spriteSheetDimensions, animationAtlas, entityType) {
}
