#include "SDL_timer.h"

#include <emscripten.h>

#include <stdio.h>
#include <unistd.h>
#include "myerror.h"

void SDL_Delay(Uint32 msec) {
  mesg(">>> SDL_Delay(%u) ignored!\n", msec);
  /*
  if (msec > 50)
    msec = 50;
  usleep(msec * 1000);
  */
}

Uint32 SDL_GetTicks() {
  return (Uint32) emscripten_get_now();
}
