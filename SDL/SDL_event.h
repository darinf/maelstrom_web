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
  SDLK_BACKSPACE = 8,
  SDLK_TAB = 9,
  SDLK_RETURN = 13,
  SDLK_PAUSE = 19,
  SDLK_ESCAPE = 27,
  SDLK_SPACE = 32,
  SDLK_LEFT = 37,
  SDLK_UP = 38,
  SDLK_RIGHT = 39,
  SDLK_DELETE = 46,
  SDLK_0 = 48,
  SDLK_1 = 49,
  SDLK_2 = 50,
  SDLK_3 = 51,
  SDLK_4 = 52,
  SDLK_5 = 53,
  SDLK_6 = 54,
  SDLK_7 = 55,
  SDLK_8 = 56,
  SDLK_9 = 57,
  SDLK_a = 65,
  SDLK_c = 67,
  SDLK_l = 76,
  SDLK_p = 80,
  SDLK_q = 81,
  SDLK_x = 88,
  SDLK_z = 90,
  SDLK_F1 = 112,
  SDLK_F3 = 114,
  SDLK_LSHIFT = 0xA0,
  SDLK_RSHIFT = 0xA1,
  SDLK_LCTRL = 0xA2,
  SDLK_RCTRL = 0xA3,
  SDLK_LALT = 0xA4,
  SDLK_RALT = 0xA5,
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
