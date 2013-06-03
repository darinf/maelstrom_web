#ifndef SDL_wm_h_
#define SDL_wm_h_

#include "SDL_types.h"

struct SDL_Surface;

int SDL_WM_ToggleFullScreen(SDL_Surface*);
void SDL_WM_SetCaption(const char* title, const char* icon);
void SDL_WM_SetIcon(SDL_Surface*, const Uint8* mask);
int SDL_ShowCursor(int);

#endif  /* SDL_wm_h_ */
