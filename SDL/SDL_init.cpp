#include <emscripten.h>

#include "SDL_init.h"

int SDL_Init(int init_flags) {
  EM_ASM(
    FS.mkdir('/Maelstrom');
    FS.mount(IDBFS, {}, '/Maelstrom');
  );

  return 0;
}

void SDL_Quit() {}
