#ifndef SDL_audio_h_
#define SDL_audio_h_

/* placeholder */

#include "SDL_types.h"

struct SDL_AudioSpec {
  int format;
  Uint32 channels;
  Uint32 size;
  Uint32 samples;
  Uint32 freq;
  void (*callback)(void *userdata, Uint8 *stream, int len);
  void* userdata;
};

enum {
  AUDIO_U8,
};

int SDL_OpenAudio(SDL_AudioSpec*, int*);
void SDL_CloseAudio();
void SDL_PauseAudio(int);

void* SDL_LoadWAV(const char* wavefile, SDL_AudioSpec*, Uint8** samples, Uint32* num_samples);
void SDL_FreeWAV(Uint8* samples);

#endif  /* SDL_audio_h_ */
