/**
 * Tile management code for game 2048.
 */
#include "tile.h"


// Could re-organize these three arrays into a single array with a
// custom struct to keep bgColor, fgColor and fontSize together.

/**
 * Background color of a tile indexed by log2(value)
 */
static const SDL_Color sTileBgColor[32] = {
   { .r = 0x99, .g = 0x99, .b = 0x99, .a = 0xff },    // 2^0 : empty tile
   { .r = 0xaa, .g = 0xaa, .b = 0xaa, .a = 0xff },    // 2^1 : 2
   { .r = 0xbb, .g = 0xbb, .b = 0xbb, .a = 0xff },    // 2^2 : 4
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^3 : 8
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^4 : 16
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^5 : 32
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^6 : 64
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^7 : 128
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^8 : 256
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^9 : 512
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^10 : 1024
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^11 : 2048
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^12 : 4096
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^13 : 8192
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^14 : 16384
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^15 : 
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^16 : 
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^17 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^18 : 
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^19 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^20 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^21 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^22 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^23 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^24 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^25 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^26 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^27 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^28 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^29 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^30 :
   { .r = 1, .g = 1, .b = 1, .a = 0xff },    // 2^31 :
};

/**
* Forground color of a tile indexed by log2(value)
*/
static const SDL_Color sTileFgColor[32] = {
   { .r = 0x00,.g = 0x00,.b = 0x00,.a = 0x00 },    // 2^0 : empty tile
   { .r = 0x10,.g = 0x10,.b = 0x10,.a = 0xff },    // 2^1 : 2
   { .r = 0x10,.g = 0x10,.b = 0x10,.a = 0xff },    // 2^2 : 4
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^3 : 8
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^4 : 16
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^5 : 32
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^6 : 64
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^7 : 128
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^8 : 256
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^9 : 512
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^10 : 1024
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^11 : 2048
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^12 : 4096
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^13 : 8192
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^14 : 16384
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^15 : 
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^16 : 
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^17 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^18 : 
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^19 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^20 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^21 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^22 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^23 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^24 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^25 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^26 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^27 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^28 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^29 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^30 :
   { .r = 1,.g = 1,.b = 1,.a = 0xff },    // 2^31 :
};

static const unsigned char sTileFontSize[32] = {
   0,    // 2^0 : empty tile
   48,   // 2^1 : 2
   48,   // 2^2 : 4
   48,   // 2^3 : 8
   48,   // 2^4 : 16
   48,   // 2^5 : 32
   48,   // 2^6 : 64
   36,   // 2^7 : 128
   16,   // 2^8 : 256
   16,   // 2^9 : 512
   16,   // 2^10 : 1024
   16,   // 2^11 : 2048
   16,   // 2^12 : 4096
   16,   // 2^13 : 8192
   16,   // 2^14 : 16384
   16,   // 2^15 : 
   16,   // 2^16 : 
   16,   // 2^17 :
   16,   // 2^18 : 
   16,   // 2^19 :
   16,   // 2^20 :
   16,   // 2^21 :
   16,   // 2^22 :
   16,   // 2^23 :
   16,   // 2^24 :
   16,   // 2^25 :
   16,   // 2^26 :
   16,   // 2^27 :
   16,   // 2^28 :
   16,   // 2^29 :
   16,   // 2^30 :
   16,   // 2^31 :
};


// we really should pre-render the tiles.
// it would suck to have to re-render a '2' tile every time we need to render it.

void Tile_GetFgBgColors(Tile_t *tPtr, SDL_Color *fgColor, SDL_Color *bgColor)
{

}
