#ifndef SDL_event_h_
#define SDL_event_h_

#include "SDL_types.h"

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
  SDL_RELEASED = 0,
  SDL_PRESSED = 1,
};

typedef int SDLKey;

struct SDL_keysym {
  SDLKey sym;
  int mod;
  int unicode;
};

struct SDL_Event {
  Uint32 type;
  struct {
    SDL_keysym keysym;
    int state;
  } key;
  struct {
    int x, y;
    int button;
  } button;
};

int SDL_PollEvent(SDL_Event*);
int SDL_WaitEvent(SDL_Event*);
const char* SDL_GetKeyName(SDLKey);
void SDL_EnableUNICODE(int);

#endif  /* SDL_event_h_ */
