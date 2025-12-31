#ifndef POSITION_FUNCTIONS_H
#define POSITION_FUNCTIONS_H

#include "gramarye_ecs/ecs.h"
#include "gramarye_ecs/entity.h"
#include "gramarye_ecs/component.h"
#include "core/position.h"

// Add position component to entity
void Position_add(ECS* ecs, EntityId entity, ComponentTypeId typeId, int x, int y);

// Get position component for entity
Position* Position_get(ECS* ecs, EntityId entity, ComponentTypeId typeId);

// Set position component for entity
void Position_set(ECS* ecs, EntityId entity, ComponentTypeId typeId, int x, int y);

// Remove position component from entity
void Position_remove(ECS* ecs, EntityId entity, ComponentTypeId typeId);

#endif // POSITION_FUNCTIONS_H

