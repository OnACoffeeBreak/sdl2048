#pragma once
#include "SDL.h"
#include "tile.h"

#define GRID_SIZE    (4)   ///< Grid width and height
#define TILE_PAD_PX  (16)   ///< Number of pixels to pad between tiles

typedef struct {
   Tile_t tiles[GRID_SIZE][GRID_SIZE];          ///< Tiles that make up the grid
} Grid_t;

/**
 * Initialize the grid to an empty grid.
 * 
 * @param [in] gridPtr     Pointer to an allocated Grid_t to initialize
 * @param [in[ rPtr        Pointer to the renderer to use to render tile textures
 */
void Grid_Init(Grid_t *gridPtr, SDL_Renderer *rPtr);

/**
 * Out of all empty tiles in the grid, return a pointer to a random one.
 * 
 * @param [in] gridPtr     Pointer to the grid.
 *
 * @retrun A random empty tile from the grid. NULL if all tiles are occupied.
 */
Tile_t *Grid_GetRandomEmptyTile(Grid_t *gridPtr);

/**
 * Render the grid using the supplied SDL renderer.
 */
void Grid_Render(Grid_t *gridPtr, SDL_Renderer *rPtr);

/**
 * Move grid tiles up.
 */
bool Grid_UpdateUp(Grid_t *gridPtr);

/**
* Move grid tiles down.
*/
bool Grid_UpdateDown(Grid_t *gridPtr);

/**
* Move grid tiles left.
*/
bool Grid_UpdateLeft(Grid_t *gridPtr);

/**
* Move grid tiles right.
*/
bool Grid_UpdateRight(Grid_t *gridPtr);