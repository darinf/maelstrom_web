#include "SDL_timer.h"

#include <emscripten.h>

#include <stdio.h>
#include <unistd.h>
#include "myerror.h"

void SDL_Delay(Uint32 msec) {
  EM_ASM_({
    var int32 = new Int32Array(new SharedArrayBuffer(4));
    Atomics.wait(int32, 0, 0, $0);
  }, msec);
}

Uint32 SDL_GetTicks() {
  return (Uint32) emscripten_get_now();
}
