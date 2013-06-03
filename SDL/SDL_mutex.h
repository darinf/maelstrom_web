#ifndef SDL_mutex_h_
#define SDL_mutex_h_

/* placeholder */

struct SDL_mutex;

SDL_mutex* SDL_CreateMutex();
void SDL_DestroyMutex(SDL_mutex*);
void SDL_mutexV(SDL_mutex*);
void SDL_mutexP(SDL_mutex*);

#endif  /* SDL_mutex_h_ */
