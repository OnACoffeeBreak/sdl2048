#pragma once

#include "SDL.h"

#define TILE_SIZE_PX          (86)              ///< Tile size (width and height) in pixels
#define TILE_FONT_FILE_NAME   ("assets\\impact.ttf")  ///< Name of the font file to use for tile numbers
#define TILE_MAX_EXP          (13)              ///< Maximum value Tile_t::exp can have.

typedef struct {
   unsigned char exp;      ///< Tile exponent: the actual tile value is 2^exp.
                           ///< 0 denotes an empty tile. Max value is TILE_MAX_EXP.
                           ///< exp = 1, value = 2^1 = 2
                           ///< exp = 2, value = 2^2 = 4
                           ///< exp = 3, value = 2^3 = 8
                           ///< ...
                           ///< exp = 13, value = 2^13 = 8192
   SDL_Texture *texture;   ///< Tile texture
   unsigned char gridRow;  ///< Row in which the tile is in the grid. Starts at 0.
   unsigned char gridCol;  ///< Column in which the tile is in the grid. Starts at 0.
   int x;                  ///< x coord of the top left corner of the tile on the screen
   int y;                  ///< y coord of the top left corner of the tile on the screen
} Tile_t;

/**
 * Intialize a new tile
 */
void Tile_Init(Tile_t *tilePtr, unsigned char gridRow, unsigned char gridCol, int x, int y, SDL_Renderer *rPtr);

/**
 * Set tile exponent
 * @TODO: Passing in SDL_Renderer doesn't fit here. What to do?
 */
void Tile_SetExp(Tile_t *tilePtr, unsigned char exp, SDL_Renderer *rPtr);
