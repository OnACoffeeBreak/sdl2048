/**
 * Grid management function for game 2048.
 */
#include <stdio.h>
#include <stdlib.h>

#include "grid.h"


void Grid_New(Grid_t *gridPtr)
{
   // Seed the random number generator
   srand((unsigned)time(NULL));

   for (int r = 0; r < GRID_SIZE; r++)
   {
      for (int c = 0; c < GRID_SIZE; c++)
      {
         gridPtr->tiles[r][c].gridRow = r;
         gridPtr->tiles[r][c].gridCol = c;
         gridPtr->tiles[r][c].value = 0;
         gridPtr->tiles[r][c].x = TILE_PAD_PX + c * (TILE_SIZE_PX + TILE_PAD_PX);
         gridPtr->tiles[r][c].y = TILE_PAD_PX + r * (TILE_SIZE_PX + TILE_PAD_PX);
      }
   }
}

Tile_t *Grid_GetRandomEmptyTile(Grid_t *gridPtr)
{
   int r = 0;
   int c = 0;

   /// "TODO: This will get expensive as the grid fills up! Fix it!"
   do
   {
      r = rand() % GRID_SIZE;
      c = rand() % GRID_SIZE;
   } while (gridPtr->tiles[r][c].value > 0);

   return &(gridPtr->tiles[r][c]);
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
   SDL_SetRenderDrawColor(rPtr, 0xcc, 0xcc, 0xcc, 0xff);

   for (int r = 0; r < GRID_SIZE; r++)
   {
      for (int c = 0; c < GRID_SIZE; c++)
      {
         rect.x = gridPtr->tiles[r][c].x;
         rect.y = gridPtr->tiles[r][c].y;
         SDL_RenderFillRect(rPtr, &rect);
      }
   }
}