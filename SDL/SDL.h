#ifndef SDL_h_
#define SDL_h_

/* placeholder */

#include "SDL_types.h"

/* functions */

int SDL_PollEvent(SDL_Event*);
int SDL_WaitEvent(SDL_Event*);

SDL_Surface* SDL_GetVideoSurface();
int SDL_WM_ToggleFullScreen(SDL_Surface*);
void SDL_WM_SetCaption(const char* title, const char* icon);

void SDL_ShowCursor(int);

void SDL_Delay(Uint32);
Uint32 SDL_GetTicks();

/* values */

enum {
  SDL_SWSURFACE = 1 << 0,
  SDL_FULLSCREEN = 1 << 1,
};

enum {
  SDL_QUIT,
  SDL_KEYDOWN,
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
};

enum {
  KMOD_ALT = (1 << 0),
};

#endif  /* SDL_h_ */
