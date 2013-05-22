#include "SDL_surface.h"

SDL_Surface* SDL_SetVideoMode(int width, int height, int depth, int video_flags) { return 0; }
SDL_Surface* SDL_GetVideoSurface() { return 0; }
SDL_Surface* SDL_CreateRGBSurface(int video_flags, int width, int height, int depth, int a, int b, int c, int d) { return 0; }
void SDL_FreeSurface(SDL_Surface*) {}
void SDL_BlitSurface(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dest, SDL_Rect* dst_rect) {}
void SDL_LowerBlit(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dest, SDL_Rect* dst_rect) {}
int SDL_LockSurface(SDL_Surface*) { return 0; }
void SDL_UnlockSurface(SDL_Surface*) {}
void SDL_UpdateRects(SDL_Surface*, int num_rects, SDL_Rect* rects) {}

SDL_Surface* SDL_LoadBMP(const char* path) { return 0; }
int SDL_SaveBMP(SDL_Surface*, const char* path) { return 0; }

void SDL_SetGammaRamp(Uint16*, Uint16*, Uint16*) {}

void SDL_SetColorKey(SDL_Surface*, int flags, Uint8) {}
void SDL_SetColors(SDL_Surface*, SDL_Color* colors, int offset, int count) {}
Uint32 SDL_MapRGB(SDL_PixelFormat*, Uint8 r, Uint8 g, Uint8 b) { return 0; }
