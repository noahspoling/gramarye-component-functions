#ifndef ATLAS_FUNCTIONS_H
#define ATLAS_FUNCTIONS_H

#include "raylib.h"
#include "except.h"
#include "table.h"

// Get constants and typedefs from gramarye-components
// Since component-functions/include comes first, "textures/atlas.h" finds this header


// Constants from gramarye-components (should match)
#ifndef ATLAS_MAX_CAPACITY
#define ATLAS_MAX_CAPACITY 500
#endif
#ifndef ATLAS_INITIAL_CAPACITY
#define ATLAS_INITIAL_CAPACITY 400
#endif

typedef Table_T Atlas_Table;

// Full Atlas struct definition (with raylib types)
struct Atlas {
    Texture2D texture;
    Atlas_Table rects;
    int rectCount;
    int capacity;
};

// Typedef for Atlas
typedef struct Atlas Atlas;

// Function implementations
Atlas* Atlas_new(int initialCapacity);
void Atlas_addRect(Atlas* atlas, int index, Rectangle rect);
void Atlas_setTexture(Atlas* atlas, char *path);
Rectangle Atlas_getRect(const Atlas* atlas, int index);
void Atlas_removeRect(Atlas* atlas, int index);
void Atlas_clear(Atlas* atlas);
int Atlas_getRectCount(const Atlas* atlas);
void Atlas_free(Atlas* atlas);

#endif // ATLAS_FUNCTIONS_H

