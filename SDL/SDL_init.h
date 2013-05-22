#ifndef SDL_init_h_
#define SDL_init_h_

enum {
  SDL_INIT_VIDEO = 1 << 0,
  SDL_INIT_AUDIO = 1 << 1,
  SDL_INIT_JOYSTICK = 1 << 2,
};

int SDL_Init(int init_flags);
void SDL_Quit();

#endif  /* SDL_init_h_ */
