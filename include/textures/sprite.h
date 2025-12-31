#ifndef SPRITE_FUNCTIONS_H
#define SPRITE_FUNCTIONS_H

#include "gramarye_ecs/ecs.h"
#include "gramarye_ecs/entity.h"
#include "gramarye_ecs/component.h"
#include "textures/atlas.h"  // Full Atlas definition

// Include Sprite struct definition from gramarye-components
#ifndef SPRITE_H
#define SPRITE_H
// Forward declare Atlas (already included above with full definition)
typedef struct Atlas Atlas;
typedef struct {
    Atlas* atlas;
    int tile_id;
} Sprite;
#endif

// Add sprite component to entity
void Sprite_add(ECS* ecs, EntityId entity, ComponentTypeId typeId, Atlas* atlas, int tile_id);

// Get sprite component for entity
Sprite* Sprite_get(ECS* ecs, EntityId entity, ComponentTypeId typeId);

// Set sprite tile_id
void Sprite_set_tile(ECS* ecs, EntityId entity, ComponentTypeId typeId, int tile_id);

// Remove sprite component from entity
void Sprite_remove(ECS* ecs, EntityId entity, ComponentTypeId typeId);

#endif // SPRITE_FUNCTIONS_H

