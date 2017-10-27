/**
 * Tile management code for game 2048.
 *
 * Tile textures are generated on the fly each time the game runs because
 * I needed practice with SDL_ttf. This also allows tile castomization.
 */
#include "tile.h"

#include <stdio.h>
#include "SDL_ttf.h"


 // Could re-organize these three arrays into a single array with a
 // custom struct to keep bgColor, fgColor and fontSize together.

 /**
  * Background color of a tile indexed by tile exponent
  */
static const SDL_Color sTileBgColor[TILE_MAX_EXP+1] = {
   {.r = 0x99,.g = 0x99,.b = 0x99,.a = 0xff },    // 2^0 : empty tile
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^1 : 2
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^2 : 4
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^3 : 8
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^4 : 16
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^5 : 32
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^6 : 64
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^7 : 128
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^8 : 256
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^9 : 512
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^10 : 1024
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^11 : 2048
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^12 : 4096
   {.r = 0xbb,.g = 0xbb,.b = 0xbb,.a = 0xff },    // 2^13 : 8192
};

/**
* Forground color of a tile indexed by tile exponent
*/
static const SDL_Color sTileFgColor[TILE_MAX_EXP+1] = {
   {.r = 0x00,.g = 0x00,.b = 0x00,.a = 0x00 },    // 2^0 : empty tile
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^1 : 2
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^2 : 4
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^3 : 8
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^4 : 16
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^5 : 32
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^6 : 64
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^7 : 128
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^8 : 256
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^9 : 512
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^10 : 1024
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^11 : 2048
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^12 : 4096
   {.r = 0xef,.g = 0xef,.b = 0x92,.a = 0xff },    // 2^13 : 8192
};

static const unsigned char sTileFontSize[TILE_MAX_EXP+1] = {
   0,    // 2^0 : empty tile
   64,   // 2^1 : 2
   64,   // 2^2 : 4
   64,   // 2^3 : 8
   56,   // 2^4 : 16
   56,   // 2^5 : 32
   56,   // 2^6 : 64
   48,   // 2^7 : 128
   48,   // 2^8 : 256
   48,   // 2^9 : 512
   36,   // 2^10 : 1024
   36,   // 2^11 : 2048
   36,   // 2^12 : 4096
   36,   // 2^13 : 8192
};

/**
 * Tile texture map. Dynamically allocated from Tile_New()
 */
static SDL_Texture *sTTMap[TILE_MAX_EXP+1] = { 0 };

/**
 * If the texture of the tile with this value has already been rendered, return the rendered texture;
 * otherwise, render the texture first, add it to the texture map and return it.
 */
static SDL_Texture *fGetTileTextureFromTextureMap(SDL_Renderer *renderer, unsigned char exp)
{
   SDL_assert(exp <= TILE_MAX_EXP);

   if (sTTMap[exp] == NULL)
   {
      // Create an empty texture rectangle
      // TODO: Should this be destroyed at the end of the game or a when a new round starts?
      sTTMap[exp] = SDL_CreateTexture(
         renderer,
         SDL_PIXELFORMAT_RGBA8888,
         SDL_TEXTUREACCESS_TARGET,  // must use this access in order to render to the texture
         TILE_SIZE_PX,
         TILE_SIZE_PX);

      // Point the renderer at the new tile texture
      SDL_SetRenderTarget(renderer, sTTMap[exp]);

      // Render background rectangle into the texture
      SDL_Rect r = {
         .w = TILE_SIZE_PX,
         .h = TILE_SIZE_PX,
         .x = 0,
         .y = 0
      };
      SDL_Color *bgColor = (SDL_Color *)&sTileBgColor[exp];


      SDL_SetRenderDrawColor(renderer, bgColor->r, bgColor->g, bgColor->b, bgColor->a);
      SDL_RenderFillRect(renderer, &r);

      // Render text for non-empty tiles only
      if (exp != 0)
      {
         TTF_Font *fnt = TTF_OpenFont(TILE_FONT_FILE_NAME, sTileFontSize[exp]);
         int txtW = 0;
         int txtH = 0;
         SDL_Rect txtRect;
         TTF_SetFontHinting(fnt, TTF_HINTING_MONO);
         TTF_SetFontKerning(fnt, 1);
         // TODO: Check that the font file actually loaded and do something smart
         char str[17] = { 0 };
         snprintf(str, 16, "%u", 1 << exp);

         // If text doesn't fit into tile, replace it with "."
         TTF_SizeText(fnt, str, &txtW, &txtH);
         if (txtW >= TILE_SIZE_PX || txtH >= TILE_SIZE_PX)
         {
            str[0] = 'X';
            str[1] = '\0';
            TTF_SizeText(fnt, str, &txtW, &txtH);
         }

         SDL_Surface *surf = TTF_RenderText_Blended(fnt, str, sTileFgColor[exp]);
         SDL_Texture *txtr = SDL_CreateTextureFromSurface(renderer, surf);

         // Center the text texture in the tile background texture
         txtRect.x = (r.w - txtW) / 2;
         txtRect.y = (r.h - txtH) / 2;
         txtRect.w = txtW;
         txtRect.h = txtH;

         SDL_RenderCopy(renderer, txtr, NULL, &txtRect);
         SDL_FreeSurface(surf);
         SDL_DestroyTexture(txtr);
         TTF_CloseFont(fnt);
      }

      // Set the renderer back to its default target
      SDL_SetRenderTarget(renderer, NULL);
   }
   return sTTMap[exp];
}

void Tile_Init(Tile_t *tilePtr, unsigned char gridRow, unsigned char gridCol, int x, int y, SDL_Renderer *rPtr)
{
   tilePtr->gridRow = gridRow;
   tilePtr->gridCol = gridCol;
   tilePtr->x = x;
   tilePtr->y = y;
   tilePtr->exp = 0;
   tilePtr->texture = fGetTileTextureFromTextureMap(rPtr, tilePtr->exp);
}
