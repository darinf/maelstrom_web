#include "SDL_audio.h"

#include <stdio.h>

#if 0
#include "ppb.h"

extern PP_Instance g_instance;

namespace {

PP_Resource g_audio;
Uint8* g_buffer;
Uint32 g_buffer_size;

void AudioCallback(void* sample_buffer,
                   uint32_t buffer_size_in_bytes,
                   PP_TimeDelta latency,
                   void* user_data) {
  if (buffer_size_in_bytes != g_buffer_size * 4 * 2 * 2) {
    fprintf(stderr, "AudioCallback: unexpected buffer_size_in_bytes = %u\n", buffer_size_in_bytes);
  }

  SDL_AudioSpec* spec = static_cast<SDL_AudioSpec*>(user_data);
  spec->callback(spec->userdata, g_buffer, g_buffer_size);

  Uint16* output = static_cast<Uint16*>(sample_buffer);
  for (Uint32 i = 0; i < g_buffer_size; ++i) {
    // XXX: Need to scale audio sample?

    //Uint16 value = (Uint16) ((g_buffer[i] * 65535.0) / 255.0);
    Uint16 value = ((Uint16) g_buffer[i]) << 6;
    
    // Write the same sample 4 times to perform conversion
    // from 11025 Hz to 44100 Hz.

    *output++ = value;  // L
    *output++ = value;  // R

    *output++ = value;  // L
    *output++ = value;  // R

    *output++ = value;  // L
    *output++ = value;  // R

    *output++ = value;  // L
    *output++ = value;  // R
  }
}

}  // namespace
#endif

int SDL_OpenAudio(SDL_AudioSpec* spec, SDL_AudioSpec* obtained) {
  fprintf(stderr, "Unimplemented: SDL_OpenAudio [channels=%u, format=%u, size=%u, samples=%u, freq=%u, callback=%p]\n",
      spec->channels, spec->format, spec->size, spec->samples, spec->freq, spec->callback);
#if 0
  if (spec->freq != 11025) {
    fprintf(stderr, "SDL_OpenAudio: unsupported frequency!\n");
    return -1;
  }

  if (spec->format != AUDIO_U8) {
    fprintf(stderr, "SDL_OpenAudio: unsupported format!\n");
    return -1;
  }

  if (g_audio) {
    fprintf(stderr, "SDL_OpenAudio: need to close audio first!\n");
    return -1;
  }

  Uint32 sample_frame_count =
      ppb.audio_config->RecommendSampleFrameCount(g_instance,
                                                  PP_AUDIOSAMPLERATE_44100,
                                                  PP_AUDIOMINSAMPLEFRAMECOUNT);
  //fprintf(stderr, "  recommended sample frame count = %u\n", sample_frame_count);

  g_buffer_size = sample_frame_count / 4;
  g_buffer = new Uint8[g_buffer_size];

  PP_Resource audio_config =
      ppb.audio_config->CreateStereo16Bit(g_instance,
                                          PP_AUDIOSAMPLERATE_44100,
                                          sample_frame_count);
  
  g_audio = ppb.audio->Create(g_instance,
                              audio_config,
                              AudioCallback,
                              spec);

  ppb.core->ReleaseResource(audio_config);
  return 0;
#endif
  return -1;
}

void SDL_CloseAudio() {
  fprintf(stderr, "Unimplemented: SDL_CloseAudio\n");
#if 0
  ppb.core->ReleaseResource(g_audio);
  g_audio = 0;

  delete[] g_buffer;
  g_buffer = NULL;
  g_buffer_size = 0;
#endif
}

void SDL_PauseAudio(int pause) {
  fprintf(stderr, "Unimplemented: SDL_PauseAudio(%d)\n", pause);
#if 0
  if (pause) {
    ppb.audio->StopPlayback(g_audio);
  } else {
    ppb.audio->StartPlayback(g_audio);
  }
#endif
}

void* SDL_LoadWAV(const char* wavefile, SDL_AudioSpec*, Uint8** samples, Uint32* num_samples) { return 0; }
void SDL_FreeWAV(Uint8* samples) {}
