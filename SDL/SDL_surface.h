#ifndef SDL_surface_h_
#define SDL_surface_h_

#include "SDL_types.h"

enum {
  SDL_SWSURFACE = 1 << 0,
  SDL_HWSURFACE = 1 << 1,
  SDL_HWACCEL = 1 << 2,
  SDL_FULLSCREEN = 1 << 3,
};

enum {
  SDL_SRCCOLORKEY = 1 << 0,
  SDL_RLEACCEL = 1 << 1,
};

struct SDL_Color {
  Uint8 r, g, b, a;
};

struct SDL_Palette {
  SDL_Color* colors;
  int ncolors;
};

struct SDL_PixelFormat {
  SDL_Palette* palette;  
  int Rshift;
  int Gshift;
  int Bshift;
  int Rmask;
  int Gmask;
  int Bmask;
  int BitsPerPixel;
  int BytesPerPixel;
};

struct SDL_Surface {
  int refcount;
  SDL_PixelFormat* format;
  Uint32 w, h;
  Uint32 pitch;
  void* pixels;
  int flags;
};

SDL_Surface* SDL_SetVideoMode(int width, int height, int depth, int video_flags);
SDL_Surface* SDL_GetVideoSurface();
SDL_Surface* SDL_CreateRGBSurface(int video_flags, int width, int height, int depth, int a, int b, int c, int d);
void SDL_FreeSurface(SDL_Surface*);
void SDL_BlitSurface(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dest, SDL_Rect* dst_rect);
void SDL_LowerBlit(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dest, SDL_Rect* dst_rect);
int SDL_LockSurface(SDL_Surface*);
void SDL_UnlockSurface(SDL_Surface*);
void SDL_UpdateRects(SDL_Surface*, int num_rects, SDL_Rect* rects);

SDL_Surface* SDL_LoadBMP(const char* path);
int SDL_SaveBMP(SDL_Surface*, const char* path);

void SDL_SetGammaRamp(Uint16*, Uint16*, Uint16*);

void SDL_SetColorKey(SDL_Surface*, int flags, Uint8);
void SDL_SetColors(SDL_Surface*, SDL_Color* colors, int offset, int count);
Uint32 SDL_MapRGB(SDL_PixelFormat*, Uint8 r, Uint8 g, Uint8 b);

#endif  /* SDL_surface_h_ */
