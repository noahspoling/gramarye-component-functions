#ifndef SPRITE_FUNCTIONS_H
#define SPRITE_FUNCTIONS_H

#include "gramarye_ecs/ecs.h"
#include "gramarye_ecs/entity.h"
#include "gramarye_ecs/component.h"
#include "textures/sprite.h"  // Sprite struct from gramarye-components
#include "textures/atlas.h"  // Full Atlas definition (this file)

// Add sprite component to entity
void Sprite_add(ECS* ecs, EntityId entity, ComponentTypeId typeId, Atlas* atlas, int tile_id);

// Get sprite component for entity
Sprite* Sprite_get(ECS* ecs, EntityId entity, ComponentTypeId typeId);

// Set sprite tile_id
void Sprite_set_tile(ECS* ecs, EntityId entity, ComponentTypeId typeId, int tile_id);

// Remove sprite component from entity
void Sprite_remove(ECS* ecs, EntityId entity, ComponentTypeId typeId);

#endif // SPRITE_FUNCTIONS_H

