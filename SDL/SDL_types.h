#ifndef SDL_types_h_
#define SDL_types_h_

/* placeholder */

typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;

typedef signed char Sint8;
typedef signed short Sint16;
typedef signed int Sint32;

struct SDL_Color {
  Uint8 r, g, b, a;
};

typedef int SDLKey;

struct SDL_keysym {
  SDLKey sym;
  int mod;
};

struct SDL_Event {
  Uint32 type;
  struct {
    SDL_keysym keysym;
  } key;
  struct {
    int x, y;
  } button;
};

struct SDL_PixelFormat;

struct SDL_Rect {
  int x, y, w, h;
};

struct SDL_Surface {
  SDL_PixelFormat* format;
  int w, h;
};

#endif  /* SDL_types_h_ */
