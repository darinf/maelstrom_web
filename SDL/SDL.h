#ifndef SDL_h_
#define SDL_h_

/* placeholder */

#include "SDL_rwops.h"
#include "SDL_timer.h"
#include "SDL_types.h"

/* values */

enum {
  SDL_INIT_VIDEO = 1 << 0,
  SDL_INIT_AUDIO = 1 << 1,
  SDL_INIT_JOYSTICK = 1 << 2,
};

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

enum {
  SDL_QUIT,
  SDL_KEYDOWN,
  SDL_KEYUP,
  SDL_MOUSEBUTTONDOWN,
};

enum {
  SDLK_a,
  SDLK_c,
  SDLK_p,
  SDLK_l,
  SDLK_q,
  SDLK_0,
  SDLK_1,
  SDLK_2,
  SDLK_3,
  SDLK_4,
  SDLK_5,
  SDLK_6,
  SDLK_7,
  SDLK_8,
  SDLK_9,
  SDLK_x,
  SDLK_z,
  SDLK_F1,
  SDLK_F3,
  SDLK_LSHIFT,
  SDLK_RSHIFT,
  SDLK_LCTRL,
  SDLK_RCTRL,
  SDLK_LALT,
  SDLK_RALT,
  SDLK_RETURN,
  SDLK_TAB,
  SDLK_DELETE,
  SDLK_BACKSPACE,
  SDLK_PAUSE,
  SDLK_SPACE,
  SDLK_UP,
  SDLK_RIGHT,
  SDLK_LEFT,
  SDLK_ESCAPE,
};

enum {
  KMOD_ALT = (1 << 0),
};

enum {
  SDL_PRESSED = 1,
};

/* functions */

int SDL_Init(int init_flags);
void SDL_Quit();

int SDL_PollEvent(SDL_Event*);
int SDL_WaitEvent(SDL_Event*);
const char* SDL_GetKeyName(SDLKey);

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

int SDL_WM_ToggleFullScreen(SDL_Surface*);
void SDL_WM_SetCaption(const char* title, const char* icon);
void SDL_WM_SetIcon(SDL_Surface*, const Uint8* mask);

int SDL_ShowCursor(int);
void SDL_EnableUNICODE(int);

#endif  /* SDL_h_ */
