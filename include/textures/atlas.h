#ifndef ATLAS_FUNCTIONS_H
#define ATLAS_FUNCTIONS_H

#include "raylib.h"
#include "except.h"
#include "table.h"
#include "textures/atlas.h"

// Full Atlas struct definition (with raylib types)
struct Atlas {
    Texture2D texture;
    Atlas_Table rects;
    int rectCount;
    int capacity;
};

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

