#include "SDL_audio.h"

#include <stdio.h>

int SDL_OpenAudio(SDL_AudioSpec* spec, SDL_AudioSpec* obtained) {
  fprintf(stderr, "SDL_OpenAudio [channels=%u, size=%u, samples=%u, freq=%u]\n",
      spec->channels, spec->size, spec->samples, spec->freq);
  return 0;
}

void SDL_CloseAudio() {
}

void SDL_PauseAudio(int) {
}

void* SDL_LoadWAV(const char* wavefile, SDL_AudioSpec*, Uint8** samples, Uint32* num_samples) { return 0; }
void SDL_FreeWAV(Uint8* samples) {}
