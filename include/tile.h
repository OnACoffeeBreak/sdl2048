#pragma once

#include "SDL.h"

#define TILE_SIZE_PX (86)     ///< Tile size (width and height) in pixels

typedef struct {
   unsigned char gridRow;  ///< Row in which the tile is in the grid. Starts at 0.
   unsigned char gridCol;  ///< Column in which the tile is in the grid. Starts at 0.
   unsigned int value;     ///< Tile value: 0 to 2^31 by steps that are power of 2. 0 indicates an empty tile
   int x;                  ///< x coord of the top left corner of the tile on the screen
   int y;                  ///< y coord of the top left corner of the tile on the screen
} Tile_t;

/**
 * Get tile colors based on tile's value.
 */
void Tile_GetFgBgColors(Tile_t *tPtr, SDL_Color *fgColor, SDL_Color *bgColor);