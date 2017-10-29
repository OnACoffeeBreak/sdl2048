/**
 * Grid management function for game 2048.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "grid.h"


void Grid_Init(Grid_t *gridPtr, SDL_Renderer *rPtr)
{
   // Seed the random number generator
   srand((unsigned)time(NULL));

   for (int r = 0; r < GRID_SIZE; r++)
   {
      for (int c = 0; c < GRID_SIZE; c++)
      {
         int x = TILE_PAD_PX + c * (TILE_SIZE_PX + TILE_PAD_PX);
         int y = TILE_PAD_PX + r * (TILE_SIZE_PX + TILE_PAD_PX);
         Tile_Init(&(gridPtr->tiles[r][c]), r, c, x, y, rPtr);
      }
   }
}

Tile_t *Grid_GetRandomEmptyTile(Grid_t *gridPtr)
{
   Tile_t *emptyTiles[GRID_SIZE*GRID_SIZE];
   int numEmptyTiles = 0;

   for (int r = 0; r < GRID_SIZE; r++)
   {
      for (int c = 0; c < GRID_SIZE; c++)
      {
         if (gridPtr->tiles[r][c].exp == 0)
         {
            emptyTiles[numEmptyTiles++] = &gridPtr->tiles[r][c];
         }
      }
   }

   if (numEmptyTiles > 0)
   {
      return emptyTiles[rand() % numEmptyTiles];
   }
   else
   {
      return NULL;
   }
}

void Grid_Render(Grid_t *gridPtr, SDL_Renderer *rPtr)
{
   SDL_Rect rect = {
      .w = TILE_PAD_PX + GRID_SIZE * (TILE_SIZE_PX + TILE_PAD_PX),
      .h = TILE_PAD_PX + GRID_SIZE * (TILE_SIZE_PX + TILE_PAD_PX),
      .x = 0,
      .y = 0
   };

   SDL_SetRenderDrawColor(rPtr, 0xaa, 0xaa, 0xaa, 0xff);
   SDL_RenderFillRect(rPtr, &rect);

   rect.w = TILE_SIZE_PX;
   rect.h = TILE_SIZE_PX;
   for (int r = 0; r < GRID_SIZE; r++)
   {
      for (int c = 0; c < GRID_SIZE; c++)
      {
         rect.x = gridPtr->tiles[r][c].x;
         rect.y = gridPtr->tiles[r][c].y;
         SDL_RenderCopy(rPtr, gridPtr->tiles[r][c].texture, NULL, &rect);
      }
   }
}