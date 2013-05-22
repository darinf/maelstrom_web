#include "SDL_audio.h"

int SDL_OpenAudio(SDL_AudioSpec*, int*) { return 0; }
void SDL_CloseAudio() {}
void SDL_PauseAudio(int) {}
void* SDL_LoadWAV(const char* wavefile, SDL_AudioSpec*, Uint8** samples, Uint32* num_samples) { return 0; }
void SDL_FreeWAV(Uint8* samples) {}
