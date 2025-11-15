#include "Player.h"

#include "Entity.h"

Player::Player(Vector2 position, Vector2 scale, const char* textureFile, EntityType entityType)
    : Entity(position, scale, textureFile, entityType) {
}
