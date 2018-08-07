#include "SDL_audio.h"

#include <stdio.h>

#include "myerror.h"

static SDL_AudioSpec* g_spec = nullptr;

int SDL_OpenAudio(SDL_AudioSpec* spec, SDL_AudioSpec* obtained) {
  mesg("SDL_OpenAudio [channels=%u, format=%u, size=%u, samples=%u, freq=%u, callback=%p]\n",
      spec->channels, spec->format, spec->size, spec->samples, spec->freq, spec->callback);
  g_spec = spec;
  return 0;
}

void SDL_CloseAudio() {
  mesg("SDL_CloseAudio\n");
  g_spec = nullptr;
}

void SDL_PauseAudio(int pause) {
  mesg("SDL_PauseAudio(%d)\n", pause);
}

void* SDL_LoadWAV(const char* wavefile, SDL_AudioSpec*, Uint8** samples, Uint32* num_samples) { return 0; }
void SDL_FreeWAV(Uint8* samples) {}
