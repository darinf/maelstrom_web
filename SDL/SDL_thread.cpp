#include "SDL_thread.h"

SDL_Thread* SDL_CreateThread(int (*func)(void* userdata), void* userdata) { return 0; }
void SDL_WaitThread(SDL_Thread*, int*) {}
