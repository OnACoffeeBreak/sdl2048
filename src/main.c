#include <stdbool.h>

#include "SDL.h"

#include "grid.h"
#include "tile.h"

//========================================================
// File-local defines
//========================================================
#define WW (640)     ///< Window width
#define WH (480)     ///< Window height


//========================================================
// Globals
//========================================================
SDL_Window *gWinPtr = NULL;
SDL_Renderer *gRendPtr = NULL;

int main(int argc, char * argv[])
{

   Grid_t g;
   Tile_t *t = NULL;
   bool quit = false;
   SDL_Event e;

   SDL_Init(SDL_INIT_VIDEO);
   gWinPtr = SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WW, WH, SDL_WINDOW_SHOWN);
   gRendPtr = SDL_CreateRenderer(gWinPtr, -1, SDL_RENDERER_ACCELERATED);

   Grid_New(&g);
   t = Grid_GetRandomEmptyTile(&g);
   t->value = 2;
   t = Grid_GetRandomEmptyTile(&g);
   t->value = 2;


   while (!quit)
   {
      // Handle events on the que
      while (SDL_PollEvent(&e) != 0)
      {
         if (e.type == SDL_QUIT)
         {
            quit = true;
         }
      }

      // Clear screen on every loop
      SDL_SetRenderDrawColor(gRendPtr, 0xff, 0xff, 0xff, 0xff);
      SDL_RenderClear(gRendPtr);

      Grid_Render(&g, gRendPtr);

      SDL_RenderPresent(gRendPtr);
   }

   SDL_Quit();

   return 0;
}