#include "gramarye_ecs/ecs.h"
#include "gramarye_ecs/entity.h"
#include "gramarye_ecs/component.h"
#include "textures/atlas.h"  // Need Atlas definition first

// Get Sprite struct definition from gramarye-components
// Since component-functions/include comes first, we need to define it inline
// or ensure we get it from gramarye-components
#ifndef SPRITE_H
#define SPRITE_H
typedef struct {
    Atlas* atlas;
    int tile_id;
} Sprite;
#endif

// Now include the function declarations
#include "textures/sprite.h"

void Sprite_add(ECS* ecs, EntityId entity, ComponentTypeId typeId, Atlas* atlas, int tile_id) {
    Sprite sprite;
    sprite.atlas = atlas;
    sprite.tile_id = tile_id;
    ECS_add_component(ecs, entity, typeId, &sprite);
}

Sprite* Sprite_get(ECS* ecs, EntityId entity, ComponentTypeId typeId) {
    return (Sprite*)ECS_get_component(ecs, entity, typeId);
}

void Sprite_set_tile(ECS* ecs, EntityId entity, ComponentTypeId typeId, int tile_id) {
    Sprite* sprite = Sprite_get(ecs, entity, typeId);
    if (sprite) {
        sprite->tile_id = tile_id;
    }
}

void Sprite_remove(ECS* ecs, EntityId entity, ComponentTypeId typeId) {
    ECS_remove_component(ecs, entity, typeId);
}

