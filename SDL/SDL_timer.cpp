#include "SDL_timer.h"

#include <unistd.h>
#include "ppb.h"

void SDL_Delay(Uint32 msec) {
  usleep(msec * 1000);
}

Uint32 SDL_GetTicks() {
  return static_cast<Uint32>(ppb.core->GetTimeTicks() * 1000.0);
}
