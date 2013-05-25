#include "SDL_rwops.h"

#include <stdio.h>

const char* SDL_GetError() { return 0; }
void SDL_SetError(const char*) {}

SDL_RWops* SDL_RWFromFile(const char* path, const char* mode) {
  fprintf(stderr, "SDL_RWFromFile(\"%s\")", path);
  fflush(stderr);
  return 0;
}

int SDL_RWclose(SDL_RWops*) { return 0; }
Uint16 SDL_ReadBE16(SDL_RWops*) { return 0; }
Uint32 SDL_ReadBE32(SDL_RWops*) { return 0; }
Uint32 SDL_RWread(SDL_RWops*, void* buf, Uint32 size, Uint32 count) { return 0; }
Uint32 SDL_RWwrite(SDL_RWops*, const void* buf, Uint32 size, Uint32 count) { return 0; }
Uint32 SDL_WriteBE32(SDL_RWops*, Uint32) { return 0; }
Uint32 SDL_WriteLE32(SDL_RWops*, Uint32) { return 0; }
Uint32 SDL_RWseek(SDL_RWops*, Sint32 offset, int whence) { return 0; }
Uint32 SDL_RWtell(SDL_RWops*) { return 0; }
