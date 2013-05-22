#ifndef SDL_thread_h_
#define SDL_thread_h_

/* placeholder */

struct SDL_Thread;

SDL_Thread* SDL_CreateThread(int (*func)(void* userdata), void* userdata);
void SDL_WaitThread(SDL_Thread*, int*);

#endif  /* SDL_thread_h_ */
