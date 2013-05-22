#ifndef SDL_h_
#define SDL_h_

/* placeholder */

#include "SDL_types.h"

/* values */

enum {
  SDL_INIT_VIDEO = 1 << 0,
  SDL_INIT_AUDIO = 1 << 1,
  SDL_INIT_JOYSTICK = 1 << 2,
};

enum {
  SDL_SWSURFACE = 1 << 0,
  SDL_FULLSCREEN = 1 << 1,
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

SDL_Surface* SDL_GetVideoSurface();
SDL_Surface* SDL_CreateRGBSurface(int video_flags, int width, int height, int depth, int a, int b, int c, int d);
void SDL_FreeSurface(SDL_Surface*);
void SDL_BlitSurface(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dest, SDL_Rect* dst_rect);
SDL_Surface* SDL_LoadBMP(const char* path);

const char* SDL_GetError();
void SDL_SetError(const char*);

int SDL_WM_ToggleFullScreen(SDL_Surface*);
void SDL_WM_SetCaption(const char* title, const char* icon);

void SDL_ShowCursor(bool);
void SDL_EnableUNICODE(bool);

void SDL_Delay(Uint32);
Uint32 SDL_GetTicks();

SDL_RWops* SDL_RWFromFile(const char* path, const char* mode);
void SDL_RWclose(SDL_RWops*);
Uint16 SDL_ReadBE16(SDL_RWops*);
Uint32 SDL_ReadBE32(SDL_RWops*);
Uint32 SDL_RWread(SDL_RWops*, void* buf, Uint32 size, Uint32 count);
Uint32 SDL_RWwrite(SDL_RWops*, const void* buf, Uint32 size, Uint32 count);
Uint32 SDL_WriteBE32(SDL_RWops*, Uint32);

#endif  /* SDL_h_ */
