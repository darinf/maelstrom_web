#include "SDL_timer.h"

#include <emscripten.h>

#include <stdio.h>
#include <unistd.h>
#include "myerror.h"

void SDL_Delay(Uint32 msec) {
  //mesg(">>> SDL_Delay(%u) ignored!\n", msec);
  /*
  if (msec > 50)
    msec = 50;
  usleep(msec * 1000);
  */
  EM_ASM_({
    var sab = new SharedArrayBuffer(4);
    var int32 = new Int32Array(sab);
    Atomics.wait(int32, 0, 0, $0);
  }, msec);
}

Uint32 SDL_GetTicks() {
  return (Uint32) emscripten_get_now();
}
