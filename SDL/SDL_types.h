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

struct SDL_Palette {
  SDL_Color colors[2];
};

struct SDL_PixelFormat {
  SDL_Palette* palette;  
};

struct SDL_Rect {
  int x, y, w, h;
};

struct SDL_Surface {
  SDL_PixelFormat* format;
  Uint32 w, h;
  Uint32 pitch;
  void* pixels;
};

struct SDL_RWops {
};

#endif  /* SDL_types_h_ */
