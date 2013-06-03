#include "SDL_mutex.h"

SDL_mutex* SDL_CreateMutex() { return 0; }
void SDL_DestroyMutex(SDL_mutex*) {}
void SDL_mutexV(SDL_mutex*) {}
void SDL_mutexP(SDL_mutex*) {}
