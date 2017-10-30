#include <stdbool.h>

#include "SDL.h"
#include "SDL_ttf.h"

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
   bool tilesMoved = false;
   SDL_Event e;

   SDL_Init(SDL_INIT_VIDEO);
   SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");   // This is necessary to make tile numbers look smoother

   gWinPtr = SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WW, WH, SDL_WINDOW_SHOWN);
   gRendPtr = SDL_CreateRenderer(gWinPtr, -1, SDL_RENDERER_ACCELERATED);

   TTF_Init();

   Grid_Init(&g, gRendPtr);

   // Add two starting tiles
   for (int i = 0; i < 2; i++)
   {
      if ((t = Grid_GetRandomEmptyTile(&g)) != NULL)
      {
         Tile_SetExp(t, 1);
      }
   }

   while (!quit)
   {
      if (tilesMoved)
      {
         tilesMoved = false;
         if ((t = Grid_GetRandomEmptyTile(&g)) != NULL)
         {
            Tile_SetExp(t, 1);
         }
      }

      // Handle events on the que
      while (SDL_PollEvent(&e) != 0)
      {
         if (e.type == SDL_QUIT)
         {
            quit = true;
         }
         else if (e.type == SDL_KEYDOWN)
         {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
            {
               tilesMoved = Grid_UpdateUp(&g);
            }
            break;
            case SDLK_DOWN:
            {
               tilesMoved = Grid_UpdateDown(&g);
            }
            break;
            case SDLK_LEFT:
            {
               tilesMoved = Grid_UpdateLeft(&g);
            }
            break;
            case SDLK_RIGHT:
            {
               tilesMoved = Grid_UpdateRight(&g);
            }
            break;
            }
         }
      }

      // Clear screen on every loop
      SDL_SetRenderDrawColor(gRendPtr, 0xff, 0xff, 0xff, 0xff);
      SDL_RenderClear(gRendPtr);

      Grid_Render(&g, gRendPtr);
      SDL_RenderPresent(gRendPtr);
   }

   TTF_Quit();
   SDL_Quit();

   return 0;
}