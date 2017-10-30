/**
 * Grid management function for game 2048.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "grid.h"

static bool fSlideLeft(Grid_t *gridPtr);
static bool fSlideRight(Grid_t *gridPtr);
static bool fSlideUp(Grid_t *gridPtr);
static bool fSlideDown(Grid_t *gridPtr);

static bool fMergeUp(Grid_t *gridPtr);
static bool fMergeDown(Grid_t *gridPtr);
static bool fMergeLeft(Grid_t *gridPtr);
static bool fMergeRight(Grid_t *gridPtr);


void Grid_Init(Grid_t *gridPtr, SDL_Renderer *rPtr)
{
   // Seed the random number generator
   srand((unsigned)time(NULL));

   Tile_SetRenderer(rPtr);
   for (int r = 0; r < GRID_SIZE; r++)
   {
      for (int c = 0; c < GRID_SIZE; c++)
      {
         int x = TILE_PAD_PX + c * (TILE_SIZE_PX + TILE_PAD_PX);
         int y = TILE_PAD_PX + r * (TILE_SIZE_PX + TILE_PAD_PX);
         Tile_Init(&(gridPtr->tiles[r][c]), r, c, x, y);
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
            emptyTiles[numEmptyTiles] = &gridPtr->tiles[r][c];
            numEmptyTiles++;
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

bool Grid_UpdateUp(Grid_t *gridPtr)
{
   bool movedPre = false;
   bool merged = false;
   bool movedPost = false;

   movedPre = fSlideUp(gridPtr);
   merged = fMergeUp(gridPtr);
   movedPost = fSlideUp(gridPtr);

   return movedPre || merged || movedPost;
}

bool Grid_UpdateDown(Grid_t *gridPtr)
{
   bool movedPre = false;
   bool merged = false;
   bool movedPost = false;

   movedPre = fSlideDown(gridPtr);
   merged = fMergeDown(gridPtr);
   movedPost = fSlideDown(gridPtr);

   return movedPre || merged || movedPost;
}

bool Grid_UpdateLeft(Grid_t *gridPtr)
{
   bool movedPre = false;
   bool merged = false;
   bool movedPost = false;

   movedPre = fSlideLeft(gridPtr);
   merged = fMergeLeft(gridPtr);
   movedPost = fSlideLeft(gridPtr);

   return movedPre || merged || movedPost;
}

bool Grid_UpdateRight(Grid_t *gridPtr)
{
   bool movedPre = false;
   bool merged = false;
   bool movedPost = false;

   movedPre = fSlideRight(gridPtr);
   merged = fMergeRight(gridPtr);
   movedPost = fSlideRight(gridPtr);

   return movedPre || merged || movedPost;
}

/**
 * Push tiles up until tiles cannot be pushed up anymore.
 * @note this function does not combine/merge tiles.
 */
static bool fSlideUp(Grid_t *gridPtr)
{
   bool atLeastOneMoved = false;
   bool moved = false;
   do
   {
      moved = false;
      for (int c = 0; c < GRID_SIZE; c++)
      {
         for (int r = 1; r < GRID_SIZE; r++)
         {
            if (gridPtr->tiles[r][c].exp != 0)
            {

               // Move tile one row up if above is clear
               if (gridPtr->tiles[r - 1][c].exp == 0)
               {
                  Tile_SetExp(&gridPtr->tiles[r - 1][c], gridPtr->tiles[r][c].exp);
                  Tile_SetExp(&gridPtr->tiles[r][c], 0);
                  moved = true;
                  atLeastOneMoved = true;
               }
            }
         }
      }
   } while (moved);
   return atLeastOneMoved;
}

/**
 * Push tiles down until tiles cannot be pushed down anymore.
 * @note this function does not combine/merge tiles.
 */
static bool fSlideDown(Grid_t *gridPtr)
{
   bool atLeastOneMoved = false;
   bool moved = false;
   do
   {
      moved = false;
      for (int c = 0; c < GRID_SIZE; c++)
      {
         for (int r = GRID_SIZE - 2; r >= 0; r--)
         {
            if (gridPtr->tiles[r][c].exp != 0)
            {
               // Move tile one row down if below is clear
               if (gridPtr->tiles[r + 1][c].exp == 0)
               {
                  Tile_SetExp(&gridPtr->tiles[r + 1][c], gridPtr->tiles[r][c].exp);
                  Tile_SetExp(&gridPtr->tiles[r][c], 0);
                  moved = true;
                  atLeastOneMoved = true;
               }
            }
         }
      }
   } while (moved);
   return atLeastOneMoved;
}

/**
 * Push tiles left until tiles cannot be pushed left anymore.
 * @note this function does not combine/merge tiles.
 */
static bool fSlideLeft(Grid_t *gridPtr)
{
   bool atLeastOneMoved = false;
   bool moved = false;
   do
   {
      moved = false;
      for (int r = 0; r < GRID_SIZE; r++)
      {
         for (int c = 1; c < GRID_SIZE; c++)
         {
            if (gridPtr->tiles[r][c].exp != 0)
            {
               // Move tile one row left if left is clear
               if (gridPtr->tiles[r][c - 1].exp == 0)
               {
                  Tile_SetExp(&gridPtr->tiles[r][c - 1], gridPtr->tiles[r][c].exp);
                  Tile_SetExp(&gridPtr->tiles[r][c], 0);
                  moved = true;
                  atLeastOneMoved = true;
               }
            }
         }
      }
   } while (moved);
   return atLeastOneMoved;
}

/**
 * Push tiles right until tiles cannot be pushed right anymore.
 * @note this function does not combine/merge tiles.
 */
static bool fSlideRight(Grid_t *gridPtr)
{
   bool atLeastOneMoved = false;
   bool moved = false;
   do
   {
      moved = false;
      for (int r = 0; r < GRID_SIZE; r++)
      {
         for (int c = GRID_SIZE - 2; c >= 0; c--)
         {
            if (gridPtr->tiles[r][c].exp != 0)
            {
               // Move tile one row right if right is clear
               if (gridPtr->tiles[r][c + 1].exp == 0)
               {
                  Tile_SetExp(&gridPtr->tiles[r][c + 1], gridPtr->tiles[r][c].exp);
                  Tile_SetExp(&gridPtr->tiles[r][c], 0);
                  moved = true;
                  atLeastOneMoved = true;
               }
            }
         }
      }
   } while (moved);
   return atLeastOneMoved;
}

/**
 * Combine/merge tiles of the same value as tiles get pushed up.
 * @note this tile doesn't push tiles.
 */
static bool fMergeUp(Grid_t *gridPtr)
{
   bool merged = false;
   for (int c = 0; c < GRID_SIZE; c++)
   {
      for (int r = 1; r < GRID_SIZE; r++)
      {
         if (gridPtr->tiles[r][c].exp != 0)
         {

            // Move tile one row up if above is clear
            if (gridPtr->tiles[r - 1][c].exp == gridPtr->tiles[r][c].exp)
            {
               Tile_SetExp(&gridPtr->tiles[r - 1][c], gridPtr->tiles[r][c].exp + 1);
               Tile_SetExp(&gridPtr->tiles[r][c], 0);
               merged = true;
            }
         }
      }
   }
   return merged;
}

static bool fMergeDown(Grid_t *gridPtr)
{
   bool merged = false;
   for (int c = 0; c < GRID_SIZE; c++)
   {
      for (int r = GRID_SIZE - 2; r >= 0; r--)
      {
         if (gridPtr->tiles[r][c].exp != 0)
         {
            // Move tile one row down if below is clear
            if (gridPtr->tiles[r + 1][c].exp == gridPtr->tiles[r][c].exp)
            {
               Tile_SetExp(&gridPtr->tiles[r + 1][c], gridPtr->tiles[r][c].exp + 1);
               Tile_SetExp(&gridPtr->tiles[r][c], 0);
               merged = true;
            }
         }
      }
   }
   return merged;
}

/**
 * Combine/merge tiles of the same value as tiles get pushed up.
 * @note this tile doesn't push tiles.
 *
 * Examples of left merge:
 * [2, 0, 2, 4] -> [4, 4, 0, 0]
 * [0, 0, 2, 2] -> [4, 0, 0, 0]
 * [2, 2, 0, 0] -> [4, 0, 0, 0]
 * [2, 2, 2, 2, 2] -> [4, 4, 2, 0, 0]
 * [8, 16, 16, 8] -> [8, 32, 8, 0]
 */
static bool fMergeLeft(Grid_t *gridPtr)
{
   bool merged = false;
   for (int r = 0; r < GRID_SIZE; r++)
   {
      for (int c = 1; c < GRID_SIZE; c++)
      {
         if (gridPtr->tiles[r][c].exp != 0)
         {
            // If value to the left is the same, double the value of that tile and
            // clear the current tile.
            if (gridPtr->tiles[r][c - 1].exp == gridPtr->tiles[r][c].exp)
            {
               Tile_SetExp(&gridPtr->tiles[r][c - 1], gridPtr->tiles[r][c].exp + 1);
               Tile_SetExp(&gridPtr->tiles[r][c], 0);
               merged = true;
            }
         }
      }
   }
   return merged;
}

static bool fMergeRight(Grid_t *gridPtr)
{
   bool merged = false;
   for (int r = 0; r < GRID_SIZE; r++)
   {
      for (int c = GRID_SIZE - 2; c >= 0; c--)
      {
         if (gridPtr->tiles[r][c].exp != 0)
         {
            // If value to the right is the same, double the value of that tile and
            // clear the current tile.
            if (gridPtr->tiles[r][c + 1].exp == gridPtr->tiles[r][c].exp)
            {
               Tile_SetExp(&gridPtr->tiles[r][c + 1], gridPtr->tiles[r][c].exp + 1);
               Tile_SetExp(&gridPtr->tiles[r][c], 0);
               merged = true;
            }
         }
      }
   }
   return merged;
}

