# gramarye-component-functions

ECS component functions library for the Gramarye game engine. Provides convenient helper functions for working with components in the ECS system.

## Overview

gramarye-component-functions provides ECS helper functions for common component types. This library separates component function implementations from component struct definitions (which are in gramarye-components). This separation allows systems to access component structs without depending on implementation details or backend-specific types (like raylib types).

## Architecture

### Separation of Concerns

- **gramarye-components**: Contains component struct definitions (Position, Health, Sprite, etc.)
- **gramarye-component-functions**: Contains ECS functions for manipulating these components (Position_add, Health_get, etc.)

This separation is important because:
- Component structs may reference backend-specific types (e.g., raylib's `Texture2D` in Atlas)
- Systems that only need struct definitions can include gramarye-components without pulling in function implementations
- Component functions that need full API access (like Atlas with raylib types) are in this library

### Include Order

When using component functions, the include path order is critical:

1. Include gramarye-component-functions headers first (they include struct definitions)
2. Component-functions headers include the struct definitions inline to ensure proper type visibility
3. For components with backend types (like Atlas), the full API with backend types is provided here

Example:
```c
// Component-functions headers come first
#include "core/position.h"  // Position_get, Position_add (includes Position struct)
#include "textures/atlas.h"   // Atlas_getRect (full API with raylib types)
```

## Component Categories

### Core Components (`core/`)

Core game components that don't depend on rendering backends:

- **Position** (`core/position.h`): 2D position component
  - `Position_add(ECS*, EntityId, ComponentTypeId, int x, int y)`
  - `Position_get(ECS*, EntityId, ComponentTypeId) -> Position*`
  - `Position_set(ECS*, EntityId, ComponentTypeId, int x, int y)`
  - `Position_remove(ECS*, EntityId, ComponentTypeId)`

- **Health** (`core/health.h`): Health/bar value component
  - `Health_add(ECS*, EntityId, ComponentTypeId, float maxHealth)`
  - `Health_get(ECS*, EntityId, ComponentTypeId) -> BarValue*`
  - `Health_remove(ECS*, EntityId, ComponentTypeId)`

- **BarValue** (`core/bar_value.h`): Generic bar value struct (used by Health)

### Texture Components (`textures/`)

Texture and rendering-related components that depend on rendering backends:

- **Atlas** (`textures/atlas.h`): Texture atlas with full raylib API
  - `Atlas_new(int initialCapacity) -> Atlas*`
  - `Atlas_setTexture(Atlas*, char* path)`
  - `Atlas_addRect(Atlas*, int index, Rectangle rect)`
  - `Atlas_getRect(const Atlas*, int index) -> Rectangle`
  - `Atlas_removeRect(Atlas*, int index)`
  - `Atlas_clear(Atlas*)`
  - `Atlas_getRectCount(const Atlas*) -> int`
  - `Atlas_free(Atlas*)`
  
  Note: This provides the full Atlas API with raylib types (`Texture2D`, `Rectangle`). The struct definition in gramarye-components uses forward declarations.

- **Sprite** (`textures/sprite.h`): Sprite component referencing an atlas
  - `Sprite_add(ECS*, EntityId, ComponentTypeId, Atlas*, int tile_id)`
  - `Sprite_get(ECS*, EntityId, ComponentTypeId) -> Sprite*`
  - `Sprite_set_tile(ECS*, EntityId, ComponentTypeId, int tile_id)`
  - `Sprite_remove(ECS*, EntityId, ComponentTypeId)`

## Usage

### Basic Example

```c
#include "gramarye_ecs/ecs.h"
#include "core/position.h"  // Position functions and struct
#include "core/health.h"    // Health functions

// Register component types
ComponentTypeId positionType = ECS_register_component_type(ecs, "Position", sizeof(Position));
ComponentTypeId healthType = ECS_register_component_type(ecs, "Health", sizeof(BarValue));

// Create entity
EntityId player = Entity_create(entityRegistry);

// Add components using helper functions
Position_add(ecs, player, positionType, 100, 200);
Health_add(ecs, player, healthType, 100.0f);

// Get components
Position* pos = Position_get(ecs, player, positionType);
BarValue* health = Health_get(ecs, player, healthType);

// Update components
Position_set(ecs, player, positionType, 150, 250);
```

### Atlas Usage

```c
#include "textures/atlas.h"  // Full Atlas API with raylib types

// Create atlas
Atlas* atlas = Atlas_new(400);
Atlas_setTexture(atlas, "resources/spritesheet.png");

// Add rectangles
Rectangle rect = {0.0f, 0.0f, 16.0f, 16.0f};
Atlas_addRect(atlas, 0, rect);

// Get rectangle (returns raylib Rectangle)
Rectangle sourceRect = Atlas_getRect(atlas, 0);
```

## Dependencies

- **gramarye-libcore**: For Table, Arena, memory management
- **gramarye-ecs**: For ECS, EntityId, ComponentTypeId
- **gramarye-components**: For component struct definitions
- **raylib**: For Atlas and Sprite components (provided by parent project)

## Building

This library is typically built as part of the main game project via CMake FetchContent or as a subdirectory. See the main project's CMakeLists.txt for build configuration.

## Integration

This library is designed to be used as a submodule in game projects. It provides a clean API for working with ECS components while keeping backend-specific dependencies isolated.

## Folder Structure

The library is organized by component category:

```
gramarye-component-functions/
├── include/
│   ├── core/          # Core components (no backend dependencies)
│   │   ├── position.h
│   │   └── health.h
│   └── textures/      # Texture/rendering components
│       ├── atlas.h
│       └── sprite.h
└── src/
    ├── core/
    │   ├── position.c
    │   ├── health.c
    │   └── bar_value.c
    └── textures/
        ├── atlas.c
        └── sprite.c
```

Components can be selectively built via CMake options. See CMakeLists.txt for details on building specific component folders.

